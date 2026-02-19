
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/complexalgebra/complexfieldmultiply.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/util/ndloop.h"
#include "TempLat/lattice/algebra/listoperators/vectordotter.h"

namespace TempLat
{

  struct ComplexFieldMultiplyTester {
    static void Test(TDDAssertion &tdd);
  };

  void ComplexFieldMultiplyTester::Test(TDDAssertion &tdd)
  {
    struct MyStruct {
      DEVICE_FORCEINLINE_FUNCTION
      int ComplexFieldGet(Tag<0> t) const { return 1; }
      DEVICE_FORCEINLINE_FUNCTION
      int ComplexFieldGet(Tag<1> t) const { return 2; }
    };
    struct MyStruct2 {
      DEVICE_FORCEINLINE_FUNCTION
      int ComplexFieldGet(Tag<0> t) const { return 3; }
      DEVICE_FORCEINLINE_FUNCTION
      int ComplexFieldGet(Tag<1> t) const { return 4; }
    };

    auto test = MyStruct() * MyStruct2();
    auto test2 = complex<double>(1, 2) * MyStruct2();

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(test.ComplexFieldGet(0_c) == -5);
    tdd.verify(test.ComplexFieldGet(1_c) == 10);
    tdd.verify(test2.ComplexFieldGet(0_c) == -5);
    tdd.verify(test2.ComplexFieldGet(1_c) == 10);

    {
      // Test whether norm works.
      constexpr size_t NDim = 3;
      using T = double;
      ptrdiff_t nGrid = 16, nGhost = 2;
      auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);
      toolBox->setVerbose();

      ComplexField<NDim, T> fa("a", toolBox);
      Field<NDim, T> fb("b", toolBox);
      Field<NDim, T> fc("c", toolBox);

      fa = Complexify(3.8, 9.1);
      fb = norm2(fa);
      fc = norm(fa);

      // fa
      {
        auto viewRe = fa.ComplexFieldGet(0_c).getLocalNDHostView();
        auto viewIm = fa.ComplexFieldGet(1_c).getLocalNDHostView();
        bool all_correct = true;
        NDLoop<NDim>(viewRe, [&](const auto... idx) {
          bool this_correct = true;
          this_correct &= AlmostEqual(viewRe(idx...), 3.8);
          this_correct &= AlmostEqual(viewIm(idx...), 9.1);
          if (!this_correct) {
            std::cout << "ComplexField operation test failed at index (";
            ((std::cout << idx << ", "), ...);
            std::cout << ") got (" << viewRe(idx...) << ", " << viewIm(idx...) << "), expected (" << 3.8 << ", " << 9.1
                      << ")\n";
          }
          all_correct = all_correct && this_correct;
        });
        tdd.verify(all_correct);
      }

      // fb
      {
        auto view = fb.getLocalNDHostView();
        auto viewc = fc.getLocalNDHostView();
        bool all_correct = true;
        NDLoop<NDim>(view, [&](const auto... idx) {
          bool this_correct = true;
          this_correct &= AlmostEqual(view(idx...), (3.8 * 3.8 + 9.1 * 9.1));
          if (!this_correct) {
            std::cout << "real test failed at index (";
            ((std::cout << idx << ", "), ...);
            std::cout << ") got " << view(idx...) << ", expected (" << (3.8 * 3.8 + 9.1 * 9.1) << ")\n";
          }

          this_correct &= AlmostEqual(viewc(idx...), sqrt(3.8 * 3.8 + 9.1 * 9.1));
          if (!this_correct) {
            std::cout << "real test failed at index (";
            ((std::cout << idx << ", "), ...);
            std::cout << ") got " << viewc(idx...) << ", expected (" << sqrt(3.8 * 3.8 + 9.1 * 9.1) << ")\n";
          }

          all_correct = all_correct && this_correct;
        });
        tdd.verify(all_correct);
      }
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ComplexFieldMultiplyTester> test;
}
