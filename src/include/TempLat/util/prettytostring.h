#ifndef TEMPLAT_UTIL_PRETTYTOSTRING_H
#define TEMPLAT_UTIL_PRETTYTOSTRING_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which attempts to write a float removing the unnnecessary 0 at the end. Not completely robust
     * as this is a complicated problem but good enough for our purposes.
     * 
     * Unit test: make test-prettytostring
     **/

    class PrettyToString {
    public:
        /* Put public methods here. These should change very little over time. */
        PrettyToString() {

        }

        template<typename R>
        static std::string get(R x, int prec=14)
        {

            std::ostringstream out;
            out.precision(prec);
            out << std::fixed << x;
            std::string res(out.str());
            removeZeros(res);
            return res;
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */


        static void removeZeros(std::string& x)
        {
            while(x.back() == '0') x.pop_back();
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/prettytostring_test.h"
#endif


#endif
