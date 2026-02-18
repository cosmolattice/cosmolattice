#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_TEST_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/almostequal.h"
#include "TempLat/parallel/device_iteration.h"

template <size_t NDim, typename T> inline void TempLat::MemoryBlockTester<NDim, T>::Test(TempLat::TDDAssertion &tdd)
{
  // Basic raw access
  {
    MemoryBlock<NDim, T> test(128);

    device::iteration::foreach<1>("it1", {0}, {128}, DEVICE_LAMBDA(const device::IdxArray<1> i) { test[i[0]] = i[0]; });

    const auto view = test.getRawHostView();

    bool all_true = true;
    for (size_t i = 0; i < test.size(); ++i) {
      all_true &= (AlmostEqual(view[i], (T)i));
    }
    tdd.verify(all_true);
  }
  // Slicing
  if constexpr (NDim == 3) {
    MemoryBlock<NDim, T> test(16 * 16 * 16);

    auto view = test.template getNDView<T>(device::IdxArray<3>{{16, 16, 16}});

    device::iteration::foreach<3>(
        "it2", {0, 0, 0}, {16, 16, 16},
        DEVICE_LAMBDA(const device::IdxArray<3> i) { view(i[0], i[1], i[2]) = i[0] * 256 + i[1] * 16 + i[2]; });

    auto host_view = test.template getNDHostView<T>(device::IdxArray<3>{{16, 16, 16}});

    bool all_true = true;
    for (size_t i = 0; i < 16; ++i) {
      for (size_t j = 0; j < 16; ++j) {
        for (size_t k = 0; k < 16; ++k) {
          all_true &= (AlmostEqual(host_view(i, j, k), (T)(i * 256 + j * 16 + k)));
        }
      }
    }
    tdd.verify(all_true);
  }
}

#endif
