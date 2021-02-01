#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETUNARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETUNARYOPERATOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletget.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"


namespace TempLat {


    /** \brief A class which groups common features to su2doublets unary operators.
     *
     * 
     * Unit test: make test-su2doubletunaryoperator
     **/


    template <typename R>
    class SU2DoubletUnaryOperator {
    public:
        /* Put public methods here. These should change very little over time. */
        SU2DoubletUnaryOperator(const R& pR) : mR(pR){

        }


        /** \brief Override this method in your derived class, to have an easy implementation of your toString method. */
        virtual std::string operatorString() const {
            return " ";
        }


        /** \brief If your descending class implements `operatorString()` and your operator is of the type "OP b" (where OP is * or whatever), this toString method does all the work for you, only adding parentheses if b contains spaces. */
        virtual std::string toString() const {

            std::string result = GetString::get(mR);

            if ( ContainsSpace::test(result) ) result = "(" + result + ")";

            return operatorString() + result;
        }


        inline auto getDx() const
        {
            return GetDx::getDx(mR);
        }

        inline auto getKIR() const
        {
            return GetKIR::getKIR(mR);
        }

        static constexpr size_t size = 4;
        using Getter = SU2DoubletGetter;


    protected:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        R mR;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/su2algebra/su2doubletunaryoperator_test.h"
#endif


#endif
