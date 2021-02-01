#ifndef TEMPLAT_UTIL_CSTYLETIME_H
#define TEMPLAT_UTIL_CSTYLETIME_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include <ctime>

namespace TempLat {


    /** \brief A class which returns date and time, pre C++17.
     *
     * 
     * Unit test: make test-cstyletime
     **/

    class CStyleTime {
    public:
        /* Put public methods here. These should change very little over time. */
        CStyleTime() { //available in c++ since c++20

        }

        void now()
        {
            time_t now = std::time(0);
            tm *ltm = localtime(&now);
            // afficher divers member de la structure tm.
            year = 1900 + ltm->tm_year;
            month  = 1 + ltm->tm_mon;
            day = ltm->tm_mday;
            hour =  1 + ltm->tm_hour;
            minute = (1 + ltm->tm_min)%60;
            second = 1 + ltm->tm_sec;
        }

        std::string date(std::string sep = "_")
        {
            return "d"+std::to_string(day)+sep+"m"+std::to_string(month)+sep+"y"+std::to_string(year);
        }

        std::string time(std::string sep = "_")
        {
            return "h"+std::to_string(hour)+sep+"m"+std::to_string(minute)+sep+"s"+std::to_string(second);
        }

        std::string approxTime(std::string sep = "_")
        {
            return "h"+std::to_string(hour)+sep+"m"+std::to_string(minute);
        }



    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        int day,month,year,second,minute,hour;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/cstyletime_test.h"
#endif


#endif
