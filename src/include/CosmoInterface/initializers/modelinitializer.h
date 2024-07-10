#ifndef COSMOINTERFACE_INITIALIZERS_MODELINITIALIZER_H
#define COSMOINTERFACE_INITIALIZERS_MODELINITIALIZER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/initializers/fluctuationsgenerator.h"
#include "CosmoInterface/initializers/scalefactorinitializer.h"
#include "CosmoInterface/initializers/scalarsingletinitializer.h"
#include "CosmoInterface/initializers/gwsinitializer.h"
#include "CosmoInterface/initializers/u1initializer.h"
#include "CosmoInterface/initializers/su2initializer.h"
#include "CosmoInterface/definitions/averages.h"

namespace TempLat {

    /** \brief A class which is used to initialize the model.
     *
     **/

    template <typename T>
    class ModelInitializer {
    public:
        /* Put public methods here. These should change very little over time. */
        template<class Model>
        ModelInitializer(Model& model, T pLSide, std::string pSeed) :
        fg(model, pLSide, pSeed)
        {
        }

        template<class Model>
        void initialize(Model& model, RunParameters<T>& rPar)
        {
            // Initialize scale factor:
            if(rPar.expansion) ScaleFactorInitializer::initializeScaleFactor(model, rPar);
            
            // Initialize scalar singlets:
            if(Model::Ns > 0) ScalarSingletInitializer::initializeScalars(model, fg, rPar.kCutoff);
            
            // Initialize GWs:
            if (model.fldGWs != nullptr) GWsInitializer::initializeGWs(model);
            
            // Initialize the SU2 gauge fields and SU2 doublets:
            //  --> Note: It is important to initialize SU2 first, as the doublet contributes to the U1 currents.
            if(Model::NSU2Doublet > 0) SU2Initializer::initializeSU2(model, fg, rPar.kCutoff);
            
            // Initialize the U1 gauge fields and complex scalars:
            if(Model::NCs > 0 or (Model::NSU2Doublet > 0 && Model::NU1 > 0 ) ) U1Initializer::initializeU1(model, fg, rPar.kCutoff);

            Averages::setAllAverages(model);
            if(rPar.expansion && !rPar.fixedBackground) {
                // For consistency, correct the scale factor time-derivative with the fluctuations.
                // Relevant only for higher order evolvers.
                auto hubbleLaw =  HubbleConstraint::get(model);
                model.aDotI = sqrt(hubbleLaw[2]);
            }
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        FluctuationsGenerator<T> fg;
    };

    struct ModelInitializerTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/initializers/modelinitializer_test.h"
#endif


#endif
