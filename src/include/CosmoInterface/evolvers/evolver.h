#ifndef COSMOINTERFACE_EVOLVERS_EVOLVER_H
#define COSMOINTERFACE_EVOLVERS_EVOLVER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/evolvers/leapfrog.h"
#include "CosmoInterface/evolvers/velocityverlet.h"

#include "TempLat/util/exception.h"


namespace TempLat {

    /** \brief A class which interfaces the evolver, so we don't need to use pointers in the main.
     *
     *
     **/


    MakeException(EvolverTypeNotInEvolver);

    template <class Model>
    class Evolver {
    public:
        /* Put public methods here. These should change very little over time. */
        using T = typename Model::FloatType;

        Evolver(Model& model, RunParameters<T>& rPar)  :
        type(rPar.eType) {
            if( type == LF){
                lf = std::make_shared<LeapFrog<T>>(model, rPar);
            }
            else{
                if(!(VelocityVerletParameters<T>::isVerlet(type) )) throw(EvolverTypeNotInEvolver("The evolver type you specified was not implemented in the Evolver class, which dispatch between different evolvers. Abort."));
                else vv = std::make_shared<VelocityVerlet<T>>(model, rPar);
            }
        }

        inline void evolve(Model& model, T tMinust0) const
        {
            if( type == LF){
                lf->evolve(model, tMinust0);
            }
            else{
                if(!(VelocityVerletParameters<T>::isVerlet(type) )) throw(EvolverTypeNotInEvolver("The evolver type you specified was not implemented in the Evolver class, which dispatch between different evolvers. Abort."));
                else vv->evolve(model, tMinust0);
            }
        }

        // The next function is used to synchronised all the fields to live
        // at integer time before measurements. Useful for evolvers where
        // this is not naturally the case, such as leapfrog.

        inline void sync(Model& model, T tMinust0) const
        {
            if(type == LF){
                lf->sync(model, tMinust0);
            }
            else { // The default evolvers have fields and momenta living at integer times, so no need to sync. for measurements.
                if(!(VelocityVerletParameters<T>::isVerlet(type) )) throw(EvolverTypeNotInEvolver("The evolver type you specified was not implemented in the Evolver class, which dispatch between different evolvers. Abort."));
                else vv->sync(model, tMinust0); //The sync function is used to set aDot to its correct value in the case of fixed background.
            }
        }



    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */


        std::shared_ptr<LeapFrog<T> > lf;
        std::shared_ptr<VelocityVerlet<T> > vv;

        const EvolverType type;


    };

    class EvolverTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/evolvers/evolver_test.h"
#endif


#endif
