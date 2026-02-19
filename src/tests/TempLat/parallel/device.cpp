/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/device.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/device_memory.h"
#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{

  struct DeviceTester {
    template <typename _TDDAssertion> static void Test(_TDDAssertion &tdd);
  };

  template <typename _TDDAssertion> void DeviceTester::Test(_TDDAssertion &tdd)
  {
    // Test device::memory::host_ptr base functionality
    {
      int value = 42;
      // Original pointer
      device::memory::host_ptr<int> devPtr(value);
      tdd.verify(*devPtr == value);

      // Copy constructor
      device::memory::host_ptr<int> devPtr2 = devPtr;
      tdd.verify(*devPtr2 == value);
      tdd.verify(devPtr.get() == devPtr2.get());
      tdd.verify(devPtr.use_count() == 2);
      tdd.verify(devPtr2.use_count() == 2);

      // Copy assignment
      device::memory::host_ptr<int> devPtr3;
      devPtr3 = devPtr2;
      tdd.verify(*devPtr3 == value);
      tdd.verify(devPtr3.get() == devPtr.get());
      tdd.verify(devPtr.use_count() == 3);
      tdd.verify(devPtr2.use_count() == 3);
      tdd.verify(devPtr3.use_count() == 3);

      // Reset pointers
      devPtr2 = nullptr;
      tdd.verify(devPtr.use_count() == 2);
      tdd.verify(devPtr2.use_count() == 0);
      tdd.verify(devPtr3.use_count() == 2);
      tdd.verify(devPtr.get() != nullptr);
      tdd.verify(devPtr2.get() == nullptr);
      tdd.verify(devPtr3.get() != nullptr);
      tdd.verify(*devPtr == value);
      tdd.verify(*devPtr3 == value);

      // Reset original pointer
      devPtr = nullptr;
      tdd.verify(devPtr.use_count() == 0);
      tdd.verify(devPtr2.use_count() == 0);
      tdd.verify(devPtr3.use_count() == 1);
      tdd.verify(devPtr.get() == nullptr);
      tdd.verify(devPtr2.get() == nullptr);
      tdd.verify(devPtr3.get() != nullptr);
      tdd.verify(*devPtr3 == value);

      // Reset last pointer
      devPtr3 = nullptr;
      tdd.verify(devPtr.use_count() == 0);
      tdd.verify(devPtr2.use_count() == 0);
      tdd.verify(devPtr3.use_count() == 0);
      tdd.verify(devPtr.get() == nullptr);
      tdd.verify(devPtr2.get() == nullptr);
      tdd.verify(devPtr3.get() == nullptr);
    }

    // Test that we can construct, copy and assign device::memory::host_ptr in a kernel
    {
      constexpr int value = 123;
      device::memory::host_ptr<int> devPtr(value);

      // We reduce over some indices and copy the ptr inside the kernel. This way, we test that both the copy
      // constructor can be used without errors (runtime or compile-time), and the destructor can be called without
      // errors.
      const device::IdxArray<1> start{0};
      const device::IdxArray<1> stop{8};
      int reduction = 0;
      device::iteration::reduce(
          "Test", start, stop,
          DEVICE_LAMBDA(const device::IdxArray<1> &idx, int &update) {
            [[maybe_unused]] device::memory::host_ptr<int> devPtr2 = devPtr;
            update += idx[0];
            // The following should (and does) segfault if we try to dereference devPtr2 in the kernel.
            //*devPtr2 = 5;
          },
          reduction);
      device::iteration::fence();

      tdd.verify(reduction == 28); // sum 0..7 = 28
      tdd.verify(*devPtr == value);
      tdd.verify(devPtr.use_count() == 1);
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::DeviceTester> test;
}
