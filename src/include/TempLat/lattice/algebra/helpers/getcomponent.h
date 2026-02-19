#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETCOMPONENT_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETCOMPONENT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"

namespace TempLat
{
  /** @brief A class which calls getComp.
   *
   * Unit test: ctest -R test-getcomponent
   **/
  class GetComponent
  { // need two is gettable cause need to know what function to use in the return types.

  public:
    template <int N, typename U>
      requires IsTempLatGettable<N, U>
    static inline auto get(U &&obj, Tag<N> t)
    {
      return obj.getComp(t);
    }

    template <int N, typename U>
      requires(IsSTDGettable<N, U> && !IsTempLatGettable<N, U>)
    static inline auto &get(U &&obj, Tag<N> t)
    {
      return std::get<N>(obj);
    }

    template <int N, typename U>
      requires(!IsSTDGettable<N, U> && !IsTempLatGettable<N, U>)
    static inline auto &get(U &&obj, Tag<N> t)
    {
      return obj;
    }
  };
} // namespace TempLat

#endif
