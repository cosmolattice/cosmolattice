#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

namespace TempLat
{
  namespace TestScratch
  {
    template <typename T, typename S> struct BinaryDummy {
    };
  } // namespace TestScratch
} // namespace TempLat

inline void TempLat::ConditionalBinaryGetterTester::Test(TempLat::TDDAssertion &tdd)
{
  class MyClass
  {
  public:
    MyClass(int b) : a(b) {}

    auto get(ptrdiff_t i) { return a; }

  private:
    int a;
  };

  /* .verify is actually a preprocessor function, to add a string version of the command to the output.
    This doesn't play well with template instances, due to the comma's! */
  tdd.verify(ConditionalBinaryGetter<MyClass, MyClass> == true);

  /* .verify is actually a preprocessor function, to add a string version of the command to the output.
    This doesn't play well with template instances, due to the comma's! */
  tdd.verify(ConditionalBinaryGetter<MyClass, double> == true);

  tdd.verify(ConditionalBinaryGetter<double, double> == false);

  //     second test: this should not compile:
  //    typedef typename ConditionalBinaryGetter<
  //        TestScratch::BinaryDummy,
  //        double,
  //        double
  //    >::type templatedType4;
  // passed.

  //   third test: should not compile either

  class MyVecClass
  {
  public:
    MyVecClass(int b) : a(b) {}

    auto get(ptrdiff_t i) { return a; }
    auto vectorGet(ptrdiff_t i, ptrdiff_t j) { return a; }

  private:
    int a;
  };

  //        typedef typename ConditionalBinaryGetter<
  //            TestScratch::BinaryDummy,
  //            MyVecClass,
  //            MyVecClass
  //        >::type templatedType5;
  // passed
}

#endif
