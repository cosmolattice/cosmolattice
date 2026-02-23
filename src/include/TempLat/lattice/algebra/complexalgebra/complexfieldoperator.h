#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDOPERATOR_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDOPERATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget.h"

namespace TempLat
{
  /** @brief A class which implements common features of complex fields operators.
   *
   *
   * Unit test: ctest -R test-complexfieldoperator
   **/
  class ComplexFieldOperator
  {
  public:
    // Put public methods here. These should change very little over time.
    DEVICE_FUNCTION
    ComplexFieldOperator() = default;

    static constexpr size_t size = 2;
    using Getter = ComplexFieldGetter;

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };
} // namespace TempLat

#endif
