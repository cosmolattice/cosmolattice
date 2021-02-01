#ifndef TEMPLAT_PARAMETERS_MATHPARSER_SPLITTER_H
#define TEMPLAT_PARAMETERS_MATHPARSER_SPLITTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which splits things with parenthesis. Inspired from https://medium.com/@stoopidguy1992/how-to-write-a-math-expression-parser-in-javascript-b5147bc9466b
     *
     * 
     * Unit test: make test-splitter
     **/

    class Splitter {
    public:
        /* Put public methods here. These should change very little over time. */
        Splitter() = delete;

        static std::vector<std::string> split(std::string str, char op, char parOpen = '(')
        {
            std::vector<std::string> ret;
            char parClose = ' ';
            if(parOpen == '(') parClose = ')';
            if(parOpen == '[') parClose = ']';
            if(parOpen == '{') parClose = '}';

            int count = 0;
            std::string tmp = "";
            for(char x : str){
                if(x == parOpen) ++count;
                if(x == parClose) --count;
                if(count == 0 && x == op) {
                    ret.emplace_back(tmp);
                    tmp = "";
                }
                else tmp.push_back(x);
            }
            if(!tmp.empty()) ret.emplace_back(tmp);
            return ret;
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
#include "TempLat/parameters/mathparser/splitter_test.h"
#endif


#endif
