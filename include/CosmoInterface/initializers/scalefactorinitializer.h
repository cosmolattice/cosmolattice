#ifndef COSMOINTERFACE_INITIALIZERS_SCALEFACTORINITIALIZER_H
#define COSMOINTERFACE_INITIALIZERS_SCALEFACTORINITIALIZER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/common.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "CosmoInterface/evolvers/evolvertype.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/definitions/energies.h"
#include "CosmoInterface/definitions/nonminimalcoupling.h"

namespace TempLat
{
  /** @brief A class which is used to initialize the scale factor.
   *
   **/
  class ScaleFactorInitializer
  {
  public:
    // Put public methods here. These should change very little over time.
    ScaleFactorInitializer() = delete;

    template <class Model, typename T> static void initializeScaleFactor(Model &model, RunParameters<T> &rPar)
    {
      // If fixed background, the initial Hubble parameter H0 is given by the user
      if (rPar.fixedBackground) {
        model.aDotI = rPar.H0 / model.omegaStar;
        if constexpr (Model::IsNonMinimallyCoupled) model.RI = -9.0 * pow<2>(rPar.H0 / model.omegaStar) * (rPar.omegaEoS - 1.0/3.0); 
        // H0 is in GeV, so we transform to program variables
      }

      // If self-consistent expansion, the initial Hubble parameter must be computed
      // via the 1st Friedmann eqn:
      else {
        T kin = 0; // initial kinetic energy
        ForLoop(i, 0, Model::Ns - 1,
                kin += Energies::kineticS(model, pow<2>(model.piS0(i) / model.fStar / model.omegaStar)););
        ForLoop(i, 0, Model::NCs - 1,
                kin += Energies::kineticCS(model, norm2(model.piCS0(i) / model.fStar / model.omegaStar)););
        ForLoop(i, 0, Model::NSU2Doublet - 1,
                kin +=
                Energies::kineticSU2Doublet(model, norm2(model.piSU2Doublet0(i) / model.fStar / model.omegaStar)););
        // Note: piS0(i), piCS0(i), piSU2Doublet0(i) are given in GeV^2,
        // so we divide by fStar*omegaStar to transform to program variables

        auto rhoMC = kin + model.pot0;

        model.aI = rPar.a0;
        model.aDotI = model.fStar / Model::MPl * sqrt(rhoMC / 3.0); // 1st Friedmann eqn

        if constexpr (Model::IsNonMinimallyCoupled) {
          auto A = 1.0 - Total(i, 0, Model::Ns - 1,
              IfElse(Model::NonMinimalCouplings::couples(i, 0_c),
                pow<2>(1.0 / Model::MPl) * model.xis(i, 0_c) * pow<2>(model.fldS0(i)), 0));
          auto B = -2.0 * Total(i, 0, Model::Ns - 1,
              IfElse(Model::NonMinimalCouplings::couples(i, 0_c),
                pow<2>(1.0 / Model::MPl) / model.omegaStar * model.xis(i, 0_c) * model.fldS0(i) * model.piS0(i), 0));
          auto C = -pow<2>(model.fStar / Model::MPl) / 3.0 * rhoMC;

          auto Delta = pow<2>(B) - 4 * A * C;
          auto H1 = (-B + sqrt(Delta)) / 2.0 / A;

          model.aI = rPar.a0;
          model.aDotI = H1;
          model.piAI = model.aDotI;
          model.RI = NonMinimalCoupling::R(model);
        }

        // Note: Initially, the gradients are 0, so the scale factors receives
        //  only contributions from the kinetic energies and the potential.
      }
    }
  };
} // namespace TempLat

#endif
