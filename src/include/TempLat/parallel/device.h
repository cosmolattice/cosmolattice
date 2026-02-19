#ifndef TEMPLAT_PARALLEL_DEVICE_H
#define TEMPLAT_PARALLEL_DEVICE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#ifdef DEVICE_KOKKOS

#include "TempLat/parallel/devices/kokkos/kokkos.h"

namespace TempLat
{
  namespace export_device_namespace = device_kokkos;
} // namespace TempLat

#elif DEVICE_STD

#include "TempLat/parallel/std/std.h"
namespace TempLat
{
  namespace export_device_namespace = std_device;
} // namespace TempLat

#else

static_assert(false, "Unknown device type. Please define DEVICE_KOKKOS or DEVICE_STD.");

#endif

namespace TempLat
{
  namespace device
  {
    using export_device_namespace::reverse_access_pattern;

    // ------------------------------------------------
    // Tuple and logic defaults
    // ------------------------------------------------

    using export_device_namespace::apply;
    using export_device_namespace::array;
    using export_device_namespace::forward_as_tuple;
    using export_device_namespace::get;
    using export_device_namespace::index_sequence;
    using export_device_namespace::make_index_sequence;
    using export_device_namespace::make_pair;
    using export_device_namespace::make_tuple;
    using export_device_namespace::pair;
    using export_device_namespace::tie;
    using export_device_namespace::tuple;
    using export_device_namespace::tuple_cat;

    using export_device_namespace::Idx;
    using export_device_namespace::IdxArray;

    // ------------------------------------------------
    // Atomics
    // ------------------------------------------------

    using export_device_namespace::atomic_add;
    using export_device_namespace::atomic_inc;
    using export_device_namespace::atomic_max;
    using export_device_namespace::atomic_min;

    // ------------------------------------------------
    // Arithmetic defaults
    // ------------------------------------------------

    using export_device_namespace::abs;
    using export_device_namespace::acos;
    using export_device_namespace::acosh;
    using export_device_namespace::asin;
    using export_device_namespace::asinh;
    using export_device_namespace::atan;
    using export_device_namespace::atan2;
    using export_device_namespace::atanh;
    using export_device_namespace::ceil;
    using export_device_namespace::conj;
    using export_device_namespace::cos;
    using export_device_namespace::cosh;
    using export_device_namespace::exp;
    using export_device_namespace::floor;
    using export_device_namespace::fmod;
    using export_device_namespace::imag;
    using export_device_namespace::log;
    using export_device_namespace::max;
    using export_device_namespace::min;
    using export_device_namespace::pow;
    using export_device_namespace::real;
    using export_device_namespace::round;
    using export_device_namespace::sin;
    using export_device_namespace::sinh;
    using export_device_namespace::sqrt;
    using export_device_namespace::tan;
    using export_device_namespace::tanh;

    using export_device_namespace::complex;
  } // namespace device

  // The complex type is used in many places, so we re-export it at the top level.
  using device::complex;

  template <typename T, size_t N>
    requires(!std::is_same_v<device::array<T, N>, std::array<T, N>>)
  std::ostream &operator<<(std::ostream &stream, const device::array<T, N> &vec)
  {
    stream << "{{ ";
    bool first = true;
    int limiter = 0;
    for (auto &&it : vec) {
      if (++limiter > 10) {
        stream << ", ...";
        break;
      }
      if (first) {
        first = false;
      } else {
        stream << ", ";
      }
      stream << it;
    }
    stream << " }}";
    return stream;
  };

} // namespace TempLat

// Including this here, as we need that anywhere basically, where Kokkos is explicitly used.
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"

#endif
