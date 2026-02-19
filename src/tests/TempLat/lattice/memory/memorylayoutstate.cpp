
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/memory/memorylayoutstate.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct MemoryLayoutStateTester {
    static void Test(TDDAssertion &tdd);
  };

  void MemoryLayoutStateTester::Test(TDDAssertion &tdd)
  {

    MemoryLayoutState mState;

    tdd.verify(mState.isConfigSpace() && mState.isFFTConfigSpace() && mState.isFourierSpace());

    mState.setToConfigSpace();

    tdd.verify(mState.isConfigSpace() && !mState.isFFTConfigSpace() && !mState.isFourierSpace());

    mState.setToFFTConfigSpace();

    tdd.verify(!mState.isConfigSpace() && mState.isFFTConfigSpace() && !mState.isFourierSpace());

    mState.setToFourierSpace();

    tdd.verify(!mState.isConfigSpace() && !mState.isFFTConfigSpace() && mState.isFourierSpace());
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MemoryLayoutStateTester> test;
}
