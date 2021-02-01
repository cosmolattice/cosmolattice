#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTUNARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTUNARYOPERATOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/getvectorsize.h"
#include "TempLat/lattice/algebra/helpers/getjumps.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/confirmghosts.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"

namespace TempLat {


    /** \brief A class which
     *
     * 
     * Unit test: make test-listunaryoperator
     **/

    template<typename R>
    class
    ListUnaryOperator{
    public:

        ListUnaryOperator(const R& pR):
        mR(pR)
        {
        }

        template <int N>
        ptrdiff_t confirmGhostsUpToDate(Tag<N> i)
        {
            return ConfirmGhosts::apply(mR,i);
        }

        template <int N>
        void confirmSpace(Tag<N> i, const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
            ConfirmSpace::apply(mR, i, newLayout, spaceType);
        }

        inline JumpsHolder getJumps() { //Don't need indexing for get jumps.
            return GetJumps::apply(mR);
        }
        /** For measurement objects: need the toolbox for easiest access to loopers and whatever else. */
        virtual inline
        std::shared_ptr<MemoryToolBox> getToolBox() {
            return GetToolBox::get(mR);
        }

        /** \brief Override this method in your derived class, to have an easy implementation of your toString method. */
        virtual std::string operatorString() const {
            return " ";
        }

        /** \brief If your descending class implements `operatorString()` and your operator is of the type "OP b" (where OP is * or whatever), this toString method does all the work for you, only adding parentheses if b contains spaces. */
        template<int N>
        std::string toString(Tag<N> i) const {

            std::string result = GetString::get(mR,i);

            if ( ContainsSpace::test(result) ) result = "(" + result + ")";

            return operatorString() + result;
        }

        using Getter = GetComponent;
    protected:
        R mR;
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/listunaryoperator_test.h"
#endif


#endif
