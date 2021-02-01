#ifndef TEMPLAT_PARAMETERS_MULTIPLEPARAMETERGETTER_H
#define TEMPLAT_PARAMETERS_MULTIPLEPARAMETERGETTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parameters/parametergetter.h"
#include "TempLat/util/rangeiteration/tag.h"
#include <sstream>

namespace TempLat {

    /** \brief A class which returns multiple parameters.
     *
     *
     * Unit test: make test-multipleparametergetter
     **/

    template <typename T>
    class MultipleParameterGetter {
    public:
        /* Put public methods here. These should change very little over time. */
        MultipleParameterGetter() {

        }

        void push_back(const ParameterGetter<T>& p){
          params.push_back(p);
        }
        void clear()
        {
          params.clear();
        }
        size_t size()
        {
          return params.size();
        }
        ParameterGetter<T> operator[](int i){
          return params[i];
        }
        const T& operator()(int i){
          return params[i]();
        }

        template<int Q>
        auto getComp(Tag<Q> t)
        {
            return params[Q]();
        }

        std::string toString() const
        {
          return params[0].toString();
        }

        friend std::ostream& operator<<(std::ostream& os, const MultipleParameterGetter<T>& mp)
        {
          os << mp.toString() << "(";

          std::stringstream stmp;

          for(auto p : mp.params)
          {
            stmp<<p()<<",";
          }

          std::string tmp = stmp.str();

          tmp.pop_back();

          os << tmp << ")";

          return os;
        }

        operator std::vector<T>()
        {
          std::vector<T> res;
          for(size_t i = 0; i < params.size(); ++i)
          {
            res.push_back(params[i]());
          }
          return res;
        }

        template <int N>
        operator std::array<T,N>()
        {
            std::array<T,N> res;
            for(size_t i = 0; i < params.size(); ++i)
            {
                res[i] = params[i]();
            }
            return res;
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        std::vector<ParameterGetter<T> > params;
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parameters/multipleparametergetter_test.h"
#endif


#endif
