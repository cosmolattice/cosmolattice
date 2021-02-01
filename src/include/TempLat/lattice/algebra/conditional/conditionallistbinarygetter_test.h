#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERVECTOR_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERVECTOR_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

namespace TempLat {
    namespace TestScratch {
        template <typename T, typename S>
        struct BinaryDummy3 {
        };
    }
}

inline void TempLat::ConditionalListBinaryGetterTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    class MyClass{
    public:
      MyClass(int b):a(b){}

      auto vectorGet(ptrdiff_t i, ptrdiff_t j)
      {
        return a;
      }

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

#endif
