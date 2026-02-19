
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/memory/memorymanager.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  template <size_t NDim, typename T> struct MemoryManagerTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim, typename T> void MemoryManagerTester<NDim, T>::Test(TDDAssertion &tdd)
  {
    auto toolBox = MemoryToolBox<NDim>::makeShared(192, 2);

    toolBox->mFFTLibrary.setVerbose();

    MemoryManager<NDim, T> mManager(toolBox);

    if (TDDRegister::isSingleUnitTest()) {
      std::cerr << mManager << "\n\n";

      toolBox->setVerbose();
    }

    /* first allocation */
    tdd.verify(mManager.confirmConfigSpace() > 0);

    /* fft necessary */
    tdd.verify(mManager.confirmFourierSpace() > 0);

    /* fft not necessary */
    tdd.verify(mManager.confirmFourierSpace() == 0);

    /* fft necessary */
    tdd.verify(mManager.confirmConfigSpace() > 0);

    /* fft not necessary */
    tdd.verify(mManager.confirmConfigSpace() == 0);

    /* ghost update necessary */
    tdd.verify(mManager.confirmGhostsUpToDate() > 0);

    /* ghost update not necessary */
    tdd.verify(mManager.confirmGhostsUpToDate() == 0);

    /* fft not necessary */
    tdd.verify(mManager.confirmConfigSpace() == 0);

    /* ghost update not necessary */
    tdd.verify(mManager.confirmGhostsUpToDate() == 0);

    /* fft necessary */
    tdd.verify(mManager.confirmFourierSpace() > 0);

    /* fft && ghost update necessary */
    tdd.verify(mManager.confirmGhostsUpToDate() > 0);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MemoryManagerTester<3, double>> test;
}
