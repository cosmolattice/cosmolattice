#ifndef COSMOINTERFACE_DEFINITIONS_DEFECTSMODULE_FATTENING_H
#define COSMOINTERFACE_DEFINITIONS_DEFECTSMODULE_FATTENING_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
 *   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
 *   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros,  Year: 2026


#include "TempLat/util/exception.h"
#include "CosmoInterface/evolvers/evolver.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

    /** @brief A class that applies diffusion to the fields. Currently used to generate initial conditions of defect networks
     *
     *
     **/

    class Fattening
    {
    public:
        // Put public methods here. These should change very little over time.

        Fattening() = delete;

        template<typename Model, typename T>
        static void updateFatteningFactor(Model &model, T tminust0, T t0, T t0Fat, T tMaxFat, T sfat)
        {
            if(tminust0 + t0 < tMaxFat && tminust0 + t0 >= t0Fat - model.dt) {
                model.fatteningFactor *= pow(model.aI / model.aIM, 2. * (sfat - 1.));
            }
        }

        template<typename Model, typename T>
        static bool updateTimeStep(Model &model, RunParameters<T> &rPar, T t)
        {
            if (t > (rPar.tMaxFat - rPar.dt) && t < rPar.tMaxFat && !AlmostEqual(rPar.tMaxFat, t)) {
                model.dt = rPar.tMaxFat - t;
                return true;
            }
            else if ( AlmostEqual(rPar.tMaxFat, t) ) model.dt = rPar.dt - model.dt;
            else if ( !AlmostEqual(model.dt, rPar.dt) ) model.dt = rPar.dt;
            return false;
        }

    };


} // namespace TempLat

#endif
