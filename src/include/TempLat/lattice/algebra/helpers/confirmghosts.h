#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_CONFIRMGHOSTS_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_CONFIRMGHOSTS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Satter,  Year: 2026

#include "TempLat/lattice/algebra/helpers/hasghostmethod.h"

namespace TempLat
{
  /** @brief A class which wraps the calls to confirm(Configuration/Fourier)Space, for classes that have it and objects
   *that don't.
   *
   * Unit test: ctest -R test-confirmspace
   **/
  class ConfirmGhosts
  {
  public:
    // Put public methods here. These should change very little over time.
    template <typename U>
      requires HasGhostMethod<U>
    static inline ptrdiff_t apply(U &obj)
    {
      return obj.confirmGhostsUpToDate();
    }

    template <typename U>
      requires(!HasGhostMethod<U>)
    static constexpr inline ptrdiff_t apply(U &obj)
    {
      return 0;
    }

    template <int N, typename U>
      requires HasGhostMethodIndexed<N, U>
    static inline ptrdiff_t apply(U &obj, Tag<N> i)
    {
      return obj.confirmGhostsUpToDate(i);
    }

    template <int N, typename U>
      requires(!HasGhostMethodIndexed<N, U> && HasGhostMethodElement<N, U>)
    static inline ptrdiff_t apply(U &obj, Tag<N> i)
    {
      return obj(i).confirmGhostsUpToDate();
    }

    template <int N, typename U>
      requires(!HasGhostMethodIndexed<N, U> && !HasGhostMethodElement<N, U>)
    static constexpr inline ptrdiff_t apply(U &obj, Tag<N> i)
    {
      return 0;
    }

    template <typename U>
      requires HasGhostMethodDirectIndexed<U>
    static inline ptrdiff_t apply(U &obj, ptrdiff_t i)
    {
      return obj.confirmGhostsUpToDate(i);
    }

    template <typename U>
      requires(!HasGhostMethodDirectIndexed<U> && HasGhostMethodDirectElement<U>)
    static inline ptrdiff_t apply(U &obj, ptrdiff_t i)
    {
      return obj(i).confirmGhostsUpToDate();
    }

    template <typename U>
      requires(!HasGhostMethodDirectIndexed<U> && !HasGhostMethodDirectElement<U>)
    static constexpr inline ptrdiff_t apply(U &obj, ptrdiff_t i)
    {
      return 0;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    ConfirmGhosts() = default;
  };
} // namespace TempLat

#endif
