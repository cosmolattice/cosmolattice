#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/field/field.h"

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

  class MyClass2
  {
  public:
    MyClass2(int b) : a(b) {}

    auto get(ptrdiff_t i, ptrdiff_t j, ptrdiff_t k) const { return a; }

  private:
    int a;
  };

  sqrt(MyClass2(1)).get(1, 2, 3); // should compile

  using A = TempLat::Operators::Multiplication<
      TempLat::Operators::Cosine<TempLat::Operators::Multiplication<double, TempLat::Field<3UL, double>>>,
      TempLat::ExpressionShifterByOne<TempLat::Field<3UL, double>, 1>>;

  using B = TempLat::Operators::Multiplication<
      TempLat::Operators::Sine<TempLat::Operators::Multiplication<double, TempLat::Field<3UL, double>>>,
      TempLat::ExpressionShifterByOne<TempLat::Field<3UL, double>, 1>>;

  tdd.verify(ConditionalBinaryGetter<A, B> == true);

  static_assert(ConditionalBinaryGetter<A, B>);
}

#endif
