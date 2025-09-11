#ifndef TEMPLAT_PARALLEL_DEVICE_GUARD_H
#define TEMPLAT_PARALLEL_DEVICE_GUARD_H

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
