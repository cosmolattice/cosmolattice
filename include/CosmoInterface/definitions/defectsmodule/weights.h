#ifndef COSMOINTERFACE_DEFINITIONS_DEFECTSMODULE_WEIGHTS_H
#define COSMOINTERFACE_DEFINITIONS_DEFECTSMODULE_WEIGHTS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
 *   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
 *   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros,  Year: 2026


#include "TempLat/util/exception.h"
#include "CosmoInterface/evolvers/evolver.h"

namespace TempLat
{

    /** @brief A class that defines the weights used to measure the energy contained in the strings. Vy defect, we use weights based on the potential, normalized such that they are equal to unity at the core of the defects.
     *
     *
     **/

    class Weights
    {
    public:
        // Put public methods here. These should change very little over time.

        Weights() = delete;

        template<typename Model>
        static auto weightScalarSinglet(Model &model)
        {
            return 4 * Potential::potential(model) *  heaviside(1 - sqrt(Total(i, 0_c, Model::Ns - 1, pow<2>(model.fldS(i)); )) ) / model.resolutionPreservingFactor;
        }

        template<typename Model>
        static auto weightComplexScalar(Model &model)
        {
            return 4 * Potential::potential(model) * heaviside(0.5 - norm2(model.fldCS(0_c))) / model.resolutionPreservingFactor;
        }

    };


} // namespace TempLat

#endif
