
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/su2multiply.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/util/ndloop.h"
#include <limits>

namespace TempLat
{

  struct SU2MultiplyTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2MultiplyTester::Test(TDDAssertion &tdd)
  {
    static constexpr size_t NDim = 3;
    using T = float;

    auto toolBox = MemoryToolBox<3>::makeShared(8, 1);
    SU2Field<NDim, T> su2_1("testSU2_1", toolBox);
    SU2Field<NDim, T> su2_2("testSU2_2", toolBox);
    SU2Field<NDim, T> result("resultSU2", toolBox);

    su2_1(1_c) = 0.1;
    su2_1(2_c) = 0.2;
    su2_1(3_c) = 0.3;
    su2_1.unitarize();

    su2_2(1_c) = 0.4;
    su2_2(2_c) = 0.5;
    su2_2(3_c) = 0.6;
    su2_2.unitarize();

    result = su2_1 * su2_2;

    auto f1view = result(1_c).getLocalNDHostView();
    auto f2view = result(2_c).getLocalNDHostView();
    auto f3view = result(3_c).getLocalNDHostView();

    {
      bool all_true = true;
      NDLoop<NDim>(f1view, [&](const auto... idx) {
        bool this_true = true;
        this_true &= AlmostEqual(f1view(idx...), (T)0.44890305505295536, sqrt(std::numeric_limits<T>::epsilon()));
        this_true &= AlmostEqual(f2view(idx...), (T)0.49959755524103955, sqrt(std::numeric_limits<T>::epsilon()));
        this_true &= AlmostEqual(f3view(idx...), (T)0.7302920554291238, sqrt(std::numeric_limits<T>::epsilon()));
        // Checked just in Mathematica

        if (!this_true) {
          std::cout << "Mismatch at index: ";
          ((std::cout << idx << " "), ...);
          std::cout << " got " << f1view(idx...) << " " << f2view(idx...) << " " << f3view(idx...);
          std::cout << " expected " << (T)0.44890305505295536 << " " << (T)0.49959755524103955 << " "
                    << (T)0.7302920554291238 << "\n";
        }
        all_true &= this_true;
      });
      tdd.verify(all_true);
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2MultiplyTester> test;
}
