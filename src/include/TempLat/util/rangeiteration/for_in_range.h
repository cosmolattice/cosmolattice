#ifndef TEMPLAT_UTIL_RANGEITERATION_FOR_IN_RANGE_H
#define TEMPLAT_UTIL_RANGEITERATION_FOR_IN_RANGE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/foreach.h"
#include "TempLat/util/rangeiteration/taglist.h"
#include "TempLat/util/staticif.h"


namespace TempLat {


    /** \brief A class which implements a static for loop.
     *
     *
     * Unit test: make test-for_in_range
     **/

    class for_in_range_Tester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<int i, int j>
    struct ForRangeHelper{

        template<typename F>
        static void run(F&& f)
        {
            TagList<i,j-1> tg;
            for_each(tg.tags,f);
        }
    };
    template<int i>
    struct ForRangeHelper<i,0>{

        template<typename F>
        static void run(F&& f)
        {
        }
    };

    template<int i, int j, typename F>
    void for_in_range(F&& f)
    {
      //TagList<i,j-1> tg;
      //for_each(tg.tags,f);
      ForRangeHelper<i,j>::run(f);
    }



#define ForLoop(i, start, end, expr) for_in_range<start, end+1>([&](auto i){ expr; })



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/rangeiteration/for_in_range_test.h"
#endif


#endif
