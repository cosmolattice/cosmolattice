#ifndef TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::RandomGaussianFieldTester::Test(TempLat::TDDAssertion& tdd) {


    /* test the stability of the getter at various coordinates. */

    ptrdiff_t nGrid = 32, nGhost = 1;

    auto toolBox = MemoryToolBox::makeShared(3, nGrid, nGhost);
    RandomGaussianField<double> myField("Hello world",toolBox);
//
//  //  auto myFieldGetter = GetterWrap(myField);
//
//  //  auto test = myFieldGetter + myFieldGetter + myFieldGetter;
//
//  //  /* let it register the dimensionality */
//  //  myField.confirmSpace(LayoutStruct(3), SpaceStateInterface::SpaceType::Fourier);
//
//    say << myField.getNDimensions() << " dimensions detected.\n";
//
    std::array<std::complex<double>, 10> a, b;
//
//    auto pIterCoords = IterationCoordinates::createDummy(0, 0, 0, 0);
      std::vector<ptrdiff_t> coord({0,0,0});

//
    auto it = toolBox->itP();
    for (ptrdiff_t i = 0, iEnd = a.size(); i < iEnd; ++i) {
        coord[2] = i;
//        pIterCoords.setOffset(i);
        a[i] = myField.get(coord);
    }
//
    auto firstSeed = myField.getCurrentSeed();
//
//    /* test rewinding */
    bool rewindingWorks = true;
    for (ptrdiff_t i = 0, iEnd = a.size(); i < iEnd; ++i) {
       coord[2] = i;
//        pIterCoords.setOffset(i);
        b[i] = myField.get(coord);
        rewindingWorks = rewindingWorks && AlmostEqual(a[i], b[i]);
    }
//
    tdd.verify( rewindingWorks );
//
    tdd.verify( firstSeed == myField.getCurrentSeed());
//
//    /* test moving */
    bool movingWorks = true;
    bool seedChanged = false;
    coord[1] = 10;
    for (ptrdiff_t i = 0, iEnd = a.size(); i < iEnd; ++i) {
        coord[2] = i;
        b[i] = myField.get(coord);
//
        seedChanged = firstSeed != myField.getCurrentSeed();
        if ( ! seedChanged ) break;
//
        movingWorks = movingWorks && ! AlmostEqual(a[i], b[i]);
    }
//
    tdd.verify( seedChanged );
//
    tdd.verify( movingWorks );
//
//    /* test moving back */
    bool movingBackWorks = true;
    coord[1] = 0;
    for (ptrdiff_t i = 0, iEnd = a.size(); i < iEnd; ++i) {
        coord[2] = i;
        b[i] = myField.get(coord);
        movingBackWorks = movingBackWorks && AlmostEqual(a[i], b[i]);
    }
    tdd.verify( movingBackWorks );

    tdd.verify( Throws<RandomGaussianFieldNegativeFrequencyException> (
               [&]() {
                   coord[2] = -1;
                   myField.get(coord);
               }
    ));

}

#endif
