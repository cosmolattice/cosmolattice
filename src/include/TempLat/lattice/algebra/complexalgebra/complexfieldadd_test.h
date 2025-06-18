#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDADD_TEST_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDADD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include <Kokkos_Macros.hpp>
inline void TempLat::ComplexFieldAddTester::Test(TempLat::TDDAssertion &tdd)
{
  struct MyStruct {
    KOKKOS_FORCEINLINE_FUNCTION
    int ComplexFieldGet(Tag<0> t) const { return 1; }
    KOKKOS_FORCEINLINE_FUNCTION
    int ComplexFieldGet(Tag<1> t) const { return 2; }

    using Getter = ComplexFieldGetter;
  };
  struct MyStruct2 {
    KOKKOS_FORCEINLINE_FUNCTION
    int ComplexFieldGet(Tag<0> t) const { return 3; }
    KOKKOS_FORCEINLINE_FUNCTION
    int ComplexFieldGet(Tag<1> t) const { return 4; }

    using Getter = ComplexFieldGetter;
  };

  auto test = MyStruct() + MyStruct2();
  /* Default is to fail: to remind yourself to implement something here. */
  tdd.verify(test.ComplexFieldGet(0_c) == 4);
  tdd.verify(test.ComplexFieldGet(1_c) == 6);
}

#endif
