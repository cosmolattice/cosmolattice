#ifndef COSMOINTERFACE_HELPERS_MATTERCURRENTS_H
#define COSMOINTERFACE_HELPERS_MATTERCURRENTS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/definitions/gaugederivatives.h"

namespace TempLat {


    /** \brief A class which computes the matter currents and charge densities of the gauge fields
     *
     * 
     * Unit test: make test-mattercurrents
     **/

    class MatterCurrents {
    public:
        /* Put public methods here. These should change very little over time. */
        MatterCurrents() {

        }
        
        ////// U(1) CHARGE DENSITY (used in the Gauss constraint) //////
        
        template<class Model, int N>
        static auto U1ChargeDensity(Model& model, Tag<N> n) { //N is the Nth U1 abelian field, i is the spatial component.

            auto norm = 2.0 / model.dx * pow<2>((model.fStar / model.omegaStar));

            auto U1Part =  Total(a,0,Model::NCs-1,//sum over U(1) matter field
                                 IfElse(Model::CsU1Couplings::couples(a,Tag<N>()), //check if they couple to this specific U1
                                        norm * model.gQ_CsU1(a,n) * Imag(dagger(model.piCS(a)) * model.fldCS(a))
                                         ,ZeroType()));

            auto SU2Part = Total(a,0,Model::NSU2Doublet-1,//sum over SU(2) matter field
                                 IfElse(Model::SU2DoubletU1Couplings::couples(a,Tag<N>()), //check if they couple to this specific U1
                                        norm * model.gQ_SU2DblU1(a,n) * Imag(scalar_prod(
                                                model.piSU2Doublet(a),
                                                model.fldSU2Doublet(a))) //Compute the current, taking into account all the contributions to the cov derivs.
                                         ,ZeroType()));


            return  (U1Part + SU2Part);
        }  
        
        ////// U(1) GAUGE CURRENT (used in the kernels of the EOM) //////

        template<class Model, int N, int I>
        static auto U1Current(Model& model, Tag<N>, Tag<I> i) { //N is the Nth U1 abelian field, i is the spatial component.

            auto U1Part =  Total(a,0,Model::NCs-1,//sum over U(1) matter field
                                 IfElse(Model::CsU1Couplings::couples(a,Tag<N>()), //check if they couple to this specific U1
                                        model.gQ_CsU1(a,Tag<N>()) * Imag(dagger(GaugeDerivatives::U1sForCSCovDerivs(model,a,i) * shift<I>(model.fldCS(a))) * model.fldCS(a))
                                         ,ZeroType()));

            auto SU2Part = Total(a,0,Model::NSU2Doublet-1,//sum over SU(2) matter field
                                 IfElse(Model::SU2DoubletU1Couplings::couples(a,Tag<N>()), //check if they couple to this specific U1
                                        model.gQ_SU2DblU1(a,Tag<N>()) * Imag(scalar_prod(
                                                GaugeDerivatives::U1sForSU2DoubletCovDerivs(model,a,i) * (GaugeDerivatives::SU2sForSU2DoubletCovDerivs(model,a,i) *
                                                                                        shift<I>(model.fldSU2Doublet(a))),
                                                model.fldSU2Doublet(a))) //Compute the current, taking into account all the contributions to the cov derivs.
                                         ,ZeroType()));

            return 2.0 / model.dx * pow<2>((model.fStar / model.omegaStar))  * (U1Part + SU2Part);
        }

        template<class Model, int N>  // creates a 3-component vector containing the U(1) current
        static auto U1Current(Model& model, Tag<N> t)
        {
            return MakeVector(i,1,Model::NDim,U1Current(model,t,i));
        }
        
        ////// SU(2) CHARGE DENSITY (used in the Gauss constraint) //////
        
        template<class Model, int N>
        static auto SU2ChargeDensity(Model& model, Tag<N>) { // N is the gaugefield number.

            auto SU2Part = Total(a,0,Model::NSU2Doublet-1,//sum over SU(2) matter field
                                 IfElse(Model::SU2DoubletSU2Couplings::couples(a,Tag<N>()), //check if they couple to this specific SU2
                                        model.gQ_SU2DblSU2(a,Tag<N>()) * MakeSU2(b,
                                               Real(scalar_prod(Constants::i_sigma(b) * model.fldSU2Doublet(a), model.piSU2Doublet(a))))
                                         ,ZeroType()));

            return   pow<2>(model.fStar / model.omegaStar) / 2.0 * SU2Part;
        }
            
        ////// SU(2) GAUGE CURRENT (used in the kernels of the EOM) //////

        template<class Model, int N, int I>
        static auto SU2Current(Model& model, Tag<N>, Tag<I> i) {

            auto SU2Part = Total(a,0,Model::NSU2Doublet-1,//sum over SU(2) matter field
                                 IfElse(Model::SU2DoubletSU2Couplings::couples(a,Tag<N>()), //check if they couple to this specific SU2
                                        model.gQ_SU2DblSU2(a,Tag<N>()) *
                                                MakeSU2(b,Real(scalar_prod(GaugeDerivatives::U1sForSU2DoubletCovDerivs(model,a,i)  * (GaugeDerivatives::SU2sForSU2DoubletCovDerivs(model,a,i) * shift(model.fldSU2Doublet(a),i)),
                                                                                                          Constants::i_sigma(b) * model.fldSU2Doublet(a)))) //Compute the current, taking into account all the contributions to the cov derivs.
                                         ,ZeroType()));

            return   pow<2>( (model.fStar / model.omegaStar)) / 2.0 * SU2Part;
        }

        template<class Model, int N>    // creates a 3-component vector containing the SU(2) current
        static auto SU2Current(Model& model, Tag<N> t)
        {
            return MakeVector(i,1,Model::NDim,SU2Current(model,t,i));
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
#include "CosmoInterface/definitions/mattercurrents_test.h"
#endif


#endif
