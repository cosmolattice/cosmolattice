
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/measuringtools/radialprojector.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"
#include "TempLat/lattice/field/field.h"

namespace TempLat
{

  struct RadialProjectorTester {
    static void Test(TDDAssertion &tdd);
  };

  void RadialProjectorTester::Test(TDDAssertion &tdd)
  {
    static constexpr size_t NDim = 3;
    const ptrdiff_t nGrid = 32;
    const ptrdiff_t nGhost = 1;

    auto mToolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    Field<NDim, double> phi("phi", mToolBox);
    auto phi_of_k = phi.inFourierSpace();
    const double mFac = 1.;
    phi_of_k = mFac * WaveNumber<NDim>(mToolBox).norm();

    auto rProj = projectRadiallyFourier(abs(phi_of_k));

    auto result = rProj.measure();

    //    say << "result: \n" << result << "\n";

    /// not sure this really tests the radial projection, but at least it tests MPI.
    bool allRight = true;
    size_t total = 0;
    size_t wrong = 0;
    for (auto &&it : result) {
      allRight = allRight && AlmostEqual(mFac * it.getBin().average, it.getValue().average);
      // variance is close to zero, all numerical noise.
      // allRight = allRight && AlmostEqual(3 * it.getBin().sampleVariance, it.getValue().sampleVariance, 1.e-2);
      allRight = allRight && AlmostEqual(mFac * it.getBin().minVal, it.getValue().minVal);
      allRight = allRight && AlmostEqual(mFac * it.getBin().maxVal, it.getValue().maxVal);
      if (!allRight) {
        say << "Broken: " << "\n  Bin: "
            << "    average: " << it.getBin().average << ", min: " << it.getBin().minVal
            << ", max: " << it.getBin().maxVal << "\n  Value: "
            << "    average: " << it.getValue().average << ", min: " << it.getValue().minVal
            << ", max: " << it.getValue().maxVal << "\n";

        // break;
        wrong++;
      }
      total++;
    }

    std::cout << "RadialProjectorTester: " << wrong << " out of " << total << " bins were wrong." << std::endl;
    tdd.verify(allRight);
    /*

    // test the rebinning
    result.rebin(10);
    tdd.verify(result.size() == 10u);

    allRight = true;
    for (auto &&it : result) {
      allRight = allRight && AlmostEqual(3 * it.getBin().average, it.getValue().average);
      // variance is close to zero, all numerical noise.
      // allRight = allRight && AlmostEqual(3 * it.getBin().sampleVariance, it.getValue().sampleVariance, 1.e-2);
      if (it.getBin().average != 0) {
        allRight = allRight && AlmostEqual(3 * it.getBin().minVal, it.getValue().minVal);
        allRight = allRight && AlmostEqual(3 * it.getBin().maxVal, it.getValue().maxVal);
      }
      if (!allRight) {
        say << "Broken: " << it << "\n";
        break;
      }
    }
    tdd.verify(allRight);

    // test the rescaling

    result.rescale([](auto x) { return x * x; });
    allRight = true;
    for (auto &&it : result) {
      // variance is close to zero, all numerical noise.
      // allRight = allRight && AlmostEqual(3 * it.getBin().sampleVariance, it.getValue().sampleVariance, 1.e-2);
      if (it.getBin().average != 0) {
        allRight = allRight && AlmostEqual(3 * it.getBin().average,
                                           it.getValue().average / it.getBin().average / it.getBin().average);
        allRight = allRight &&
                   AlmostEqual(3 * it.getBin().minVal, it.getValue().minVal / it.getBin().average /
    it.getBin().average); allRight = allRight && AlmostEqual(3 * it.getBin().maxVal, it.getValue().maxVal /
    it.getBin().average / it.getBin().average);
      }
      if (!allRight) {
        say << "Broken: " << it << "\n";
        break;
      }
    }
    tdd.verify(allRight);
  */
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::RadialProjectorTester> test;
}
