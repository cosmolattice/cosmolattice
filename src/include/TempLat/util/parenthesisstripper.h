#ifndef TEMPLAT_UTIL_PARENTHESISSTRIPPER_H
#define TEMPLAT_UTIL_PARENTHESISSTRIPPER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"

namespace TempLat {



    /** \brief A class which strips a string of its parenthesis if they are first and last characters.
     *
     * 
     * Unit test: make test-parenthesisstripper
     **/

    class ParenthesisStripper {
    public:
        /* Put public methods here. These should change very little over time. */
        ParenthesisStripper() = delete;

        static void strip(std::string& toStrip)
        {
            if(
                    (toStrip.front() == '(' && findClosing(toStrip,'(',')')== toStrip.size() -1 )
             or
                    (toStrip.front() == '[' && findClosing(toStrip,'[',']')== toStrip.size() -1)
             or
                    (toStrip.front() == '{' && findClosing(toStrip,'{','}')== toStrip.size() -1)
             ) toStrip = toStrip.substr(1, toStrip.size() - 2);
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        static size_t findClosing(std::string str, char parOpen, char parClose)
        {
            size_t countPar= 1;
            size_t count = 0;
            while(countPar>0)
            {
                count++;
                if(str[count] == parOpen) countPar++;
                if(str[count] == parClose) countPar--;
            }
            return count;
        }



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/parenthesisstripper_test.h"
#endif


#endif
