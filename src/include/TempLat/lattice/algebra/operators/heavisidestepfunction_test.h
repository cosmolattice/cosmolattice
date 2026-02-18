#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_HEAVISIDESTEPFUNCTION_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_HEAVISIDESTEPFUNCTION_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

namespace TempLat
{
  namespace TestScratch
  {
    template <int RETURNVALUE> struct HeavisideTesterTemplate {
      DEVICE_FORCEINLINE_FUNCTION
      double get(ptrdiff_t i) const { return RETURNVALUE * std::numeric_limits<double>::epsilon(); }
      DEVICE_FORCEINLINE_FUNCTION
      double eval(ptrdiff_t i) const { return RETURNVALUE * std::numeric_limits<double>::epsilon(); }
    };
  } // namespace TestScratch
} // namespace TempLat

inline void TempLat::HeavisideStepFunctionTester::Test(TempLat::TDDAssertion &tdd)
{
  using namespace TestScratch;
  HeavisideTesterTemplate<1> positive;
  HeavisideTesterTemplate<0> zero;
  HeavisideTesterTemplate<-1> negative;

  tdd.verify(heaviside(zero).eval(0) == 1);

  tdd.verify(heaviside(positive).eval(0) == 1);

  tdd.verify(heaviside(negative).eval(0) == 0);
}

#endif
