
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/su2doublet.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct SU2DoubletTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2DoubletTester::Test(TDDAssertion &tdd)
  {
    // Test the SU2Doublet class
    auto toolBox = MemoryToolBox<3>::makeShared(16, 1);
    SU2Doublet<3, float> doublet("testDoublet", toolBox);
    SU2Doublet<3, float> doublet2("testDoublet2", toolBox);

    doublet2(0_c) = 1.0f;
    doublet2(1_c) = 2.0f;
    doublet2(2_c) = 3.0f;
    doublet2(3_c) = 4.0f;

    doublet = doublet2;

    auto f0view = doublet(0_c).getLocalNDHostView();
    auto f1view = doublet(1_c).getLocalNDHostView();
    auto f2view = doublet(2_c).getLocalNDHostView();
    auto f3view = doublet(3_c).getLocalNDHostView();

    bool all_true = true;
    for (size_t i = 0; i < f0view.extent(0); ++i)
      for (size_t j = 0; j < f0view.extent(1); ++j)
        for (size_t k = 0; k < f0view.extent(2); ++k) {
          all_true = all_true && (f0view(i, j, k) == 1.0f);
          all_true = all_true && (f1view(i, j, k) == 2.0f);
          all_true = all_true && (f2view(i, j, k) == 3.0f);
          all_true = all_true && (f3view(i, j, k) == 4.0f);

          if (!all_true) {
            std::cout << "Failed at index " << i << " " << j << " " << k << "\n";
            std::cout << "Values are: " << f0view(i, j, k) << " " << f1view(i, j, k) << " " << f2view(i, j, k) << " "
                      << f3view(i, j, k) << "\n";
          }
        }
    tdd.verify(all_true);

    std::vector<SU2Doublet<3, double>> fs;
    for (int i = 0; i < 4; ++i) {
      fs.push_back(SU2Doublet<3, double>("dd_" + std::to_string(i + 0), toolBox));
    }

    std::vector<SU2Doublet<3, double>> fs2(fs);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2DoubletTester> test;
}
