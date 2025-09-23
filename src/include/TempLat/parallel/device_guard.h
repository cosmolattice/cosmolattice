#ifndef TEMPLAT_PARALLEL_DEVICE_GUARD_H
#define TEMPLAT_PARALLEL_DEVICE_GUARD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#ifdef DEVICE_KOKKOS

#include "TempLat/parallel/kokkos/session/kokkos_guard.h"
namespace TempLat
{
  namespace export_device_namespace = device_kokkos;
} // namespace TempLat

#else

#endif

namespace TempLat
{
  using export_device_namespace::DeviceGuard;
} // namespace TempLat

#endif
