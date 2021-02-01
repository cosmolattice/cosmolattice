#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2BINARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2BINARYOPERATOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2get.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"


namespace TempLat {


    /** \brief A class which implements basic features of su2 binary operators.
     *
     * 
     * Unit test: make test-su2binaryoperator
     **/

    template<typename R, typename T>
    class SU2BinaryOperator {
    public:
        /* Put public methods here. These should change very little over time. */
        SU2BinaryOperator(const R& pR, const T& pT) : mR(pR), mT(pT) {

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

        virtual inline
        std::shared_ptr<MemoryToolBox> getToolBox() {
            auto a = GetToolBox::get(mR);
            auto b = GetToolBox::get(mT);
            return a.get() != NULL ? a : b;
        }


        static constexpr size_t size = 4;
        using Getter = SU2Getter;
    protected:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        /* Put all member variables and private methods here. These may change arbitrarily. */
        R mR;
        T mT;

    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator_test.h"
#endif


#endif
