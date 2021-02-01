#ifndef COSMOINTERFACE_INITIALIZERS_SCALARINITIALIZER_H
#define COSMOINTERFACE_INITIALIZERS_SCALARINITIALIZER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/rangeiteration/for_in_range.h"
#include "CosmoInterface/initializers/fluctuationsgenerator.h"

namespace TempLat {

    /** \brief A class which is used to initialize the scalar singlets.
     *
     **/

    class ScalarSingletInitializer {
    public:
        /* Put public methods here. These should change very little over time. */
        ScalarSingletInitializer() = delete;

        template<class Model, typename T>
        static void initializeScalars(Model& model, const FluctuationsGenerator<T>& fg, T kCutOff)
        {

            // We set fluctuations to the scalar singlets:
            ForLoop(i,0,Model::Ns-1,
            	fg.conjugateGaussianFluctuations(model, model.fldS(i), model.piS(i), model.masses2S[i], model.aDotI, kCutOff);
            );

            // We set the initial homogeneous components of the fields and derivatives.
            // model.fldCS0(i) and model.piCS0(i) are introduced in physical
            // (dimensionful variables), so we transform them to program variables
            // by dividing them by f_* and f_* omega_* respectively.
            model.fldS += model.fldS0 / model.fStar;
            model.piS += model.piS0 / model.fStar / model.omegaStar ;
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/initializers/scalarsingletinitializer_test.h"
#endif


#endif
