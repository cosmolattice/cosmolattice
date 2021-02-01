#ifndef TEMPLAT_PARAMETERS_PAIRMAKER_H
#define TEMPLAT_PARAMETERS_PAIRMAKER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include <ostream>

namespace TempLat {

    /** \brief Small class which splits strings into two and make a pair out of it.
     *
     *
     * Unit test: make test-pairmaker
     **/

    class PairMaker {
    public:
        /* Put public methods here. These should change very little over time. */
        PairMaker(const char& sep='=',const char& com='#') :
        separator(sep),
        comment(com)
        {}

        std::string getKey(){
          return key;
        }
        std::string getValue(){
          return value;
        }

        void operator()(std::string line){
            size_t pos = line.find_first_of(separator);
            size_t posCom = line.find_first_of(comment);
            line = line.substr(0,posCom);
            key = line.substr(0,pos);
            value = line.substr(pos+1);
            cleanSpaces(key);
            cleanSpaces(value);
        }
        friend std::ostream& operator<<(std::ostream& os, const PairMaker& pm)
        {
          os << pm.key << pm.separator << pm.value << std::endl;
          return os;
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::string key;
        std::string value;
        const char separator;
        const char comment;

        void cleanSpaces(std::string& str) //Remove space at begining and end of string.
        {
		      while(str.size() > 0 && str[str.size()-1] == ' ') str.pop_back();
		      while(str.size() > 0 && str[0] == ' ') str.erase(0, 1);
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parameters/pairmaker_test.h"
#endif


#endif
