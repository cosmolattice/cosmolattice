#ifndef TEMPLAT_UTIL_LOADBALANCE_H
#define TEMPLAT_UTIL_LOADBALANCE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include <cinttypes>

namespace TempLat {


    /** \brief A class which
     * given and a number of threads nth, return nth integer which reprensents the most even splitting of the number
     * 
     * 
     * Unit test: make test-loadbalance
     **/

    class LoadBalance {
    public:
        /* Put public methods here. These should change very little over time. */
        LoadBalance() {

        }

        inline static std::vector<ptrdiff_t > getConf(ptrdiff_t nPoints, ptrdiff_t nThreads)
        {
            auto quotRem = std::imaxdiv(nPoints, nThreads);
            std::vector<ptrdiff_t> res;
            for(ptrdiff_t i = 0; i< nThreads; ++i)
            {
                res.push_back(i < quotRem.rem ? quotRem.quot + 1 : quotRem.quot);
            }
            return res;
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
#include "TempLat/util/loadbalance_test.h"
#endif


#endif
