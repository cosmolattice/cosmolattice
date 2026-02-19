
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025
#include "TempLat/parallel/devices/kokkos/session/kokkos_guard.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct DeviceGuardTester {
    static void Test(TDDAssertion &tdd);
  };

  void DeviceGuardTester::Test(TDDAssertion &tdd)
  {
    using device_kokkos::DeviceGuard;
    using device_kokkos::KokkosDeviceGuardInstantiationException;
    if (DeviceGuard::GetInstanceCount() < 1) {
      DeviceGuard guard(0, NULL, true);
    } else {
      /* there is an instance of DeviceGuard in the calling main, which is a good thing. Then we can test if the
       * multiple instantiation protection works. */
      tdd.verify(Throws<KokkosDeviceGuardInstantiationException>([]() { DeviceGuard guard(0, NULL, true); }));
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::DeviceGuardTester> test;
}
