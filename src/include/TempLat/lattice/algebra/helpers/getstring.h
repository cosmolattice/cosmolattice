#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETSTRING_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETSTRING_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/floattostring.h"
#include "TempLat/lattice/algebra/helpers/hasstringmethod.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"

namespace TempLat {

    /** \brief A template-programming class which helps to get the 'toString'-value from any type, whether it has the toString-method or it is a scalar value.
     *
     * Unit test: make test-getstring
     **/

    class GetString {
    public:
        /* Put public methods here. These should change very little over time. */

        template <typename U>
        static inline
        typename std::enable_if<HasStringMethod<U>::value, std::string>::type
        get( const U& obj) {
            return obj.toString();
        }


        template <typename U>
        static inline
        typename std::enable_if<IsComplexType<U>::value, std::string>::type
        get( const U& obj) {
            return "(" + GetString::get(obj.real()) + " + " + GetString::get(obj.imag()) + "i)";
        }

        //        template <typename U>
        //        static inline
        //        get( std::complex<U>& obj) {
        //            return std::to_string(obj);
        //        }

        template <typename U>
        static inline
        typename std::enable_if<!HasStringMethod<U>::value && !IsComplexType<U>::value && !std::is_floating_point<U>::value, std::string>::type
        get( const U& obj) {
            return std::to_string(obj);
        }

        template <typename U>
        static inline
        typename std::enable_if<std::is_floating_point<U>::value, std::string>::type
        get (const U& val) {
            return FloatToString::format(val);
        }



      template <int N, typename U>
      static inline
      typename std::enable_if<HasStringMethodIndexed<N, U>::value, std::string>::type
      get( const U& obj, Tag<N> i) {
          return obj.toString(i);
      }

      template <int N, typename U>
      static inline
      typename std::enable_if<!HasStringMethodIndexed<N,U>::value, std::string>::type
      get( const U& obj, Tag<N> i) {
          return get(obj);
      }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        GetString() {

        }



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    /** \brief Enable simple operator<< for all objects with a toString method.
     */
    template <typename T>
    typename std::enable_if<HasStringMethod<T>::value, std::ostream>::type&
    operator<< ( std::ostream &ostream, const T& obj) {
        ostream << obj.toString();
        return ostream;
    }



}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getstring_test.h"
#endif


#endif
