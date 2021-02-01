#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETBINARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETBINARYOPERATOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletget.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2get.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"


namespace TempLat {


    /** \brief A class which implements common features to SU2Doublet binary operators.
     *
     * 
     * Unit test: make test-su2doubletbinaryoperator
     **/

    template<typename  R, typename T>
    class SU2DoubletBinaryOperator {
    public:
        /* Put public methods here. These should change very little over time. */
        SU2DoubletBinaryOperator(const R& pR, const T& pT): mR(pR),mT(pT) {

        }
        static constexpr size_t size = 4;
        using Getter = SU2DoubletGetter;

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
#include "TempLat/lattice/algebra/su2algebra/su2doubletbinaryoperator_test.h"
#endif


#endif
