#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETNDIM_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETNDIM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"

namespace TempLat
{
  /** \brief A class which returns the number of dimensions.
   *
   * Unit test: make test-getndim
   **/
  class GetNDim
  {
    template <typename T> struct always_false {
      enum { value = false };
    };

  public:
    template <typename U>
      requires requires {
        { U::getNDim() } -> std::convertible_to<size_t>;
      }
    static consteval size_t get()
    {
      return U::getNDim();
    }

    template <typename U>
      requires(
          requires { typename U::NDim; } &&
          !requires {
            { U::getNDim() } -> std::convertible_to<size_t>;
          })
    static consteval size_t get()
    {
      return U::NDim;
    }

    template <typename U>
      requires(
          !requires { typename U::NDim; } &&
          !requires {
            { U::getNDim() } -> std::convertible_to<size_t>;
          })
    static consteval size_t get()
    {
      return 0;
    }
  };

  template <typename T>
  concept HasNDim = requires {
    { GetNDim::get<T>() } -> std::convertible_to<size_t>;
  };

  struct GetNDimTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
