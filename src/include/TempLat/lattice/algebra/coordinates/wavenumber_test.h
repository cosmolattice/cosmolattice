#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
inline void TempLat::WaveNumberTester::Test(TempLat::TDDAssertion& tdd) {

    //WaveNumber cn;
    //
    ///* should not be happy with configuration space. */
    //tdd.verify( Throws<DimensionCountRecorder_CoordinateSpaceException>(
    //       [&]() {
    //           cn.getNorm().confirmSpace(LayoutStruct(1), SpaceStateInterface::SpaceType::Configuration);
    //       }
    //) );

    ///* should be happy with fourier space. */
    //tdd.verify( DoesNotThrow<DimensionCountRecorder_CoordinateSpaceException>(
    //       [&]() {
    //           cn.getNorm().confirmSpace(LayoutStruct(1), SpaceStateInterface::SpaceType::Fourier);
    //       }
    //) );

    auto mtb = MemoryToolBox::makeShared(3, 16, 1);
    WaveNumber k(mtb);

    auto kn=k.norm2()*pow<2>(0.685);
    auto knkn= kn * kn;
    //say<<knkn.get(45);
    auto kn1=k.norm()*pow<2>(0.685);
    //say<<knkn.get(45);
    auto kn1kn1= kn1 * kn1;
    //say<<kn1.get(45);
    //say<<kn1kn1.get(45);

    auto it = mtb->itP();

    //for(it.begin();it.end();++it) say<<k.norm().get(it());

    tdd.verify(true); //Test in power spectrum.

}

#endif
