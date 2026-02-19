#ifndef TEMPLAT_UTIL_NDLOOP_H
#define TEMPLAT_UTIL_NDLOOP_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include <array>
#include <utility>

namespace TempLat
{
  /**
   * @brief A utility to perform N-dimensional loops over views.
   * As this is not optimized for performance, it should only be used in tests.
   * Naturally, NDLoop is a sequential loop - to use parallelism make use of device::iteration utilities. However,
   * it is CPU-only and has therefore its special use cases.
   *
   * @tparam NDim Dimensionality of the loop.
   * @tparam View Type of the view being iterated over.
   * @tparam Functor Type of the functor to be applied.
   * @param v The view to iterate over.
   * @param i The current index.
   * @return requires
   */
  template <size_t NDim, typename View, typename Functor>
    requires requires(View v, int i) {
      requires(NDim > 0);
      v.size();
      v.extent(i);
    }
  void NDLoop(const View &view, const Functor &functor)
  {
    std::array<ptrdiff_t, NDim> extents;
    for (size_t i = 0; i < NDim; ++i)
      extents[i] = view.extent(i);

    std::array<ptrdiff_t, NDim> idx;
    for (size_t i = 0; i < NDim; ++i)
      idx[i] = 0;

    while (true) {
      std::apply(functor, idx);
      ptrdiff_t dim = (ptrdiff_t)NDim - 1;
      while (dim < (ptrdiff_t)NDim) {
        idx[dim]++;
        if (idx[dim] < extents[dim]) {
          break;
        } else {
          idx[dim] = 0;
          if (dim == 0) return;
          dim--;
        }
      }
    }
  }
} // namespace TempLat

#endif