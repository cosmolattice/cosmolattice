#ifndef TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_ARG_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_ARG_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

inline void TempLat::ArgTester::Test(TempLat::TDDAssertion& tdd) {

    struct MyStructR{
        double get(int i)
        {
            return (double)i / 4.0;
        }
    };

    struct MyStructC{
        MyStructR ComplexFieldGet(Tag<0> t)
        {
            return r;
        }
        MyStructR ComplexFieldGet(Tag<1> t)
        {
            return r;
        }

        MyStructR r;
    };

    auto t1 = MyStructC();
    auto argu = arg(t1.ComplexFieldGet(0_c),t1.ComplexFieldGet(1_c));

    tdd.verify(AlmostEqual(argu.get(0), 0));
    tdd.verify(AlmostEqual(argu.get(1),0.7853981633974483));
    tdd.verify(AlmostEqual(argu.get(-1), 3.926990816987241));


}

#endif
