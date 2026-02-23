#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GHOSTSHUNTER_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GHOSTSHUNTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/hasdoweneedghosts.h"

namespace TempLat
{
  /** @brief A class which helps implementing the automatic management of updating the ghost cells.
   *
   * Unit test: ctest -R test-ghostshunter
   **/
  class GhostsHunter
  {
  public:
    template <typename U>
      requires HasDoWeNeedGhostsMethod<U>
    static void apply(U &obj)
    {
      obj.doWeNeedGhosts();
    }

    template <typename U>
      requires(!HasDoWeNeedGhostsMethod<U>)
    static void apply(U &obj)
    {
    }

    // Put public methods here. These should change very little over time.
    template <int N, typename U>
      requires HasDoWeNeedGhostsMethodIndexed<N, U>
    static void apply(U &obj, Tag<N> i)
    {
      obj.doWeNeedGhosts(i);
    }

    template <int N, typename U>
      requires(!HasDoWeNeedGhostsMethodIndexed<N, U>)
    static void apply(U &obj, Tag<N> i)
    {
    }

    // Put public methods here. These should change very little over time.
    template <typename U>
      requires HasDoWeNeedGhostsMethodIndexedDyn<U>
    static void apply(U &obj, ptrdiff_t i)
    {
      obj.doWeNeedGhosts(i);
    }

    template <typename U>
      requires(!HasDoWeNeedGhostsMethodIndexedDyn<U>)
    static void apply(U &obj, ptrdiff_t i)
    {
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    GhostsHunter() = default;
  };
} // namespace TempLat

#endif
