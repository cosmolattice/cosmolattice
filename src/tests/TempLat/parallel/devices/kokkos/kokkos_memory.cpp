/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/devices/kokkos/kokkos_memory.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct KokkosMemoryTester {
    static void Test(TDDAssertion &tdd);
  };

  void KokkosMemoryTester::Test(TDDAssertion &tdd)
  {
    // Simplest test
    {
      say << "Testing 1D copy device to host";
      Kokkos::View<double *, Kokkos::LayoutRight, Kokkos::DefaultExecutionSpace> a("a", 10);
      Kokkos::parallel_for(
          Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>((size_t)0, 10), DEVICE_LAMBDA(size_t i) { a(i) = i; });
      std::vector<double> host_a(10);
      device_kokkos::memory::copyDeviceToHost(a, host_a.data());

      std::cout << "Got: ";
      for (auto v : host_a)
        std::cout << v << " ";
      std::cout << std::endl;

      bool all_correct = true;
      for (size_t i = 0; i < 10; ++i)
        all_correct &= (host_a[i] == i);
      tdd.verify(all_correct);
    }
    // Test with subview
    {
      say << "Testing 1D copy device to host with subview";
      Kokkos::View<double *, Kokkos::LayoutRight, Kokkos::DefaultExecutionSpace> _a("a", 10);

      auto a = Kokkos::subview(_a, std::make_pair((size_t)2, (size_t)8));

      Kokkos::parallel_for(
          Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>((size_t)0, 10), DEVICE_LAMBDA(size_t i) { _a(i) = i; });

      std::vector<double> host_a(6);
      device_kokkos::memory::copyDeviceToHost(a, host_a.data());

      std::cout << "Got: ";
      for (auto v : host_a)
        std::cout << v << " ";
      std::cout << std::endl;

      bool all_correct = true;
      for (size_t i = 0; i < 6; ++i)
        all_correct &= (host_a[i] == 2 + i);
      tdd.verify(all_correct);
    }
    // Test 2D
    {
      say << "Testing 2D copy device to host";

      Kokkos::View<double **, Kokkos::LayoutRight, Kokkos::DefaultExecutionSpace> a("a", 10, 10);
      Kokkos::parallel_for(
          Kokkos::MDRangePolicy<Kokkos::DefaultExecutionSpace, Kokkos::Rank<2>>({0, 0}, {10, 10}),
          DEVICE_LAMBDA(size_t i, size_t j) { a(i, j) = i * 10 + j; });

      std::vector<double> host_a(100);

      device_kokkos::memory::copyDeviceToHost(a, host_a.data());

      bool all_correct = true;
      for (size_t i = 0; i < 10; ++i)
        for (size_t j = 0; j < 10; ++j) {
          all_correct &= (host_a[i * 10 + j] == i * 10 + j);
          if (host_a[i * 10 + j] != i * 10 + j) {
            std::cout << "Error at " << i << " " << j << " got " << host_a[i * 10 + j] << " expected " << i * 10 + j
                      << std::endl;
          }
        }
      tdd.verify(all_correct);
    }
    // Test 2D with subview
    {
      say << "Testing 2D copy device to host with subview";

      Kokkos::View<double **, Kokkos::LayoutRight, Kokkos::DefaultExecutionSpace> _a("a", 10, 10);
      Kokkos::parallel_for(
          Kokkos::MDRangePolicy<Kokkos::DefaultExecutionSpace, Kokkos::Rank<2>>({0, 0}, {10, 10}),
          DEVICE_LAMBDA(size_t i, size_t j) { _a(i, j) = i * 10 + j; });

      auto a = Kokkos::subview(_a, std::make_pair((size_t)1, (size_t)9), std::make_pair((size_t)2, (size_t)8));

      std::vector<double> host_a(6 * 8);

      device_kokkos::memory::copyDeviceToHost(a, host_a.data());

      bool all_correct = true;
      for (size_t i = 1; i < 9; ++i)
        for (size_t j = 2; j < 8; ++j) {
          all_correct &= (host_a[(i - 1) * 6 + (j - 2)] == i * 10 + j);
          if (host_a[(i - 1) * 6 + (j - 2)] != i * 10 + j) {
            std::cout << "Error at " << i << " " << j << " got " << host_a[(i - 1) * 6 + (j - 2)] << " expected "
                      << i * 10 + j << std::endl;
          }
        }
      tdd.verify(all_correct);
    }
    // Test large 2D with subview
    {
      say << "Testing 2D copy device to host with subviews (large arrays)";

      Kokkos::View<double **, Kokkos::LayoutRight, Kokkos::DefaultExecutionSpace> _a("a", 128, 128);
      Kokkos::parallel_for(
          Kokkos::MDRangePolicy<Kokkos::DefaultExecutionSpace, Kokkos::Rank<2>>({0, 0}, {128, 128}),
          DEVICE_LAMBDA(size_t i, size_t j) { _a(i, j) = i * 10 + j; });

      auto a = Kokkos::subview(_a, std::make_pair((size_t)4, (size_t)110), std::make_pair((size_t)2, (size_t)120));

      std::vector<double> host_a(118 * 106);

      device_kokkos::memory::copyDeviceToHost(a, host_a.data());

      bool all_correct = true;
      for (size_t i = 4; i < 110; ++i)
        for (size_t j = 2; j < 120; ++j) {
          all_correct &= (host_a[(i - 4) * 118 + (j - 2)] == i * 10 + j);
          if (host_a[(i - 4) * 118 + (j - 2)] != i * 10 + j) {
            std::cout << "Error at " << i << " " << j << " got " << host_a[(i - 4) * 118 + (j - 2)] << " expected "
                      << i * 10 + j << std::endl;
          }
        }
      tdd.verify(all_correct);
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::KokkosMemoryTester> test;
} // namespace
