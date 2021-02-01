#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENT_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/helpers/confirmghosts.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"
#include "TempLat/lattice/algebra/helpers/getjumps.h"
#include "TempLat/lattice/algebra/helpers/getvectorvalue.h"

namespace TempLat {


    /** \brief A class which returns one component of a vector as an object, keeping all fourier and co mechanics consistent. Mostly useful for wave numbers.
     *
     *
     * Unit test: make test-getvectorcomponent
     **/

    template<typename R>
    class GetVectorComponentHelper {
    public:
        /* Put public methods here. These should change very little over time. */
        GetVectorComponentHelper(const R& pR, ptrdiff_t pJ):
        mR(pR),
        mJ(pJ)
        {

        }
        auto get(ptrdiff_t i)
        {
          return GetVectorValue::vectorGet(mR, i, mJ);
        }

        void doWeNeedGhosts()
        {
          GhostsHunter::apply(mR,mJ);
        }

        void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType)
        {
          ConfirmSpace::apply(mR,mJ,newLayout,spaceType);
        }

        ptrdiff_t confirmGhostsUpToDate()
        {
          return ConfirmGhosts::apply(mR, mJ);
        }

        inline JumpsHolder getJumps() { //Just take jumps from the first component
            return GetJumps::apply(mR);
        }

        virtual inline
        std::shared_ptr<MemoryToolBox> getToolBox() { //just take toolboc from first component
            return GetToolBox::get(mR);
        }


        std::string toString() const {
          return mR.toString(mJ);
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        R mR;
        const int mJ;

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif


    };


    template<typename R>
    GetVectorComponentHelper<R> getVectorComponent(const R& pR, ptrdiff_t i)
    {
      return GetVectorComponentHelper<R>(pR, i);
    }

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getvectorcomponent_test.h"
#endif


#endif
