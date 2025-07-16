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
  RandomGaussian prng("Hello CosmoLattice world!", N);
  say << prng << "\n";

  static constexpr ptrdiff_t measure_center = 10;

  double x = 0;

#ifdef NOKOKKOS
  std::array<size_t, 2 * measure_center> measure;
  std::fill(measure.begin(), measure.end(), 0);
  for (size_t i = 0; i < N; ++i) {
    double next = prng();
    x += next;
    ptrdiff_t index = measure_center + std::round(next * measure_center / 3); /* 5 ? yes, 5 i_sigma happens. */
    index = std::max(ptrdiff_t(0), std::min(2 * measure_center - 1, index));
    ++measure[index];
  }
  //    std::cerr << std::fixed << std::setprecision(32) << "x: " << x << "\n";

  tdd.verify(AlmostEqual(x, 210.2878193178173376054473919794));
  tdd.verify(prng.getState() == N);

  std::cout << "Obtained x = " << std::setprecision(32) << x << std::endl;
  //    say << prng << "\n";

  say << "Does this look gaussian enough?\n";
  for (auto &&it : measure) {
    if (it > 0) std::cerr << std::string(it / (N / 200), '*') << "\n";
  }

#else
  Kokkos::View<size_t[2 * measure_center]> measure("measure");
  Kokkos::parallel_reduce(
      "RandomGaussian_test", N,
      KOKKOS_LAMBDA(int i, double &sum) {
        const double next = prng(i);
        sum += next;
        ptrdiff_t index = measure_center + std::round(next * measure_center / 3); /* 5 ? yes, 5 i_sigma happens. */
        index = std::max(ptrdiff_t(0), std::min(2 * measure_center - 1, index));
        Kokkos::atomic_inc(&measure[index]);
      },
      x);

  tdd.verify(AlmostEqual(x, 5906.2331311112102412153035402298));

  std::cout << "Obtained x = " << std::setprecision(32) << x << std::endl;

  say << "Does this look gaussian enough?\n";
  auto host_mirror = Kokkos::create_mirror_view(measure);
  Kokkos::deep_copy(host_mirror, measure);
  for (size_t i = 0; i < host_mirror.extent(0); ++i) {
    const auto it = host_mirror[i];
    if (it > 0) std::cerr << std::string(it / (N / 200), '*') << "\n";
  }

#endif
}

#endif
