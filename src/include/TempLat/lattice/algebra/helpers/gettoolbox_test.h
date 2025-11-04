#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETTOOLBOX_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETTOOLBOX_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

// #include "TempLat/lattice/field/field.h"
// #include "TempLat/lattice/algebra/coordinates/wavenumber.h"

inline void TempLat::GetToolBox::Test(TempLat::TDDAssertion &tdd)
{

  struct MyTestOne {
    MyTestOne() : mToolBox(MemoryToolBox<3>::makeShared(32, 1)) {}
    std::shared_ptr<MemoryToolBox<3>> getToolBox() const { return mToolBox; }
    std::shared_ptr<MemoryToolBox<3>> mToolBox;
  };
  MyTestOne one;

  struct MyTestTwo {
  };
  MyTestTwo two;

  tdd.verify(GetToolBox::get(one)->mNDimensions == 3);
  tdd.verify(std::same_as<std::decay_t<decltype(GetToolBox::get(two))>, std::nullptr_t>);
  tdd.verify(Throws<GetToolBoxException>([&]() { GetToolBox::get(two); }));

  //  auto toolBox = MemoryToolBox::makeShared(3, 32, 1);
  //  Field<double> base("base", toolBox);

  //  WaveNumber k(toolBox);

  //  base.inFourierSpace() = power(k.norm(), 2);
  //  tdd.verify( GetToolBox::get(base)->mNDimensions == 3);
}

#endif
