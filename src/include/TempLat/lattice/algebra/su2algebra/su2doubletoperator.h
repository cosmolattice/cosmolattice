#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETOPERATOR_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETOPERATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletget.h"

namespace TempLat
{
  /** @brief A class which groups some features of the SU2Doublet ops.
   *
   * Unit test: ctest -R test-su2doubletoperator
   **/
  template <size_t NDim> class SU2DoubletOperator
  {
  public:
    // Put public methods here. These should change very little over time.
    DEVICE_FUNCTION
    SU2DoubletOperator() = default;
    static constexpr size_t size = 4;
    using Getter = SU2DoubletGetter;

    static consteval size_t getNDim() { return NDim; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };
} // namespace TempLat

#endif
