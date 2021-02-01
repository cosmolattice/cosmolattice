#ifndef TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_COMPLEXFIELDBINARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_COMPLEXFIELDBINARYOPERATOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"

namespace TempLat {


    /** \brief A class which gtoups common features of binary field operators.
     *
     * 
     * Unit test: make test-complexfieldbinaryoperator
     **/

    template <typename R, typename T>
    class ComplexFieldBinaryOperator {
    public:
        /* Put public methods here. These should change very little over time. */
        ComplexFieldBinaryOperator(const R& pR, const T& pT) : mR(pR), mT(pT) {

        }


        /** \brief Override this method in your derived class, to have an easy implementation of your toString method. */
        virtual std::string operatorString() const {
            return " ";
        }

        /** \brief If your descending class implements `operatorString()` and your operator is of the type "a OP b" (where OP is * or whatever), this toString method does all the work for you. */
        std::string toString() const {

            std::string tt = GetString::get(mR);

            if ( ContainsSpace::test(tt) ) tt = "(" + tt + ")";

            std::string ss = GetString::get(mT);

            if ( ContainsSpace::test(ss) ) ss = "(" + ss + ")";

            std::string result = tt + " " + operatorString() + " " + ss;

            return result;
        }

        inline auto getDx() const
        {
            return GetDx::getDx(mR);
        }

        inline auto getKIR() const
        {
            return GetKIR::getKIR(mR);
        }

        static constexpr size_t size = 2;
        using Getter = ComplexFieldGetter;

    protected:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        R mR;
        T mT;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator_test.h"
#endif


#endif
