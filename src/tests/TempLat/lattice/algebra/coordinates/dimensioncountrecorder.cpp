
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/coordinates/dimensioncountrecorder.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  template <size_t NDim> struct DimensionCountRecorderTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> inline void DimensionCountRecorderTester<NDim>::Test(TDDAssertion &tdd)
  {
    {
      DimensionCountRecorder<2> cn(SpaceStateType::Fourier);
      cn.confirmSpace(LayoutStruct<2>({1, 1}, 1), SpaceStateType::Fourier);
      tdd.verify(cn.getNDimensions() == 2);
    }
    {
      DimensionCountRecorder<8> cn(SpaceStateType::Fourier);
      cn.confirmSpace(LayoutStruct<8>({1, 1, 1, 1, 1, 1, 1, 1}, 1), SpaceStateType::Fourier);
      tdd.verify(cn.getNDimensions() == 8);
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::DimensionCountRecorderTester<2>> test1;
  TempLat::TDDContainer<TempLat::DimensionCountRecorderTester<3>> test2;
} // namespace
