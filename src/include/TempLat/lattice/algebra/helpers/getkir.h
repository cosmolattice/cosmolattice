#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETKIR_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETKIR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/haskir.h"

namespace TempLat
{
  /** @brief A getter for kIR.
   *
   * Careful, this method works only on types which also have a get method.
   * Need this to deduce the return type.
   *
   * Unit test: ctest -R test-getkir
   **/
  class GetKIR
  {
  public:
    // Put public methods here. These should change very little over time.

    template <typename U>
      requires HasKIR<U>
    DEVICE_FORCEINLINE_FUNCTION static auto getKIR(U &obj)
    {
      return obj.getKIR();
    }

    template <typename U>
      requires(!HasKIR<U>)
    DEVICE_FORCEINLINE_FUNCTION static constexpr double getKIR(U &obj)
    {
      return 1;
    }
  };
} // namespace TempLat

#endif
