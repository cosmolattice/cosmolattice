#ifndef COSMOINTERFACE_DEFINITIONS_FIXEDBACKGROUNDEXPANSION_H
#define COSMOINTERFACE_DEFINITIONS_FIXEDBACKGROUNDEXPANSION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2021

#include "CosmoInterface/runparameters.h"

namespace TempLat
{
  /** @brief A class which implements fixed background expansion.
   *
   *
   **/
  template <typename T> class FixedBackgroundExpansion
  {
  public:
    // Put public methods here. These should change very little over time.
    template <class Model>
    FixedBackgroundExpansion(Model &model, RunParameters<T> &rPar)
        : pEoS(2.0 / (3.0 * (1.0 + rPar.omegaEoS) -
                      2.0 * model.alpha)), // Coefficient of the power-law expansion: depends on EoS and alpha
          H0((AlmostEqual(rPar.H0, 0.0, runParameterTolerance) && Model::DefectsModel)
                 ? pEoS / rPar.t0
                 : rPar.H0 / model.omegaStar), // Initial Hubble parameter (in program units)
          alpha(model.alpha), doResolutionPreserving(rPar.doResolutionPreserving), t0(rPar.t0), tRP0(rPar.tRP0),
          tRPMax(rPar.tRPMax), sRP(rPar.sRP)
    {
      if (AlmostEqual(H0, 0.0, runParameterTolerance) && rPar.fixedBackground)
        throw(RunParametersInconsistent(
            "For models that do not involve cosmic defects, you need to specify a non-zero H0 to run a simulation with "
            "fixed background. If you want to disable expansion, please set expansion = false in the input file."));
    }

    auto operator()(T deltaT) // Scale factor
    {
      return pow(1 + H0 / pEoS * deltaT, pEoS);
    }

    auto dot(T deltaT) // Scale factor time-derivative
    {
      return H0 * pow(1 + H0 / pEoS * deltaT, pEoS - 1);
    }

    auto const areWeResolutionPreserving() const { return doResolutionPreserving; }

    auto R(T deltaT) // Ricci scalar for NMC field evolution in fixed background expansion
    {
      return -6.0 * pow<2>(H0) * pEoS * pow(1 + H0 / pEoS * deltaT, -2.0 * alpha * pEoS) * (1 + pEoS * (alpha - 2)) /
             pow<2>(pEoS + H0 * deltaT);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    T pEoS;
    T H0;
    T alpha;
    bool doResolutionPreserving;

  public:
    const T t0, tRP0, tRPMax, sRP;
  };

} // namespace TempLat

#endif
