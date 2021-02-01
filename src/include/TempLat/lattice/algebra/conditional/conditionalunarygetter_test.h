#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALUNARYGETTER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALUNARYGETTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

//#include "TempLat/lattice/algebra/gettergetoffset.h"
//#include "TempLat/lattice/algebra/getter.h"


namespace TempLat {
    namespace TestScratch {
        template <typename T>
        struct UnaryDummy {
        };
    }
}

inline void TempLat::ConditionalUnaryGetterTester::Test(TempLat::TDDAssertion& tdd) {

    class MyClass{
    public:
      MyClass(int b):a(b){}

      auto get(ptrdiff_t i)
      {
        return a;
      }

    private:
      int a;
    };

    typedef typename ConditionalUnaryGetter<
        TestScratch::UnaryDummy,
        MyClass
    >::type templatedType;

    typedef TestScratch::UnaryDummy<MyClass> directType;

    /* .verify is actually a preprocessor function, to add a string version of the command to the output.
      This doesn't play well with template instances, due to the comma's! */
    tdd.verify( std::is_same< templatedType, directType  >::value );

    /* second test: this should not compile: */
//    ConditionalUnaryGetter<
//        TestScratch::UnaryDummy,
//        double
//    >::type (z(go));
    // passed.

}

#endif
