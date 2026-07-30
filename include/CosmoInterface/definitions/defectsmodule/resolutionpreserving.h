#ifndef COSMOINTERFACE_DEFINITIONS_DEFECTSMODULE_RESOLUTIONPRESERVING_H
#define COSMOINTERFACE_DEFINITIONS_DEFECTSMODULE_RESOLUTIONPRESERVING_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
 *   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
 *   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros,  Year: 2026

#include "TempLat/util/exception.h"
#include "CosmoInterface/evolvers/evolver.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

  /** @brief A class that implements the resoluton-preserving techniques (including fattening and extra-fattening).
   *
   *
   **/

  class ResolutionPreserving
  {
  public:
    // Put public methods here. These should change very little over time.

    ResolutionPreserving() = delete;

    template <typename Model, typename T>
    static void updateResolutionPreservingFactor(Model &model, T tminust0, T t0, T tRP0, T tRPMax, T sRP)
    {
      if (tminust0 + t0 < tRPMax && tminust0 + t0 >= tRP0 - model.dt) {
        model.resolutionPreservingFactor *= pow(model.aI / model.aIM, 2. * (sRP - 1.));
      }
    }

    template <typename Model, typename T> static bool updateTimeStep(Model &model, RunParameters<T> &rPar, T t)
    {
      if (t > (rPar.tRPMax - rPar.dt) && t < rPar.tRPMax && !AlmostEqual(rPar.tRPMax, t, runParameterTolerance) &&
          !AlmostEqual(rPar.tRPMax, t + rPar.dt, runParameterTolerance)) {
        model.dt = rPar.tRPMax - t;
        return true;
      } else if (AlmostEqual(rPar.tRPMax, t, runParameterTolerance) && !AlmostEqual(model.dt, rPar.dt, runParameterTolerance))
        model.dt = rPar.dt - model.dt;
      else if (!AlmostEqual(model.dt, rPar.dt, runParameterTolerance))
        model.dt = rPar.dt;
      return false;
    }
  };

} // namespace TempLat

#endif
