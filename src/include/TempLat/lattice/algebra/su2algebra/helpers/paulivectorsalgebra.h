#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_HELPERS_PAULIVECTORSALGEBRA_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_HELPERS_PAULIVECTORSALGEBRA_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Year: 2025

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{

  /** @brief A class which implements the SU2 algebra at the single element level.
   *
   *
   * Unit test: ctest -R test-paulivectorsalgebra
   **/
  class PauliVectorsAlgebra
  {
  public:
    /* Put public methods here. These should change very little over time. */

    template <typename Array>
      requires requires(Array a) {
        a[0];
        a[1];
        a[2];
        a[3];
      }
    DEVICE_FORCEINLINE_FUNCTION static void multiply_inplace(Array &res, const Array &cL, const Array &cR)
    {
      res[0] = cL[0] * cR[0] - cL[1] * cR[1] - cL[2] * cR[2] - cL[3] * cR[3];
      res[1] = cL[0] * cR[1] + cL[1] * cR[0] + cL[3] * cR[2] - cL[2] * cR[3];
      res[2] = cL[0] * cR[2] + cL[2] * cR[0] + cL[1] * cR[3] - cL[3] * cR[1];
      res[3] = cL[0] * cR[3] + cL[3] * cR[0] + cL[2] * cR[1] - cL[1] * cR[2];
    }
  };


#ifdef TEMPLATTEST
  class PauliVectorsAlgebraTester
  {
  public:
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
