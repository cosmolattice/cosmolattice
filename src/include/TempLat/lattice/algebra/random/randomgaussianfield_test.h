#ifndef TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/field/field.h"

inline void TempLat::RandomGaussianFieldTester::Test(TempLat::TDDAssertion &tdd)
{
  /* test the stability of the getter at various coordinates. */

  ptrdiff_t nGrid = 4, nGhost = 1;

  auto toolBox = MemoryToolBox<3>::makeShared(nGrid, nGhost);
  RandomGaussianField<3, double> myField("Hello world", toolBox);
  //
  //  //  auto myFieldGetter = GetterWrap(myField);
  //
  //  //  auto test = myFieldGetter + myFieldGetter + myFieldGetter;
  //
  //  //  /* let it register the dimensionality */
  //  //  myField.confirmSpace(LayoutStruct(3), SpaceStateInterface::SpaceType::Fourier);
  //
  //    say << myField.getNDimensions() << " dimensions detected.\n";

  Field<3, double> a("a", toolBox);
  Field<3, double> b("b", toolBox);

  // Get random values
  a.inFourierSpace() = 100;
  // a.inFourierSpace() = myField;

  // test rewinding
  auto firstSeed = myField.getCurrentSeed();
  // myField.reset(); // reset the field to the initial seed
  // myField = RandomGaussianField<3, double>("Hello world", toolBox);
  b.inFourierSpace() = 10;
  //  b.inFourierSpace() = myField;

  // get host views
  auto a_host = a.inFourierSpace().directView();
  auto b_host = b.inFourierSpace().directView();

  for (size_t i = 0; i < a_host.size(); ++i)
    std::cout << "a = " << a_host[i] << ", b = " << b_host[i] << std::endl;

  bool rewindingWorks = true;
  for (size_t i = 0; i < a_host.size(); ++i)
    rewindingWorks = rewindingWorks && AlmostEqual(a_host[i], b_host[i]);
  tdd.verify(rewindingWorks);
  tdd.verify(firstSeed == myField.getCurrentSeed());

  /*
  // test moving
  bool movingWorks = true;
  bool seedChanged = false;
  coord[1] = 10;
  for (ptrdiff_t i = 0, iEnd = a.size(); i < iEnd; ++i) {
    coord[2] = i;
    b[i] = myField.get(coord);
    seedChanged = firstSeed != myField.getCurrentSeed();
    if (!seedChanged) break;
    movingWorks = movingWorks && !AlmostEqual(a[i], b[i]);
  }
  tdd.verify(seedChanged);
  tdd.verify(movingWorks);

  // test moving back
  bool movingBackWorks = true;
  coord[1] = 0;
  for (ptrdiff_t i = 0, iEnd = a.size(); i < iEnd; ++i) {
    coord[2] = i;
    b[i] = myField.get(coord);
    movingBackWorks = movingBackWorks && AlmostEqual(a[i], b[i]);
  }
  tdd.verify(movingBackWorks);

  tdd.verify(Throws<RandomGaussianFieldNegativeFrequencyException>([&]() {
    coord[2] = -1;
    myField.get(coord);
  }));
  */
}

#endif
