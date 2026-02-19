/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/complexalgebra/imag.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/util/ndloop.h"

namespace TempLat
{

  struct ImagTester {
    static void Test(TDDAssertion &tdd);
  };

  void ImagTester::Test(TDDAssertion &tdd)
  {
    struct MyStruct {
      DEVICE_FORCEINLINE_FUNCTION
      double ComplexFieldGet(Tag<0> t) { return 87; };
      DEVICE_FORCEINLINE_FUNCTION
      int ComplexFieldGet(Tag<1> t) { return 870; };
    };

    MyStruct ms;
    tdd.verify(Imag(ms) == 870);
    tdd.verify(Imag(complex<double>(1, 870)) == 870);

    // Test whether it works with complex fields.
    constexpr size_t NDim = 2;
    using T = double;
    ptrdiff_t nGrid = 16, nGhost = 2;
    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);
    toolBox->setVerbose();

    ComplexField<NDim, T> fa("a", toolBox);
    Field<NDim, T> fb("b", toolBox);
    Field<NDim, T> fc("c", toolBox);

    fa = Complexify(3.6, 4.8);
    fb = Imag(fa);

    // fa
    {
      auto viewRe = fa.ComplexFieldGet(0_c).getLocalNDHostView();
      auto viewIm = fa.ComplexFieldGet(1_c).getLocalNDHostView();
      bool all_correct = true;
      NDLoop<NDim>(viewRe, [&](const auto... idx) {
        bool this_correct = true;
        this_correct &= AlmostEqual(viewRe(idx...), 3.6);
        this_correct &= AlmostEqual(viewIm(idx...), 4.8);
        if (!this_correct) {
          std::cout << "ComplexField operation test failed at index (";
          ((std::cout << idx << ", "), ...);
          std::cout << ") got (" << viewRe(idx...) << ", " << viewIm(idx...) << "), expected (" << 3.0 << ", " << 4.0
                    << ")\n";
        }
        all_correct = all_correct && this_correct;
      });
      tdd.verify(all_correct);
    }

    // fb
    {
      auto view = fb.getLocalNDHostView();
      bool all_correct = true;
      NDLoop<NDim>(view, [&](const auto... idx) {
        bool this_correct = true;
        this_correct &= AlmostEqual(view(idx...), 4.8);
        if (!this_correct) {
          std::cout << "real test failed at index (";
          ((std::cout << idx << ", "), ...);
          std::cout << ") got " << view(idx...) << ", expected (" << 4.8 << ")\n";
        }
        all_correct = all_correct && this_correct;
      });
      tdd.verify(all_correct);
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ImagTester> test;
}
