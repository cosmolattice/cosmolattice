#ifndef TEMPLAT_PARALLEL_KOKKOS_OPERATIONS_TEST_H
#define TEMPLAT_PARALLEL_KOKKOS_OPERATIONS_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

void TempLat::KokkosOperationsTester::Test(TempLat::TDDAssertion &tdd)
{
  // Simplest test
  {
    say << "Testing 1D copy device to host";
    Kokkos::View<double *, Kokkos::DefaultExecutionSpace> a("a", 10);
    Kokkos::parallel_for(
        Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>((size_t)0, 10), KOKKOS_LAMBDA(size_t i) { a(i) = i; });

    std::vector<double> host_a(10);

    device::copyDeviceToHost(a, host_a.data());

    bool all_correct = true;
    for (size_t i = 0; i < 10; ++i)
      all_correct &= (host_a[i] == i);
    tdd.verify(all_correct);
  }
  // Test with subview
  {
    say << "Testing 1D copy device to host with subview";
    Kokkos::View<double *, Kokkos::DefaultExecutionSpace> _a("a", 10);

    auto a = Kokkos::subview(_a, std::make_pair((size_t)2, (size_t)8));

    Kokkos::parallel_for(
        Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>((size_t)0, 10), KOKKOS_LAMBDA(size_t i) { a(i) = i; });

    std::vector<double> host_a(10);

    device::copyDeviceToHost(a, host_a.data());
  }
  // Test 2D
  {
    say << "Testing 2D copy device to host";

    Kokkos::View<double **, Kokkos::DefaultExecutionSpace> a("a", 10, 10);
    Kokkos::parallel_for(
        Kokkos::MDRangePolicy<Kokkos::DefaultExecutionSpace, Kokkos::Rank<2>>({0, 0}, {10, 10}),
        KOKKOS_LAMBDA(size_t i, size_t j) { a(i, j) = j * 10 + i; });

    std::vector<double> host_a(100);

    device::copyDeviceToHost(a, host_a.data());

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

    Kokkos::View<double **, Kokkos::DefaultExecutionSpace> _a("a", 10, 10);
    Kokkos::parallel_for(
        Kokkos::MDRangePolicy<Kokkos::DefaultExecutionSpace, Kokkos::Rank<2>>({0, 0}, {10, 10}),
        KOKKOS_LAMBDA(size_t i, size_t j) { _a(i, j) = j * 10 + i; });

    auto a = Kokkos::subview(_a, std::make_pair((size_t)2, (size_t)8), std::make_pair((size_t)1, (size_t)9));

    std::vector<double> host_a(6 * 8);

    device::copyDeviceToHost(a, host_a.data());

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
}

#endif