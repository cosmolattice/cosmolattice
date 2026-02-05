#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_TEST_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/ndloop.h"

template <size_t NDim, typename T> void TempLat::SU2FieldBase<NDim, T>::Test(TempLat::TDDAssertion &tdd)
{
  const device::Idx nGrid = 8, nGhost = 1;
  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

  Field<NDim, T> f1("myField1", toolBox);
  Field<NDim, T> f2("myField2", toolBox);
  Field<NDim, T> f3("myField3", toolBox);

  auto res = SU2Field<NDim, T>(f1, f2, f3);

  tdd.verify(res.SU2Get(2_c).toString() == "myField2(x)");

  res(1_c) = 6;
  res(2_c) = 12;
  res(3_c) = 24;

  auto ff3 = res(3_c);
  auto ff3_view = ff3.getLocalNDHostView();

  {
    bool all_true = true;
    NDLoop<NDim>(ff3_view, [&](const auto... idx) { all_true &= (ff3_view(idx...) == 24); });
    tdd.verify(all_true);
  }

  SU2Field<NDim, double> mySU2("allNew", toolBox, LatticeParameters<double>());
  tdd.verify(mySU2(3_c).toString() == "allNew_3(x)");

  mySU2 = res;
  const auto &fr3 = mySU2(3_c);
  auto fr3_view = fr3.getLocalNDHostView();

  {
    bool all_true = true;
    NDLoop<NDim>(fr3_view, [&](const auto... idx) {
      all_true &= (fr3_view(idx...) == 24);
      if (!all_true) {
        std::cout << "Mismatch at index: ";
        ((std::cout << idx << " "), ...);
        std::cout << " expected 24 but got " << fr3_view(idx...) << std::endl;
      }
    });
    tdd.verify(all_true);
  }
}

#endif
