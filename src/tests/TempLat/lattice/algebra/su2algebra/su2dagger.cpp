
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/su2dagger.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"

namespace TempLat
{

  struct SU2DaggerTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2DaggerTester::Test(TDDAssertion &tdd)
  {
    auto toolBox = MemoryToolBox<3>::makeShared(16, 1);
    SU2Field<3, float> su2_1("testSU2_1", toolBox);
    SU2Field<3, float> result("resultSU2", toolBox);

    su2_1(1_c) = 1.0f;
    su2_1(2_c) = -1.5f;
    su2_1(3_c) = 2.0f;

    result = dagger(su2_1);

    auto f1view = result(1_c).getLocalNDHostView();
    auto f2view = result(2_c).getLocalNDHostView();
    auto f3view = result(3_c).getLocalNDHostView();

    bool all_true = true;
    for (size_t i = 0; i < f1view.extent(0); ++i)
      for (size_t j = 0; j < f1view.extent(1); ++j)
        for (size_t k = 0; k < f1view.extent(2); ++k) {
          all_true = all_true && (f1view(i, j, k) == -1.f);
          all_true = all_true && (f2view(i, j, k) == 1.5f);
          all_true = all_true && (f3view(i, j, k) == -2.f);

          if (!all_true) {
            std::cout << "Failed at index " << i << " " << j << " " << k << "\n";
            std::cout << "Values are: " << f1view(i, j, k) << " " << f2view(i, j, k) << " " << f3view(i, j, k) << "\n";
          }
        }
    tdd.verify(all_true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2DaggerTester> test;
}
