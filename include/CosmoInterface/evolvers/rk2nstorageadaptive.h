#ifndef COSMOINTERFACE_EVOLVERS_RK2NSTORAGEADAPTIVE_H
#define COSMOINTERFACE_EVOLVERS_RK2NSTORAGEADAPTIVE_H
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Year: 2026

#include "CosmoInterface/evolvers/rk2nstorage.h"
#include "CosmoInterface/evolvers/rk2nstorageparameters.h"
#include "CosmoInterface/evolvers/kernels/kernels.h"
#include "CosmoInterface/evolvers/kernels/kernelstypes.h"
#include "CosmoInterface/definitions/averages.h"
#include "CosmoInterface/definitions/nonminimalcoupling.h"
#include "CosmoInterface/extrafields.h"
#include "CosmoInterface/fieldsnumbering.h"
#include "CosmoInterface/runparameters.h"

#include "TempLat/lattice/algebra/listoperators/norm.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "TempLat/util/exception.h"

#include <algorithm>
#include <cmath>
#include <memory>

namespace TempLat
{

  MakeException(AdaptiveToleranceNotSet);

  /** \brief A composing wrapper around RK2NStorage that adds adaptive (proxy error
   *         estimate) step-size control. The inner RK2NStorage runs a single fixed-dt
   *         RK sweep per attempt; this wrapper owns the controller state, the y_n
   *         backup buffers, and the accept/reject loop.
   *
   *         Instantiated only for adaptive evolver types (currently RK3_4_A);
   *         the fixed-dt path stays entirely on RK2NStorage.
   **/
  template <typename Model> class RK2NStorageAdaptive
  {
  public:
    using T = typename Model::FloatType;

    // NOTE on adaptive clamps: `dtProposed`, `dtMin`, and `dtMax` below are seeded
    // ONCE from `model.dt` at construction time. The adaptive controller never
    // re-reads `model.dt` to refresh them — `model.dt` is mutated in-loop and the
    // clamps stay fixed. Callers must therefore set `model.dt` to the intended
    // INITIAL dt BEFORE constructing the wrapper. (Adaptive cooling drivers
    // `mccooling.cpp` / `mccoolingmeasurer.h` do this explicitly; the standard
    // `Evolver<>` wrapper relies on `model.dt = runParams.dt` already having been
    // set by the model's constructor.) If you ever expose user-overridable dt
    // floors/ceilings, add them to `RunParameters` and pass them through here.
    RK2NStorageAdaptive(Model &model, RunParameters<T> runParams, ExtraFields<Model> extraFlds)
        : inner(std::make_shared<RK2NStorage<Model>>(model, runParams)),
          backup(std::make_shared<FieldsAsInModel<Model>>(model, runParams, "_backup")),
          tolerance(runParams.tolerance), dtProposed(model.dt),
          dtMin(T(1e-6) * model.dt), dtMax(T(1e3) * model.dt),
          safety(T(0.95)), growthFactorMax(T(5)),
          bsBack(RK2NStorageParameters<T>::getBs(
                     RK2NStorageParameters<T>::baseType(runParams.eType))
                     .back()),
          methodOrder(RK2NStorageParameters<T>::order(runParams.eType)),
          maxRejects(10), expansion(runParams.expansion)
    {
      // The wrapper does not own Delta; it shares the same buffer the inner uses.
      // setDelta hands the shared_ptr to the inner; we also keep a copy for
      // errorNorm/componentMS, which read the last-stage increment back from Delta.
      inner->setDelta(extraFlds);
      delta = extraFlds.getAllFlds1();

      // FieldsAsInModel stores fldSU2 as a Lie-algebra field (correct for the
      // Delta increment), but the model's SU2 link is a group element; a faithful
      // link backup needs a dedicated group-element buffer. Allocated only for
      // models that actually carry SU2 links.
      if constexpr (Model::NSU2 > 0)
        linkBackup = std::make_shared<VectorFieldCollection<SU2Field<T, Model::NDim>, Model::NSU2>>(
            "_linkbackup", model.getToolBox(), runParams.getLatParams());

      if (tolerance <= T(0))
        throw(AdaptiveToleranceNotSet(
            "Adaptive RK2N evolvers (e.g. RK3_4_A) require a positive 'tolerance' in the input file; "
            "got tolerance <= 0. Abort."));
    }

    void evolve(Model &model, T tMinust0) { evolve(model, tMinust0, KernelsTypes::EoM<Model>()); }

    template <class KernelType> void evolve(Model &model, T tMinust0, KernelType kt)
    {
      runAdaptiveStep(model, tMinust0, kt);
    }

    void sync(Model &model, T tMinust0) { inner->sync(model, tMinust0); }

    template <int N> void activate(Tag<N> t) { inner->activate(t); }

    template <int N> void deactivate(Tag<N> t) { inner->deactivate(t); }

    // Re-seed the adaptive controller's next-trial dt from model.dt. Use this
    // when a long-lived evolver is reused across logically-distinct integration
    // runs (e.g. MCCoolingMeasurer reuses a single evolver across MC sweeps) —
    // without this call, the trial dt for the next run inherits the converged
    // dt from the end of the previous run instead of starting from the
    // caller's requested initial dt.
    void resetAdaptiveState(Model &model) { dtProposed = model.dt; }

    // Save the full dynamical state (all field species + scale-factor state) so an
    // attempted step can be undone if rejected. A plain elementwise copy is correct
    // for every species (the SU2 exp() update only applies during accumulation, not
    // during a copy). SU2 links are copied into the dedicated group-element
    // linkBackup; see backupField().
    void snapshot(Model &model)
    {
      ForEachField(Model, fld, n, backupField(model, fld, n););
      if (expansion) {
        aBak = model.aI;
        aDotBak = model.aDotI;
        if constexpr (Model::IsNonMinimallyCoupled) piABak = model.piAI;
      }
    }

    // Restore the full dynamical state from the backup buffers (used on a rejected
    // step). After restoring fields and scale-factor state, recompute the volume
    // averages and sync so the next attempt starts from a consistent state
    // (mirrors the expansion bookkeeping in the inner's evolve()).
    void restore(Model &model, T tMinust0)
    {
      ForEachField(Model, fld, n, restoreField(model, fld, n););
      if (expansion) {
        model.aI = aBak;
        model.aDotI = aDotBak;
        if constexpr (Model::IsNonMinimallyCoupled) model.piAI = piABak;
        Averages::setAllAverages(model);
        inner->sync(model, tMinust0);
      }
    }

  private:
    // Try-and-adjust loop: attempt one fixed-dt RK sweep on the inner, estimate the
    // proxy local error, accept or reject, propose a new dt for the next step.
    // Equivalent to the adaptive branch currently inside RK2NStorage::evolve
    // (rk2nstorage.h:86-113), but operating on `inner` instead of self.
    template <class KernelType> void runAdaptiveStep(Model &model, T tMinust0, KernelType kt)
    {
      model.dt = clampDt(dtProposed); // trial dt for this step
      size_t rejects = 0;
      T ratio = T(0);
      bool accepted = false;
      do {
        snapshot(model);                       // back up y_n so a rejected step can be undone
        inner->evolve(model, tMinust0, kt);    // attempt the step (overwrites the model fields)
        ratio = errorNorm() / tolerance;       // proxy local error relative to tolerance
        if (ratio > T(1) && rejects < maxRejects && model.dt > dtMin) {
          restore(model, tMinust0);            // undo the rejected step
          model.dt = clampDt(model.dt * safety * std::pow(T(1) / ratio, T(1) / T(methodOrder)));
          ++rejects;
        } else {
          accepted = true; // accept: within tolerance, or out of retries / at the dt floor
        }
      } while (!accepted);

      const T dtAccepted = model.dt;
      // Propose a (clamped) larger dt for the NEXT step, but leave model.dt at the accepted
      // value so the main loop advances t by the step actually taken (t += model.dt).
      const T grow = std::min(growthFactorMax,
                              safety * std::pow(T(1) / std::max(ratio, T(1e-300)), T(1) / T(methodOrder)));
      dtProposed = clampDt(dtAccepted * grow);
      model.dt = dtAccepted;
    }

    // Copy one model field component into the backup. SU2 links go to the dedicated
    // group-element linkBackup; every other species into the FieldsAsInModel backup
    // (whose types match the model's storage).
    template <int FLD, int N> void backupField(Model &model, Tag<FLD> fld, Tag<N> n)
    {
      if constexpr (FLD == FieldsNumbering::fldSU2::value)
        (*linkBackup)(n) = model.getField(fld)(n);
      else
        backup->get(fld)(n) = model.getField(fld)(n);
    }

    // Restore one model field component from the backup (inverse of backupField()).
    template <int FLD, int N> void restoreField(Model &model, Tag<FLD> fld, Tag<N> n)
    {
      if constexpr (FLD == FieldsNumbering::fldSU2::value)
        model.getField(fld)(n) = (*linkBackup)(n);
      else
        model.getField(fld)(n) = backup->get(fld)(n);
    }

    // ---- Adaptive step-size control (proxy error estimate) ----

    T clampDt(T x) const { return std::min(dtMax, std::max(dtMin, x)); }

    // Global RMS of the last-stage increment Bs.back()*Delta over ALL active field
    // species — the proxy local-error estimate. After inner->evolve(), Delta holds
    // the last stage's register, so |Bs.back()*Delta| is the size of the final
    // increment applied to the fields. `bsBack` is a real scalar, so its square
    // factors out of the per-component sum.
    T errorNorm()
    {
      T ms = 0; // sum over active components of the volume-mean squared Delta increment
      ForEachField(Model, fld, n,
                   if (inner->isActive(fld, n)) { ms += componentMS(fld, n); });
      return std::abs(bsBack) * std::sqrt(ms);
    }

    // Volume-mean of the squared magnitude of one Delta component, dispatched by
    // field type (mirrors the per-species magnitude idioms in FieldFunctionals:
    // pow<2> for real scalars, norm2 for complex / SU2-doublet, and a sum over
    // spatial directions for the gauge vectors).
    template <int FLD, int N> T componentMS(Tag<FLD> fld, Tag<N> n)
    {
      if constexpr (FLD == FieldsNumbering::fldCS::value || FLD == FieldsNumbering::piCS::value ||
                    FLD == FieldsNumbering::fldSU2Doublet::value ||
                    FLD == FieldsNumbering::piSU2Doublet::value)
        return average(norm2(delta->get(fld)(n)));
      else if constexpr (FLD == FieldsNumbering::fldU1::value || FLD == FieldsNumbering::piU1::value)
        return average(Total(j, 1, Model::NDim, pow<2>(delta->get(fld)(n)(j))));
      else if constexpr (FLD == FieldsNumbering::fldSU2::value || FLD == FieldsNumbering::piSU2::value)
        // SU2 link increment: sum pow<2> over spatial directions j and su(2) generators b=1..3
        // (mirrors FieldFunctionals::pi2SU2's Total over directions and algebra components).
        return average(Total(j, 1, Model::NDim, Total(b, 1, 3, pow<2>(delta->get(fld)(n)(j)(b)))));
      else // fldS / piS: real scalar singlets
        return average(pow<2>(delta->get(fld)(n)));
    }

    // ---- Composition + ownership ----

    std::shared_ptr<RK2NStorage<Model>> inner;
    std::shared_ptr<FieldsAsInModel<Model>> backup;
    // Correctly-typed (group-element) backup for SU2 links. Allocated in the ctor
    // only when Model::NSU2 > 0; null otherwise (never deref'd in that case).
    std::shared_ptr<VectorFieldCollection<SU2Field<T, Model::NDim>, Model::NSU2>> linkBackup;

    // Same Delta the inner uses (set via inner->setDelta in the ctor body).
    std::shared_ptr<FieldsAsInModel<Model>> delta;

    // ---- Adaptive control state / parameters. ----
    T tolerance;        // local-error tolerance (input "tolerance"); required > 0
    T dtProposed;       // dt to attempt on the next step
    T dtMin, dtMax;     // dt clamps (set from the initial dt)
    T safety;           // controller safety factor
    T growthFactorMax;  // max dt growth per accepted step
    T bsBack;           // cached Bs.back() of the base RK2N method
    size_t methodOrder; // RK order; sets the controller exponent 1/order
    size_t maxRejects;  // max rejected attempts before forcing acceptance
    bool expansion;

    // Scale-factor state backup (used with snapshot/restore when expansion is on).
    T aBak, aDotBak, piABak;
  };

} // namespace TempLat

#endif
