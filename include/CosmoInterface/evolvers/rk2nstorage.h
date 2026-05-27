#ifndef COSMOINTERFACE_EVOLVERS_RK2NSTORAGE_H
#define COSMOINTERFACE_EVOLVERS_RK2NSTORAGE_H
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/evolvers/rk2nstorageparameters.h"
#include "CosmoInterface/evolvers/kernels/kernels.h"
#include "CosmoInterface/evolvers/kernels/kernelstypes.h"
#include "CosmoInterface/definitions/averages.h"
#include "CosmoInterface/definitions/nonminimalcoupling.h"
#include "CosmoInterface/extrafields.h"

#include "TempLat/lattice/algebra/listoperators/norm.h" // norm2 (per-species squared magnitude)
#include "TempLat/util/exception.h"

#include <cmath>
#include <algorithm>

namespace TempLat
{

  MakeException(AdaptiveToleranceNotSet);

  /** \brief A class which implements low storage ("2N-storage") explicit RK methods.
   *
   *
   **/
  template <typename Model> class RK2NStorage
  {
  public:
    using T = typename Model::FloatType;

    /* Put public methods here. These should change very little over time. */
    // NOTE on adaptive clamps: `dtProposed`, `dtMin`, and `dtMax` below are seeded
    // ONCE from `model.dt` at construction time. The adaptive controller never
    // re-reads `model.dt` to refresh them — `model.dt` is mutated in-loop and
    // the clamps stay fixed. Callers must therefore set `model.dt` to the
    // intended INITIAL dt BEFORE constructing the evolver. (Adaptive cooling
    // drivers `mccooling.cpp` / `mccoolingmeasurer.h` do this explicitly; the
    // standard `Evolver<>` wrapper relies on `model.dt = runParams.dt` already
    // having been set by the model's constructor.) If you ever expose
    // user-overridable dt floors/ceilings, add them to `RunParameters` and pass
    // them through here.
    RK2NStorage(Model &model, RunParameters<T> runParams)
        : type(runParams.eType), dt(model.dt), As(RK2NStorageParameters<T>::getAs(type)),
          Bs(RK2NStorageParameters<T>::getBs(type)), expansion(runParams.expansion),
          adaptive(RK2NStorageParameters<T>::isAdaptative(runParams.eType)),
          tolerance(runParams.tolerance), dtProposed(model.dt), dtMin(T(1e-6) * model.dt),
          dtMax(T(1e3) * model.dt), safety(T(0.95)), growthFactorMax(T(5)),
          methodOrder(RK2NStorageParameters<T>::order(type)), maxRejects(10)
    {
      ForEachField(Model, fld, n, isDefined[fld].emplace_back(true); isDeactivated[fld].emplace_back(false););
      if (adaptive && tolerance <= T(0))
        throw(AdaptiveToleranceNotSet(
            "Adaptive RK2N evolvers (e.g. RK3_4_A) require a positive 'tolerance' in the input file; "
            "got tolerance <= 0. Abort."));
    }

    template <int N> void activate(Tag<N> t) { ForLoop(n, 0, Model::getNFields(t) - 1, isDeactivated[t][n] = false;); }

    template <int N> void deactivate(Tag<N> t) { ForLoop(n, 0, Model::getNFields(t) - 1, isDeactivated[t][n] = true;); }

    template <int FLD, int N> bool isActive(Tag<FLD>, Tag<N>) const
    { return !isDeactivated[FLD][N] && isDefined[FLD][N]; }

    void evolve(Model &model, T tMinust0) { evolve(model, tMinust0, KernelsTypes::EoM<Model>()); }

    template <class KernelType> void evolve(Model &model, T tMinust0, KernelType kt)
    {

      /*
       * 2N storage RK ....
       *
       *
       * */

      if (!adaptive) {
        // Fixed-dt path: unchanged behaviour.
        dt = model.dt;
        doStep(model, tMinust0, kt);
        return;
      }

      // Adaptive path: attempt a step, estimate the proxy local error, and accept or reject
      // it, adjusting dt. A rejected step is restored from the backup and retried smaller.
      model.dt = clampDt(dtProposed); // trial dt for this step
      size_t rejects = 0;
      T ratio = T(0);
      bool accepted = false;
      do {
        dt = model.dt; // current trial dt
        snapshot(model);                     // back up y_n so a rejected step can be undone
        doStep(model, tMinust0, kt);         // attempt the step (overwrites the model fields)
        ratio = errorNorm() / tolerance;     // proxy local error relative to tolerance
        if (ratio > T(1) && rejects < maxRejects && dt > dtMin) {
          restore(model, tMinust0); // undo the rejected step
          model.dt = clampDt(dt * safety * std::pow(T(1) / ratio, T(1) / T(methodOrder)));
          ++rejects;
        } else {
          accepted = true; // accept: within tolerance, or out of retries / at the dt floor
        }
      } while (!accepted);

      const T dtAccepted = dt;
      // Propose a (clamped) larger dt for the NEXT step, but leave model.dt at the accepted
      // value so the main loop advances t by the step actually taken (t += model.dt).
      const T grow = std::min(growthFactorMax,
                              safety * std::pow(T(1) / std::max(ratio, T(1e-300)), T(1) / T(methodOrder)));
      dtProposed = clampDt(dtAccepted * grow);
      model.dt = dtAccepted;
    }

    // One full RK2N sweep over all stages. Shared by the fixed-dt and adaptive paths; this is
    // exactly the original evolve body, so fixed-dt behaviour is unchanged.
    template <class KernelType> void doStep(Model &model, T tMinust0, KernelType kt)
    {
      kt.cache(model, tMinust0); // To be able to store some temporary info in the kernel type

      for (size_t i = 0; i < As.size(); ++i) { // loop over operations...

        ForEachField(
            Model, fld, n, if (!isDeactivated[fld][n]) {
              isDefined[fld][n] = delta(i, Delta->get(fld)(n), Kernels::get(fld, model, n, kt));
            });

        if (expansion) sfDefined = deltaScaleFactor(model, i, kt);

        ForEachField(Model, fld, n, if (!isDeactivated[fld][n] && isDefined[fld][n]) { advance(i, fld, model, n); });

        if (expansion) advanceScaleFactor(model, i);

        if (expansion) {
          Averages::setAllAverages(model);
          sync(model, tMinust0);
        }
        kt.cache(model, tMinust0);
      }
    }

    bool deltaScaleFactor(Model &model, size_t i, KernelsTypes::EoM<Model> kt)
    {
      if (i == 0) {
        deltaADot = dt * ScaleFactorKernels::get(model, kt);
        deltaA = dt * model.aDotI;
      } else {
        deltaADot = As[i] * deltaADot + dt * ScaleFactorKernels::get(model, kt);
        deltaA = As[i] * deltaA + dt * model.aDotI;
      }
      return true;
    }
    template <class KernelType> bool deltaScaleFactor(Model &model, size_t i, KernelType) { return false; }

    void advanceScaleFactor(Model &model, size_t i)
    {
      if (sfDefined) model.aI += Bs[i] * deltaA;

      if constexpr (not Model::IsNonMinimallyCoupled) {
        if (sfDefined) model.aDotI += Bs[i] * deltaADot;
      } else {
        if (sfDefined) model.piAI += Bs[i] * deltaADot;
        if (sfDefined) model.aDotI = model.piAI * pow(model.aI, model.alpha - 1);
      }
    }

    // This function is called before doing the measurements or in the case of NMC evolution to sync the value of R obtained from the volumen averages of the matter fields. It is used to set aDotI to its correct value in case
    // of a background expansion.
    void sync(Model &model, T tMinust0)
    {
      //  if(fixedBackground) model.aDotI = aBackground.dot(tMinust0);
      
      if constexpr (Model::IsNonMinimallyCoupled) {
            model.RI = NonMinimalCoupling::R(model);
      }
    }

    void setDelta(ExtraFields<Model> extraFlds) { Delta = extraFlds.getAllFlds1(); }

    // Adaptive RK2N: borrow the y_n backup buffers from ExtraFields (mirrors setDelta).
    void setBackup(ExtraFields<Model> extraFlds)
    {
      backup = extraFlds.getBackup();
      linkBackup = extraFlds.getLinkBackup();
    }

    // Re-seed the adaptive controller's next-trial dt from model.dt. Use this
    // when a long-lived evolver is reused across logically-distinct integration
    // runs (e.g. MCCoolingMeasurer reuses a single evolver across MC sweeps) —
    // without this call, the trial dt for the next run inherits the converged
    // dt from the end of the previous run instead of starting from the
    // caller's requested initial dt. No-op when not adaptive (the controller
    // is never entered on the fixed-dt path).
    void resetAdaptiveState(Model &model) { dtProposed = model.dt; }

    // Save the full dynamical state (all field species + scale-factor state) so an attempted
    // step can be undone if rejected. A plain elementwise copy is correct for every species
    // (the SU2 exp() update only applies during accumulation, not during a copy). SU2 links
    // are copied into a dedicated group-element buffer; see backupField().
    void snapshot(Model &model)
    {
      ForEachField(Model, fld, n, backupField(model, fld, n););
      if (expansion) {
        aBak = model.aI;
        aDotBak = model.aDotI;
        if constexpr (Model::IsNonMinimallyCoupled) piABak = model.piAI;
      }
    }

    // Restore the full dynamical state from the backup buffers (used on a rejected step).
    // After restoring fields and scale-factor state, recompute the volume averages and sync
    // so the next attempt starts from a consistent state (mirrors the expansion bookkeeping
    // in evolve()).
    void restore(Model &model, T tMinust0)
    {
      ForEachField(Model, fld, n, restoreField(model, fld, n););
      if (expansion) {
        model.aI = aBak;
        model.aDotI = aDotBak;
        if constexpr (Model::IsNonMinimallyCoupled) model.piAI = piABak;
        Averages::setAllAverages(model);
        sync(model, tMinust0);
      }
    }

  private:
    // Copy one model field component into the backup. SU2 links go to the group-element
    // linkBackup; every other species into the FieldsAsInModel backup (whose types match).
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

    template <class Delta, class Kernel> bool delta(size_t i, Delta delta, Kernel kernel)
    {
      if (i == 0) {
        delta = dt * kernel;
      } else {
        delta = As[i] * delta + dt * kernel;
      }
      return true;
    }

    template <class Delta> bool delta(size_t i, Delta delta, ZeroType kernel) { return false; }

    template <int FLD, int N> void advance(size_t i, Tag<FLD> fld, Model &model, Tag<N> n)
    {
      model.getField(fld)(n) += Bs[i] * Delta->get(fld)(n);
    }

    template <int N> void advance(size_t i, FieldsNumbering::fldSU2 fld, Model &model, Tag<N> n)
    {
      ForLoop(j, 1, Model::NDim, model.fldSU2(n)(j) = exp(Bs[i] * Delta->fldSU2(n)(j)) * model.fldSU2(n)(j););
    }

    // ---- Adaptive step-size control (proxy error estimate) ----

    T clampDt(T x) const { return std::min(dtMax, std::max(dtMin, x)); }

    // Global RMS of the last-stage increment Bs.back()*Delta over ALL active field species --
    // the proxy local-error estimate. After doStep(), Delta holds the last stage's register,
    // so |Bs.back()*Delta| is the size of the final increment applied to the fields. Bs.back()
    // is a real scalar, so its square factors out of the per-component sum.
    T errorNorm()
    {
      T ms = 0; // sum over active components of the volume-mean squared Delta increment
      ForEachField(Model, fld, n,
                   if (!isDeactivated[fld][n] && isDefined[fld][n]) { ms += componentMS(fld, n); });
      return std::abs(Bs.back()) * std::sqrt(ms);
    }

    // Volume-mean of the squared magnitude of one Delta component, dispatched by field type
    // (mirrors the per-species magnitude idioms in FieldFunctionals: pow<2> for real scalars,
    // norm2 for complex / SU2-doublet, and a sum over spatial directions for the gauge vectors).
    template <int FLD, int N> T componentMS(Tag<FLD> fld, Tag<N> n)
    {
      if constexpr (FLD == FieldsNumbering::fldCS::value || FLD == FieldsNumbering::piCS::value ||
                    FLD == FieldsNumbering::fldSU2Doublet::value ||
                    FLD == FieldsNumbering::piSU2Doublet::value)
        return average(norm2(Delta->get(fld)(n)));
      else if constexpr (FLD == FieldsNumbering::fldU1::value || FLD == FieldsNumbering::piU1::value)
        return average(Total(j, 1, Model::NDim, pow<2>(Delta->get(fld)(n)(j))));
      else if constexpr (FLD == FieldsNumbering::fldSU2::value || FLD == FieldsNumbering::piSU2::value)
        // SU2 link increment: sum pow<2> over spatial directions j and su(2) generators b=1..3
        // (mirrors FieldFunctionals::pi2SU2's Total over directions and algebra components).
        return average(Total(j, 1, Model::NDim, Total(b, 1, 3, pow<2>(Delta->get(fld)(n)(j)(b)))));
      else // fldS / piS: real scalar singlets
        return average(pow<2>(Delta->get(fld)(n)));
    }

    /* Put all member variables and private methods here. These may change arbitrarily. */

    EvolverType type;

    T dt; // Has its own dt as we can use it for other things than real time evolution (cooling for instance).

    const std::vector<T> As;
    const std::vector<T> Bs;

    std::shared_ptr<FieldsAsInModel<Model>> Delta;

    // y_n backup buffers, borrowed from ExtraFields; allocated only for adaptive evolvers.
    // backup holds all species except SU2 links; linkBackup holds the SU2 link group elements
    // (FieldsAsInModel stores fldSU2 as a Lie-algebra field, the wrong type for a link backup).
    std::shared_ptr<FieldsAsInModel<Model>> backup;
    std::shared_ptr<VectorFieldCollection<SU2Field<T, Model::NDim>, Model::NSU2>> linkBackup;

    std::array<std::vector<bool>, FieldsNumbering::maxNum + 1> isDefined;
    std::array<std::vector<bool>, FieldsNumbering::maxNum + 1> isDeactivated;

    T deltaA, deltaADot;
    bool sfDefined;

    bool expansion;
    bool adaptive;

    // ---- Adaptive control state / parameters (used only when adaptive). Defaults set in the
    //      constructor. ----
    T tolerance;        // local-error tolerance (input "tolerance"); required > 0 if adaptive
    T dtProposed;       // dt to attempt on the next step
    T dtMin, dtMax;     // dt clamps (set from the initial dt)
    T safety;           // controller safety factor
    T growthFactorMax;  // max dt growth per accepted step
    size_t methodOrder; // RK order; sets the controller exponent 1/order
    size_t maxRejects;  // max rejected attempts before forcing acceptance

    // Scale-factor state backup (used with snapshot/restore when expansion is on).
    T aBak, aDotBak, piABak;
  };

} // namespace TempLat

#endif
