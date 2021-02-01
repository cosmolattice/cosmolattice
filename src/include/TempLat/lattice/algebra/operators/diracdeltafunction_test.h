#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIRACDELTAFUNCTION_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIRACDELTAFUNCTION_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

namespace TempLat {
    namespace TestScratch {
        template <int RETURNVALUE>
        struct DiracTesterTemplate {
            double get(ptrdiff_t i) {
                return RETURNVALUE * std::numeric_limits<double>::epsilon();
            }
        };
    }
}


inline void TempLat::DiracDeltaFunctionTester::Test(TempLat::TDDAssertion& tdd) {


    using namespace TestScratch;
    DiracTesterTemplate<1> positive;
    DiracTesterTemplate<0> zero;
    DiracTesterTemplate<-1> negative;

    /* weird: std::isfinite and std::isinf give the WRONG result in g++ on my machine. So test explicitly. */
    /* not weird. We use -ffast-math. https://stackoverflow.com/a/22931368/2295722 */

//    tdd.verify( ! std::isfinite( DiracDelta(zero).get(pIterCoords) ) );
    tdd.verify( DiracDelta(zero).get(0) > std::numeric_limits<double>::max() );

    tdd.verify( DiracDelta(positive).get(0) == 0 );

    tdd.verify( DiracDelta(negative).get(0) == 0 );

  //  tdd.verify( std::is_same<PositiveInfinityType, decltype(DiracDelta(ZeroType()))>::value );

  //  tdd.verify( std::is_same<ZeroType, decltype(DiracDelta(OneType()))>::value );

}

#endif
