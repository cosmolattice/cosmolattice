#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVALUE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVALUE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/hasgetmethod.h"
#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/util/tdd/tdd.h"
#include <type_traits>

namespace TempLat
{
  template <typename U>
  concept TypeHasGetMethod = requires(U obj, ptrdiff_t i) { obj.get(i); };

  template <typename U>
  concept TypeHasNoGetMethod = !TypeHasGetMethod<U>;

  template <typename U>
  concept TypeGetsItself = (std::is_floating_point_v<std::decay_t<U>> || std::is_integral_v<std::decay_t<U>>);

  // template <typename T>
  // concept TypeGetsItself<complex<T>> = std::is_floating_point<T>;

  /** \brief A template-programming class which helps to get the 'get'-value from any type,
   * whether it has the get-method or it is a scalar value.
   * Now you can use an int as if it had a get method for example.
   *
   * Unit test: make test-getvalue
   **/
  class GetValue
  {
  public:
    /* Put public methods here. These should change very little over time. */

    template <typename U>
      requires TypeHasGetMethod<U>
    static KOKKOS_FORCEINLINE_FUNCTION auto get(U &&obj, ptrdiff_t i)
    {
      return obj.get(i);
    }

    template <typename U>
      requires TypeGetsItself<U>
    static KOKKOS_FORCEINLINE_FUNCTION auto get(U &&obj, ptrdiff_t i)
    {
      return obj;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    GetValue() = delete;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getvalue_test.h"
#endif

#endif
