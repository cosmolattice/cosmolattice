#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


namespace TempLat {
    namespace TestScratch {
        template <typename T, typename S>
        struct BinaryDummy {
        };
    }
}

inline void TempLat::ConditionalBinaryGetterTester::Test(TempLat::TDDAssertion& tdd) {

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


    typedef typename ConditionalBinaryGetter<
        TestScratch::BinaryDummy,
        MyClass,
        MyClass
    >::type templatedType;

    typedef TestScratch::BinaryDummy<MyClass, MyClass> directType;

    /* .verify is actually a preprocessor function, to add a string version of the command to the output.
      This doesn't play well with template instances, due to the comma's! */
    tdd.verify( std::is_same< templatedType, directType  >::value );

    tdd.verify( std::is_same< typename ConditionalBinaryGetter<
        TestScratch::BinaryDummy,
        MyClass,
        MyClass
    >::type, TestScratch::BinaryDummy<MyClass, MyClass>  >::value );


    typedef typename ConditionalBinaryGetter<
        TestScratch::BinaryDummy,
        MyClass,
        double
    >::type templatedType2;

    typedef TestScratch::BinaryDummy<MyClass, double> directType2;

    /* .verify is actually a preprocessor function, to add a string version of the command to the output.
      This doesn't play well with template instances, due to the comma's! */
    tdd.verify( std::is_same< templatedType2, directType2  >::value );

    typedef typename ConditionalBinaryGetter<
        TestScratch::BinaryDummy,
        double,
        MyClass
    >::type templatedType3;

    typedef TestScratch::BinaryDummy<double, MyClass> directType3;

    /* .verify is actually a preprocessor function, to add a string version of the command to the output.
      This doesn't play well with template instances, due to the comma's! */
    tdd.verify( std::is_same< templatedType3, directType3  >::value );



//     second test: this should not compile:
//    typedef typename ConditionalBinaryGetter<
//        TestScratch::BinaryDummy,
//        double,
//        double
//    >::type templatedType4;
    // passed.

//   third test: should not compile either

    class MyVecClass{
    public:
      MyVecClass(int b):a(b){}

      auto get(ptrdiff_t i)
      {
        return a;
      }
      auto vectorGet(ptrdiff_t i,ptrdiff_t j)
      {
        return a;
      }

    private:
      int a;
    };

//        typedef typename ConditionalBinaryGetter<
//            TestScratch::BinaryDummy,
//            MyVecClass,
//            MyVecClass
//        >::type templatedType5;
//passed
}

#endif
