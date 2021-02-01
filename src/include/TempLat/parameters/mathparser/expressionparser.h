#ifndef TEMPLAT_PARAMETERS_MATHPARSER_EXPRESSIONPARSER_H
#define TEMPLAT_PARAMETERS_MATHPARSER_EXPRESSIONPARSER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parameters/mathparser/reducer.h"

namespace TempLat {


    /** \brief A class which get an expression from the parameter parser and convert it to a double.
     *
     * 
     * Unit test: make test-expressionparser
     **/

    class ExpressionParser {
    public:
        /* Put public methods here. These should change very little over time. */
        ExpressionParser() = delete;


        static double get(std::string name, ParameterParser& parser)
        {
            std::string tmp = parser.get<std::string>(name)();
            return Reducer::reduce(tmp, parser);
        }

        static double get(std::string name, ParameterParser& parser, double def)
        {
            std::string tmp = parser.get<std::string>(name,"")();
            if(tmp.empty()) return def;
            else return Reducer::reduce(tmp, parser);
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
#include "TempLat/parameters/mathparser/expressionparser_test.h"
#endif


#endif
