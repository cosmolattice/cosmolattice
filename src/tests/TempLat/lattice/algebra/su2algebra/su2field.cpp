/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/ndloop.h"

namespace TempLat {

template<size_t NDim, typename T>
struct SU2FieldBaseTester {
  static void Test(TDDAssertion &tdd);
};

template <size_t NDim, typename T> void SU2FieldBaseTester<NDim, T>::Test(TDDAssertion &tdd)
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

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2FieldBaseTester<1, double>> test5;
  TempLat::TDDContainer<TempLat::SU2FieldBaseTester<2, double>> test6;
  TempLat::TDDContainer<TempLat::SU2FieldBaseTester<3, double>> test7;
  TempLat::TDDContainer<TempLat::SU2FieldBaseTester<4, double>> test8;
} // namespace
