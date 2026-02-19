#ifndef TEMPLAT_PARALLEL_DEVICE_GUARD_H
#define TEMPLAT_PARALLEL_DEVICE_GUARD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#include "TempLat/parallel/device.h"

#ifdef DEVICE_KOKKOS

#include "TempLat/parallel/devices/kokkos/session/kokkos_guard.h"

#elif DEVICE_STD

#include "TempLat/parallel/devices/std/std_guard.h"

#else

static_assert(false, "Unknown device type.");

#endif

namespace TempLat
{
  using export_device_namespace::DeviceGuard;
} // namespace TempLat

#endif
