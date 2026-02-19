/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/threadsettings.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ThreadSettingsTester {
    static void Test(TDDAssertion &tdd);
  };

  void ThreadSettingsTester::Test(TDDAssertion &tdd)
  {

    ptrdiff_t initialThreadCount = ThreadSettings::getMaxThreadCount();

    ptrdiff_t initialMPISize = ThreadSettings::getMPILocalSize();

    ThreadSettings::setMPILocalSize(initialMPISize * 2);

    ptrdiff_t newThreadCount = ThreadSettings::getMaxThreadCount();

    ptrdiff_t manuallyComputedThreadCount = std::max((ptrdiff_t)1, initialThreadCount / 2);

    tdd.verify(manuallyComputedThreadCount == newThreadCount);

    ThreadSettings::setMPILocalSize(initialMPISize);

    say << ThreadSettings::getInstance();
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ThreadSettingsTester> test;
}
