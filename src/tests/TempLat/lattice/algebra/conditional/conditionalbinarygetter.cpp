
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/field/field.h"

namespace TempLat
{

  namespace TempLat
  {
    namespace TestScratch
    {
      template <typename T, typename S> struct BinaryDummy {
      };
    } // namespace TestScratch
  } // namespace TempLat

  struct ConditionalBinaryGetterTester {
    static void Test(TDDAssertion &tdd);
  };

  void ConditionalBinaryGetterTester::Test(TDDAssertion &tdd)
  {
    class MyClass
    {
    public:
      MyClass(int b) : a(b) {}

      auto get(ptrdiff_t i) { return a; }
      auto eval(ptrdiff_t i) { return a; }

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
      auto eval(ptrdiff_t i, ptrdiff_t j, ptrdiff_t k) const { return a; }

    private:
      int a;
    };

    sqrt(MyClass2(1)).eval(1, 2, 3); // should compile

    using A = Operators::Multiplication<Operators::Cosine<Operators::Multiplication<double, Field<3UL, double>>>,
                                        ExpressionShifterByOne<Field<3UL, double>, 1>>;

    using B = Operators::Multiplication<Operators::Sine<Operators::Multiplication<double, Field<3UL, double>>>,
                                        ExpressionShifterByOne<Field<3UL, double>, 1>>;

    tdd.verify(ConditionalBinaryGetter<A, B> == true);

    static_assert(ConditionalBinaryGetter<A, B>);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ConditionalBinaryGetterTester> test;
}
