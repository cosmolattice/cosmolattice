#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONRESULT_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONRESULT_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

template <typename T> inline void TempLat::RadialProjectionResult<T>::Test(TempLat::TDDAssertion &tdd)
{

  RadialProjectionResult one(10), two(12), three(10);

  // tdd.verify(Throws<RadialProjectionResultSizeException>([&]() { one += two; }));

  /* dummy data */
  // for (ptrdiff_t i = 0, iEnd = three.size(); i < iEnd; ++i) {
  //   three.add(i, 2 * i, 2 * i);
  // }

  Kokkos::parallel_for(
      "RadialProjectionResultTest", Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>(0, 10),
      KOKKOS_LAMBDA(const ptrdiff_t i) { three.add_device(i, 2 * i, 2 * i); });
  three.finalize(MPICommReference());
  tdd.verify(three.size() == 10);
  {
    bool allRight = true;
    for (ptrdiff_t i = 0, iEnd = three.size(); i < iEnd; ++i) {
      allRight = allRight && three[i].getValue().average == 2 * i;
      allRight = allRight && three[i].getValue().sampleVariance == 0; // no variance in this case
      allRight = allRight && three[i].getValue().multiplicity == 1;
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

#endif
