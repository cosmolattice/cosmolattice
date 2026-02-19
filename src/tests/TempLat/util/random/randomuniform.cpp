
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/random/randomuniform.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/almostequal.h"
#include <iomanip> // setprecision
#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{

  struct RandomUniformTester {
    static void Test(TDDAssertion &tdd);
  };

  void RandomUniformTester::Test(TDDAssertion &tdd)
  {
    {
      constexpr size_t N = 1000000;
      RandomUniform prng("Hello CosmoLattice world!");

      double x = 0;

      // Let's draw two generations
      device::iteration::reduce<1>(
          "RandomUniformTester", {0}, {N},
          DEVICE_LAMBDA(device::IdxArray<1> i, double &sum) { sum += prng.get(i[0], i[0], 0); }, x);
      tdd.verify(AlmostEqual(x, 500102.3901097552152350545));
      device::iteration::reduce<1>(
          "RandomUniformTester", {0}, {N},
          DEVICE_LAMBDA(device::IdxArray<1> i, double &sum) { sum += prng.get(i[0], i[0], 1); }, x);
      tdd.verify(AlmostEqual(x, 499372.8769075584132224321));

      // And let's do that again:
      device::iteration::reduce<1>(
          "RandomUniformTester", {0}, {N},
          DEVICE_LAMBDA(device::IdxArray<1> i, double &sum) { sum += prng.get(i[0], i[0], 0); }, x);
      tdd.verify(AlmostEqual(x, 500102.3901097552152350545));
      device::iteration::reduce<1>(
          "RandomUniformTester", {0}, {N},
          DEVICE_LAMBDA(device::IdxArray<1> i, double &sum) { sum += prng.get(i[0], i[0], 1); }, x);
      tdd.verify(AlmostEqual(x, 499372.8769075584132224321));

      // Just a brief check, that all generated numbers are different
      device::memory::NDView<1, double> a("a", 10);
      device::iteration::foreach<1>(
          "RandomUniformTester", {0}, {10},
          DEVICE_LAMBDA(device::IdxArray<1> i) { a(i[0]) = prng.get(i[0], i[0], 0); });
      auto a_host = device::memory::createMirrorView(a);
      device::memory::copyDeviceToHost(a, a_host.data());
      for (size_t i = 0; i < 9; ++i)
        tdd.verify(!AlmostEqual(a_host(i), a_host(i + 1)));

      // If we use the same state, all values should be identical
      device::memory::NDView<1, double> b("b", 2);
      device::iteration::foreach<1>(
          "RandomUniformTester", {0}, {2}, DEVICE_LAMBDA(device::IdxArray<1> i) { b(i[0]) = prng.get(0, 0, 0); });
      auto b_host = device::memory::createMirrorView(b);
      device::memory::copyDeviceToHost(b, b_host.data());
      tdd.verify(AlmostEqual(b_host(0), b_host(1)));
    }
    {
      // Test saveState/loadState round-trip
      RandomUniform<> rng("serialization_test");
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
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::RandomUniformTester> test;
}
