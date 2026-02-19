
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/complexfieldsu2doubletmultiply.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2doublet.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"

namespace TempLat
{

  struct ComplexFieldSU2DoubletMultiplyTester {
    static void Test(TDDAssertion &tdd);
  };

  void ComplexFieldSU2DoubletMultiplyTester::Test(TDDAssertion &tdd)
  {
    // Test the SU2Doublet class
    auto toolBox = MemoryToolBox<3>::makeShared(16, 1);
    ComplexField<3, float> cfield("testCField", toolBox);
    SU2Doublet<3, float> doublet2("testDoublet2", toolBox);
    SU2Doublet<3, float> doublet3("testDoublet3", toolBox);

    cfield(0_c) = 1.0f;
    cfield(1_c) = 2.0f;

    doublet2(0_c) = 1.0f;
    doublet2(1_c) = 2.0f;
    doublet2(2_c) = 3.0f;
    doublet2(3_c) = 4.0f;

    doublet3 = cfield * doublet2;

    auto f0view = doublet3(0_c).getLocalNDHostView();
    auto f1view = doublet3(1_c).getLocalNDHostView();
    auto f2view = doublet3(2_c).getLocalNDHostView();
    auto f3view = doublet3(3_c).getLocalNDHostView();

    bool all_true = true;
    for (size_t i = 0; i < f0view.extent(0); ++i)
      for (size_t j = 0; j < f0view.extent(1); ++j)
        for (size_t k = 0; k < f0view.extent(2); ++k) {
          all_true = all_true && (f0view(i, j, k) == -3.0f);
          all_true = all_true && (f1view(i, j, k) == 4.0f);
          all_true = all_true && (f2view(i, j, k) == -5.0f);
          all_true = all_true && (f3view(i, j, k) == 10.0f);

          if (!all_true) {
            std::cout << "Failed at index " << i << " " << j << " " << k << "\n";
            std::cout << "Values are: " << f0view(i, j, k) << " " << f1view(i, j, k) << " " << f2view(i, j, k) << " "
                      << f3view(i, j, k) << "\n";
          }
        }
    tdd.verify(all_true);
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ComplexFieldSU2DoubletMultiplyTester> test;
}
