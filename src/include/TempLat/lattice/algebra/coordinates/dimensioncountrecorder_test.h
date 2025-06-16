#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_DIMENSIONCOUNTRECORDER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_DIMENSIONCOUNTRECORDER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
template <size_t NDim> inline void TempLat::DimensionCountRecorder<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  {
    DimensionCountRecorder<2> cn(SpaceStateInterface<2>::SpaceType::Fourier);
    cn.confirmSpace(LayoutStruct<2>({1, 1}), SpaceStateInterface<2>::SpaceType::Fourier);
    tdd.verify(cn.getNDimensions() == 2);
  }
  {
    DimensionCountRecorder<8> cn(SpaceStateInterface<8>::SpaceType::Fourier);
    cn.confirmSpace(LayoutStruct<8>({1, 1, 1, 1, 1, 1, 1, 1}), SpaceStateInterface<8>::SpaceType::Fourier);
    tdd.verify(cn.getNDimensions() == 8);
  }
}

#endif
