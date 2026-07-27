#ifndef COSMOINTERFACE_DEFINITIONS_DEFECTSMODULE_DIFFUSION_H
#define COSMOINTERFACE_DEFINITIONS_DEFECTSMODULE_DIFFUSION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
 *   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
 *   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros,  Year: 2026

#include "TempLat/util/exception.h"
#include "CosmoInterface/evolvers/evolver.h"
#include "CosmoInterface/evolvers/rk2nstoragefields.h"

namespace TempLat
{

  /** @brief A class that applies diffusion to the fields. Currently used to generate initial conditions of defect
    * networks
   *
   *
   **/

  MakeException(DiffusionEvolverInvalid);

  class Diffusion
  {
  public:
    // Put public methods here. These should change very little over time.

    Diffusion() = delete;

    template <typename Model, typename T>
    static void diffuse(Model &model, RunParameters<T> &rPar, FilesManager<Model::NDim> &filesManager,
                        ExtraFields<Model> extraFields)
    {

      bool iAmRoot = model.getToolBox()->amIRoot();
      typename Model::FloatType auxg;

      if constexpr (Model::DefectsModel && Model::Ns == 1) {
        auxg = model.g;
        model.g = 0;
      }

      if (!RK2NStorageParameters<T>::isRK2n(rPar.diffType))
        throw(DiffusionEvolverInvalid(
            "You need to choose a RK integrator to diffuse initial conditions for defect simulations. Abort."));
      if (iAmRoot) say << "Diffusing initial defect conditions";

      auto diffuser = RK2NStorageFields(model, rPar.diffType);

      std::optional<DefectsMeasurer<T>> measurerS;
      std::optional<ComplexScalarMeasurer<T>> measurerCS;
      std::optional<U1Measurer<T>> measurerU1;

      if constexpr (Model::Ns > 0) measurerS.emplace(model, filesManager, rPar, rPar.appendMode, "_diffusion", true);
      if constexpr (Model::NCs > 0) measurerCS.emplace(model, filesManager, rPar, rPar.appendMode, "_diffusion", false);
      if constexpr (Model::NU1 > 0)
        measurerU1.emplace(model, filesManager, rPar, rPar.appendMode, "_diffusion", false, false);
      EnergiesMeasurer<T> measurerE(model, filesManager, rPar, rPar.appendMode, "_diffusion", false);
      EnergySnapshotsMeasurer<Model> snapshots(model, rPar, filesManager, rPar.energySnapshotMeasDiffusion,
                                               "_diffusion");

      model.dt = rPar.dtdiff;
      T tdiff = 0.;
      ptrdiff_t outputFreqDiffusion = static_cast<int>(round(rPar.tOutFreqDiff / rPar.dtdiff));
      ptrdiff_t outputRareFreqDiffusion = static_cast<int>(round(rPar.tOutRareFreqDiff / rPar.dtdiff));

      for (int i = 0; tdiff < rPar.tmaxdiff + rPar.dtdiff * T(0.5); i++) {
        if (i % outputFreqDiffusion == 0) {
          ForEachConjugateMomenta(
              Model, fld, n,
              model.getField(fld)(n) =
                  Kernels::get(fld, model, n,
                               KernelsTypes::Diffusion<Model>())); // Sets the conjugate momenta fields, which are used
                                                                   // as auxiliary variables for the evolution, to zero.
          if (iAmRoot) say << "Diffusion step " << i << " completed. Current diffusion time " << tdiff << "\n";
          if constexpr (Model::Ns > 0) measurerS->measure(model, tdiff);
          if constexpr (Model::NCs > 0) measurerCS->measureStandard(model, tdiff);
          if constexpr (Model::NU1 > 0) measurerU1->measureStandard(model, tdiff);
          measurerE.measure(model, tdiff, i == 0);
        }
        if (i % outputRareFreqDiffusion == 0) snapshots.measure(model, tdiff);

        diffuser.evolve(model, tdiff, KernelsTypes::Diffusion<Model>());
        tdiff += rPar.dtdiff;
      }

      ForLoop(n, 0, Model::Ns - 1, model.piS(n) = 0.;);
      ForLoop(n, 0, Model::NCs - 1, model.piCS(n) = Complexify(0., 0.););
      ForLoop(n, 0, Model::NU1 - 1, ForLoop(i, 1, Model::NDim, model.piU1(n)(i) = 0.;));

      model.dt = rPar.dt;

      if constexpr (Model::DefectsModel && Model::Ns == 1) {
        model.g = auxg;
      }
    }
  };

} // namespace TempLat

#endif
