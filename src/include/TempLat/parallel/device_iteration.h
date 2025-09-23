#ifndef TEMPLAT_PARALLEL_DEVICE_ITERATION_H
#define TEMPLAT_PARALLEL_DEVICE_ITERATION_H

#include "TempLat/parallel/device.h"

#ifdef DEVICE_KOKKOS

#include "TempLat/parallel/kokkos/kokkos_iteration.h"

#else

#endif

namespace TempLat
{
  namespace device
  {
    namespace iteration
    {
      using export_device_namespace::iteration::fence;
      using export_device_namespace::iteration::foreach;
      using export_device_namespace::iteration::reduce;
    } // namespace iteration
  } // namespace device
} // namespace TempLat

#endif
