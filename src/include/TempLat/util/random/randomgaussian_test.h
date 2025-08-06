#ifndef TEMPLAT_UTIL_RANDOM_RANDOMGAUSSIAN_TEST_H
#define TEMPLAT_UTIL_RANDOM_RANDOMGAUSSIAN_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <algorithm>

inline void TempLat::Util::RandomGaussian::Test(TempLat::TDDAssertion &tdd)
{
  constexpr size_t N = 1e8;
  RandomGaussian prng("Hello CosmoLattice world!");
  say << prng << "\n";

  static constexpr ptrdiff_t measure_center = 10;

  double x = 0;

  Kokkos::View<size_t[2 * measure_center]> measure("measure");
  Kokkos::parallel_reduce(
      "RandomGaussian_test", N,
      KOKKOS_LAMBDA(int i, double &sum) {
        const double next = prng.get(i, i, 0);
        sum += next;
        ptrdiff_t index = measure_center + std::round(next * measure_center / 3); /* 5 ? yes, 5 i_sigma happens. */
        index = std::max(ptrdiff_t(0), std::min(2 * measure_center - 1, index));
        Kokkos::atomic_inc(&measure[index]);
      },
      x);

  tdd.verify(AlmostEqual(x, 2672.0293294442953992984257638454));

  say << "Does this look gaussian enough?";
  auto host_mirror = Kokkos::create_mirror_view(measure);
  Kokkos::deep_copy(host_mirror, measure);
  for (size_t i = 0; i < host_mirror.extent(0); ++i) {
    const auto it = host_mirror[i];
    if (it > 0) std::cerr << std::string(it / (N / 200), '*') << "\n";
  }
  std::cerr << "\n\n";
}

#endif
