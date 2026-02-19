
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/su2algebra/scalarsu2multiplication.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/util/ndloop.h"

namespace TempLat
{

  template <size_t NDim, typename T> struct ComplexFieldSU2MultiplyTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim, typename T> inline void ComplexFieldSU2MultiplyTester<NDim, T>::Test(TDDAssertion &tdd)
  {
    auto toolBox = MemoryToolBox<NDim>::makeShared(16, 1);
    Field<NDim, T> field("testField", toolBox);
    SU2Field<NDim, T> su2("testSU2", toolBox);
    SU2Field<NDim, T> result("testResultSU2", toolBox);

    field = -3;

    su2(1_c) = 1;
    su2(2_c) = 1.5;
    su2(3_c) = 2;

    result = field * su2;

    auto f1view = result(1_c).getLocalNDHostView();
    auto f2view = result(2_c).getLocalNDHostView();
    auto f3view = result(3_c).getLocalNDHostView();

    bool all_true = true;
    NDLoop<NDim>(f1view, [&](const auto &...idx) {
      all_true = all_true && AlmostEqual(f1view(idx...), T(-3));
      all_true = all_true && AlmostEqual(f2view(idx...), T(-4.5));
      all_true = all_true && AlmostEqual(f3view(idx...), T(-6));

      if (!all_true) {
        std::cout << "Failed at index " << std::vector<device::Idx>{{idx...}} << "\n";
        std::cout << "Values are: " << f1view(idx...) << " " << f2view(idx...) << " " << f3view(idx...) << "\n";
      }
    });
    tdd.verify(all_true);
  }

} // namespace TempLat

namespace
{
  // TempLat::TDDContainer<TempLat::ComplexFieldSU2MultiplyTester<1, double>> test1;
  TempLat::TDDContainer<TempLat::ComplexFieldSU2MultiplyTester<2, double>> test2;
  TempLat::TDDContainer<TempLat::ComplexFieldSU2MultiplyTester<3, double>> test3;
  TempLat::TDDContainer<TempLat::ComplexFieldSU2MultiplyTester<4, double>> test4;
  // TempLat::TDDContainer<TempLat::ComplexFieldSU2MultiplyTester<1, float>> test1f;
  // TempLat::TDDContainer<TempLat::ComplexFieldSU2MultiplyTester<2, float>> test2f;
  // TempLat::TDDContainer<TempLat::ComplexFieldSU2MultiplyTester<3, float>> test3f;
  // TempLat::TDDContainer<TempLat::ComplexFieldSU2MultiplyTester<4, float>> test4f;
} // namespace
