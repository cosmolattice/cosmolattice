
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionresult.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/log/saycomplete.h"
#include "TempLat/util/powr.h"
#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{

  template <typename T> struct RadialProjectionResultTester {
    static void Test(TDDAssertion &tdd);
  };

  template <typename T> inline void RadialProjectionResultTester<T>::Test(TDDAssertion &tdd)
  {
    RadialProjectionResult one(10), two(12), three(10);

    // tdd.verify(Throws<RadialProjectionResultSizeException>([&]() { one += two; }));

    /* dummy data */
    // for (ptrdiff_t i = 0, iEnd = three.size(); i < iEnd; ++i) {
    //   three.add(i, 2 * i, 2 * i);
    // }

    int size = 1, rank = 0;
#ifdef HAVE_MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

    device::iteration::foreach<1>(
        "RadialProjectionResultTest", {0}, {10}, DEVICE_LAMBDA(const device::IdxArray<1> i) {
          three.add_device(i[0], 2 * i[0], 2 * i[0]);
          three.add_device(i[0], 2 * i[0] + 0.5, 2 * i[0]);
          three.add_device(i[0], 2 * i[0] - 0.5, 2 * i[0]);
        });
    three.finalize(MPICommReference());
    tdd.verify(three.size() == 10);
    {
      bool allRight = true;
      for (ptrdiff_t i = 0, iEnd = three.size(); i < iEnd; ++i) {
        const double expectedVariance =
            abs(powr<2>(2. * i) * 2. / 3. - powr<2>(2 * i + 0.5) / 3. - powr<2>(2 * i - 0.5) / 3.);
        allRight = allRight && AlmostEqual(three[i].getValue().average, 2 * i);
        allRight = allRight && AlmostEqual(three[i].getValue().sampleVariance, expectedVariance);
        allRight = allRight && three[i].getValue().multiplicity == 3 * size;
        if (!allRight) {
          say << "Bin " << i << ": "
              << "Average: " << three[i].getValue().average
              << ", Sample Variance: " << three[i].getValue().sampleVariance
              << ", Multiplicity: " << three[i].getValue().multiplicity << "\n"
              << "Expected: "
              << "Average: " << 2 * i << ", Sample Variance: " << expectedVariance << ", Multiplicity: " << 3 * size
              << "\n";

          break;
        }
      }
      tdd.verify(allRight);
    }
    /*
      one += three;
      one += three;

      bool allRight = true;
      tdd.verify(one.size() == three.size());

      for (ptrdiff_t i = 0, iEnd = one.size(); i < iEnd; ++i) {
        allRight = allRight && one[i].getValue().average == 2 * three.mValues.mAverages[i];
        allRight = allRight && one[i].getValue().sampleVariance == 2 * three.mValues.mVariances[i];
        allRight = allRight && one[i].getValue().multiplicity == 2 * three.mMultiplicities[i];

        say << "Bin " << i << ": "
            << "Average: " << one[i].getValue().average << ", Sample Variance: " << one[i].getValue().sampleVariance
            << ", Multiplicity: " << one[i].getValue().multiplicity << "\n";
      }

      tdd.verify(allRight);
    */
    /* test that this compiles */
    for (auto &&it : one) {
      it.getValue().average *= 1;
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::RadialProjectionResultTester<double>> test;
}
