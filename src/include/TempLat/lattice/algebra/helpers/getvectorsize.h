#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORSIZE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORSIZE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/hasvectorgetmethod.h"

namespace TempLat
{
  /** @brief A class which gets the size of a vector like composite object.
   *
   * Unit test: ctest -R test-getvectorsize
   **/
  class GetVectorSize
  {
  public:
    // Put public methods here. These should change very little over time.

    template <typename U>
      requires HasVectorGetMethod<U>
    static inline ptrdiff_t getVectorSize(U &obj)
    {
      return obj.getVectorSize();
    }

    template <typename U>
      requires(!HasVectorGetMethod<U>)
    static inline ptrdiff_t getVectorSize(U &&obj)
    {
      return 1;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    GetVectorSize() = default;
  };
} // namespace TempLat

#endif
