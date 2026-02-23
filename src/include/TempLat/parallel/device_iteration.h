#ifndef TEMPLAT_PARALLEL_DEVICE_ITERATION_H
#define TEMPLAT_PARALLEL_DEVICE_ITERATION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#include "TempLat/parallel/device.h"

#ifdef DEVICE_KOKKOS

#include "TempLat/parallel/devices/kokkos/kokkos_iteration.h"

#elif DEVICE_STD

#include "TempLat/parallel/devices/std/std_iteration.h"

#else

static_assert(false, "No device iteration backend selected.");

#endif

namespace TempLat::device::iteration
{
      using export_device_namespace::iteration::fence;
      using export_device_namespace::iteration::foreach;
      using export_device_namespace::iteration::reduce;

      using export_device_namespace::iteration::Max;
      using export_device_namespace::iteration::Min;
      using export_device_namespace::iteration::Prod;
      using export_device_namespace::iteration::Sum;
} // namespace TempLat::device::iteration

#endif
