#ifndef COSMOINTERFACE_DEFINITIONS_FIXEDBACKGROUNDEXPANSION_H
#define COSMOINTERFACE_DEFINITIONS_FIXEDBACKGROUNDEXPANSION_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2021

#include "CosmoInterface/runparameters.h"

namespace TempLat {


    /** \brief A class which implements fixed background expansion.
     *
     * 
     **/

    template<typename T>
    class FixedBackgroundExpansion {
    public:
        /* Put public methods here. These should change very little over time. */
        template<class Model>
        FixedBackgroundExpansion(Model& model, RunParameters<T>& rPar)
        {
            H0 = rPar.H0 / model.omegaStar; // Initial Hubble parameter (in program units)
            
            // Coefficient of the power-law expansion: depends on EoS and alpha
            pEoS = 2.0 / (3.0 * (1.0 + rPar.omegaEoS) - 2.0 * model.alpha);
        }

        auto operator()( T deltaT) // Scale factor
        {
            return pow(1 + H0 / pEoS * deltaT, pEoS);
        }

        auto dot( T deltaT)  // Scale factor time-derivative
        {
            return H0 * pow(1 + H0 / pEoS * deltaT, pEoS- 1);
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        T pEoS;
        T H0;


    };

    class FixedBackgroundExpansionTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/definitions/fixedbackgroundexpansion_test.h"
#endif


#endif
