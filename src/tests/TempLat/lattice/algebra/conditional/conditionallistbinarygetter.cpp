
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/conditional/conditionallistbinarygetter.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  namespace TempLat
  {
    namespace TestScratch
    {
      template <typename T, typename S> struct BinaryDummy3 {
      };
    } // namespace TestScratch
  } // namespace TempLat

  struct ConditionalListBinaryGetterTester {
    static void Test(TDDAssertion &tdd);
  };

  void ConditionalListBinaryGetterTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    class MyClass
    {
    public:
      MyClass(int b) : a(b) {}

      auto vectorGet(ptrdiff_t i, ptrdiff_t j) { return a; }

    private:
      int a;
    };

    //    typedef typename ConditionalBinaryGetterVector<
    //       TestScratch::BinaryDummy3,
    //        MyClass,
    //        MyClass
    //    >::type templatedType;

    //    typedef TestScratch::BinaryDummy3<MyClass, MyClass> directType;

    /* .verify is actually a preprocessor function, to add a string version of the command to the output.
      This doesn't play well with template instances, due to the comma's! */
    //   tdd.verify( std::is_same< templatedType, directType  >::value );

    //  typedef typename ConditionalBinaryGetterScalarVector< //should not compile
    //      TestScratch::BinaryDummy3,
    //      MyClass,
    //      double
    //  >::type templatedType2;
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ConditionalListBinaryGetterTester> test;
}
