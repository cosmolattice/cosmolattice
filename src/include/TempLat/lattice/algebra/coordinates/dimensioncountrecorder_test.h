#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_DIMENSIONCOUNTRECORDER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_DIMENSIONCOUNTRECORDER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::DimensionCountRecorder::Test(TempLat::TDDAssertion& tdd) {

    DimensionCountRecorder cn(SpaceStateInterface::SpaceType::Fourier);

    cn.confirmSpace(LayoutStruct({1,1}), SpaceStateInterface::SpaceType::Fourier);

    tdd.verify( cn.getNDimensions() == 2 );

    cn.confirmSpace(LayoutStruct({1,1,1,1,1,1,1,1}), SpaceStateInterface::SpaceType::Fourier);

    tdd.verify( cn.getNDimensions() == 8 );

}

#endif
