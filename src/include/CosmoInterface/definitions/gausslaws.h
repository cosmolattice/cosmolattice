#ifndef COSMOINTERFACE_HELPERS_GAUSSLAWS_H
#define COSMOINTERFACE_HELPERS_GAUSSLAWS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/su2algebra/su2generators.h"
#include "TempLat/lattice/algebra/su2algebra/su2wrapper.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/util/templatvector.h"
#include "CosmoInterface/definitions/mattercurrents.h"

namespace TempLat {


    /** \brief A class that checks the preservation of the Gauss constraints, for both U1 and SU2 gauge sectors.
     *
     * 
     **/

    class GaussLaws {
    public:
        /* Put public methods here. These should change very little over time. */
        GaussLaws() {

        }

        template<class Model, int N>
        
        // Checks Gauss constraint for the U(1) gauge sector:
        static inline auto checkU1(Model& model,  Tag<N> n)
        {
            auto RHS = -model.dx * MatterCurrents::U1ChargeDensity(model,n);  // right hand side (source term)
            auto LHS = Total(i, 1, Model::NDim, model.piU1(n)(i) - shift( model.piU1(n)(i),-i)  ) / model.dx; // left hand side
            
            auto diff = LHS - RHS;
            auto sum = LHS + RHS;

            auto avDiff = average(sqrt(pow<2>(diff)));
            auto avSum = average(sqrt(pow<2>(sum)));

            auto avRHS = average(sqrt(pow<2>(RHS)));
            auto avLHS = average(sqrt(pow<2>(LHS)));
            
            return make_templatvector(avDiff/avSum, avLHS, avRHS); // Returns a vector, whose 0 component is the relative degree of conservation.
        }

        // Checks Gauss constraint for the SU(2) gauge sector:
        template<class Model,int N>
        static inline auto checkSU2(Model& model,  Tag<N> n)
        {
            auto RHS  = -model.dx   * MatterCurrents::SU2ChargeDensity(model,n);   // right hand side (source term)
            auto LHS  = Total(i, 1, Model::NDim, model.piSU2(n)(i)-  shift(dagger(model.fldSU2(n)(i)) * model.piSU2(n)(i) * model.fldSU2(n)(i),-i)  ); // left hand side
            
            auto diff = LHS - RHS;
            auto sum  = LHS + RHS;

            auto avDiff = Total(i,1,3,
            				average(sqrt(pow<2>(diff(i))))); 
            auto avSum  = Total(i,1,3,
            				average(sqrt(pow<2>(sum(i))))); 
            auto avRHS  = Total(i,1,3,
            				average(sqrt(pow<2>(RHS(i))))); 
            auto avLHS  = Total(i,1, 3,
            				average(sqrt(pow<2>(LHS(i)))));

            return make_templatvector(avDiff / avSum, avLHS, avRHS);  // Returns a vector, whose 0 component is the relative degree of conservation.
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
#include "CosmoInterface/definitions/gausslaws_test.h"
#endif


#endif
