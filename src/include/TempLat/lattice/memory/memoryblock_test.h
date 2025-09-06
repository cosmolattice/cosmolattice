#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_TEST_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/almostequal.h"

template <size_t NDim, typename T> inline void TempLat::MemoryBlock<NDim, T>::Test(TempLat::TDDAssertion &tdd)
{
#ifndef NOKOKKOS
  MemoryBlock<NDim, T> test(128);

  Kokkos::parallel_for(
      Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>(0, test.size()),
      DEVICE_LAMBDA(const size_t i) { test[i] = i; });

  const auto view = test.getRawHostView();

  bool all_true = true;
  for (size_t i = 0; i < test.size(); ++i) {
    all_true &= (AlmostEqual(view[i], (T)i));
  }
  tdd.verify(all_true);
#else
  MemoryBlock<NDim, T> test(128);

  // verified that we get segfault on i == test.size()
  for (ptrdiff_t i = 0, iEnd = test.size(); i < iEnd; ++i) {
    test[i] = i;
  }
  tdd.verify(true);

#endif
}

#endif
