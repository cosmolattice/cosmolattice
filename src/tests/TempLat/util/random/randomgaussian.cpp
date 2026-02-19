/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/random/randomgaussian.h"
#include "TempLat/util/tdd/tdd.h"
#include <algorithm>
#include <cstddef>
#include <vector>
#include "TempLat/parallel/device_iteration.h"
#include "TempLat/parallel/device_memory.h"

namespace TempLat
{

  struct RandomGaussianTester {
    static void Test(TDDAssertion &tdd);
  };

  void RandomGaussianTester::Test(TDDAssertion &tdd)
  {
    constexpr size_t N = 1e8;
    RandomGaussian prng("Hello CosmoLattice world!");
    say << prng << "\n";

    static constexpr ptrdiff_t measure_center = 10;

    double x = 0;

    device::memory::NDView<1, size_t> measure("measure", 2 * measure_center);
    device::iteration::reduce<1>(
        "RandomGaussian_test", {0}, {N},
        DEVICE_LAMBDA(device::IdxArray<1> i, double &sum) {
          const double next = prng.get(i[0], i[0], 0);
          sum += next;
          ptrdiff_t index = measure_center + std::round(next * measure_center / 3); /* 5 ? yes, 5 i_sigma happens. */
          index = std::max(ptrdiff_t(0), std::min(2 * measure_center - 1, index));
          device::atomic_inc(&measure[index]);
        },
        x);

    tdd.verify(AlmostEqual(x, 2672.0293294442953992984257638454));

    // Test saveState/loadState round-trip
    RandomGaussian rng("serialization_test");
    std::string savedState = rng.saveState();

    // Generate 1000 values after saving state
    std::vector<double> seq1;
    for (int i = 0; i < 1000; ++i) {
      seq1.push_back(rng.get(i, 0, 0));
    }

    // Restore state and generate again
    rng.loadState(savedState);
    std::vector<double> seq2;
    for (int i = 0; i < 1000; ++i) {
      seq2.push_back(rng.get(i, 0, 0));
    }

    // Verify sequences are identical
    bool sequencesMatch = true;
    for (int i = 0; i < 1000; ++i) {
      if (seq1[i] != seq2[i]) {
        sequencesMatch = false;
        break;
      }
    }
    tdd.verify(sequencesMatch, "saveState/loadState round-trip produces identical sequence");
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::RandomGaussianTester> test;
}
