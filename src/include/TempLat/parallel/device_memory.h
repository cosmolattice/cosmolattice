#ifndef TEMPLAT_PARALLEL_DEVICE_MEMORY_H
#define TEMPLAT_PARALLEL_DEVICE_MEMORY_H

#include "TempLat/parallel/device.h"

#ifdef DEVICE_KOKKOS

#include "TempLat/parallel/kokkos/kokkos_memory.h"

#else

#endif

namespace TempLat
{
  namespace device
  {
    namespace memory
    {
      using export_device_namespace::memory::NDView;
      using export_device_namespace::memory::NDViewUnmanaged;
      using export_device_namespace::memory::NDViewUnmanagedHost;

      using export_device_namespace::memory::copyDeviceToDevice;
      using export_device_namespace::memory::copyDeviceToHost;
      using export_device_namespace::memory::copyHostToDevice;
      using export_device_namespace::memory::getAtOnePoint;
      using export_device_namespace::memory::setAtOnePoint;
    } // namespace memory
  } // namespace device
} // namespace TempLat

#endif
