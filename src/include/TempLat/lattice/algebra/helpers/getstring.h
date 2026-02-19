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
#include <type_traits>

#include "TempLat/util/floattostring.h"
#include "TempLat/lattice/algebra/helpers/hasstringmethod.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"

namespace TempLat
{
  template <typename U>
  concept CanBeConvertedToString = requires(U &&obj) { std::to_string(obj); };

  template <typename U>
  concept HasNoStringConverter = !CanBeConvertedToString<U> && !HasStringMethod<U> && !IsComplexType<U>;

  /** @brief A template-programming class which helps to get the 'toString'-value from any type, whether it has the
   *toString-method or it is a scalar value.
   *
   * Unit test: ctest -R test-getstring
   **/
  class GetString
  {
  public:
    // Put public methods here. These should change very little over time.

    template <typename U>
      requires HasStringMethod<U>
    static inline std::string get(const U &obj)
    {
      return obj.toString();
    }

    template <typename U>
      requires IsComplexType<U>
    static inline std::string get(const U &obj)
    {
      return "(" + GetString::get(obj.real()) + " + " + GetString::get(obj.imag()) + "i)";
    }

    template <typename U>
      requires(CanBeConvertedToString<U> && !std::is_floating_point_v<U> && !HasStringMethod<U>)
    static std::string get(const U &obj)
    {
      return std::to_string(obj);
    }

    template <typename U>
      requires HasNoStringConverter<U>
    static inline std::string get(const U &obj)
    {
      // if everything fails, return the typeid
      return std::string(typeid(obj).name());
    }

    template <typename U>
      requires std::is_floating_point_v<U>
    static inline std::string get(const U &val)
    {
      return FloatToString::format(val);
    }

    template <int N, typename U>
      requires HasStringMethodIndexed<N, U>
    static inline std::string get(const U &obj, Tag<N> i)
    {
      return obj.toString(i);
    }

    template <int N, typename U>
      requires(!HasStringMethodIndexed<N, U>)
    static inline std::string get(const U &obj, Tag<N> i)
    {
      return get(obj);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    GetString() {}
  };

  /** @brief Enable simple operator<< for all objects with a toString method.
   */
  template <typename T>
    requires HasStringMethod<T>
  typename std::ostream &operator<<(std::ostream &ostream, const T &obj)
  {
    ostream << obj.toString();
    return ostream;
  }
} // namespace TempLat

#endif
