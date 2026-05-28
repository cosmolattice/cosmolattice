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
    template <class Model> FixedBackgroundExpansion(Model &model, RunParameters<T> &rPar)
    {
      H0 = rPar.H0 / model.omegaStar; // Initial Hubble parameter (in program units)
      // Coefficient of the power-law expansion: depends on EoS and alpha
      pEoS = 2.0 / (3.0 * (1.0 + rPar.omegaEoS) - 2.0 * model.alpha);
      alpha = model.alpha;
    }

    auto operator()(T deltaT) // Scale factor
    {
      return pow(1 + H0 / pEoS * deltaT, pEoS);
    }

    auto dot(T deltaT) // Scale factor time-derivative
    {
      return H0 * pow(1 + H0 / pEoS * deltaT, pEoS - 1);
    }

    auto R(T deltaT) // Ricci scalar for NMC field evolution in fixed background expansion
    {
      return -6.0 * pow<2>(H0) * pEoS * pow(1 + H0 / pEoS * deltaT, -2.0 * alpha * pEoS) * (1 + pEoS * (alpha-2)) / pow<2>(pEoS + H0 * deltaT);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    T alpha;
    T pEoS;
    T H0;
  };

} // namespace TempLat

#endif
