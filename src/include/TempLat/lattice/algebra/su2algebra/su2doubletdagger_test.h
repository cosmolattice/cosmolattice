#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETDAGGER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETDAGGER_TEST_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Year: 2025

inline void TempLat::SU2DoubletDaggerTester::Test(TempLat::TDDAssertion& tdd) {

    struct MySU2Doublet{
        MySU2Doublet(double i, double j, double k, double l) : arr{i,j,k,l}{}

        int SU2DoubletGet(Tag<0> t)
        {
            return arr[0];
        }
        int SU2DoubletGet(Tag<1> t)
        {
            return arr[1];
        }
        int SU2DoubletGet(Tag<2> t)
        {
            return arr[2];
        }
        int SU2DoubletGet(Tag<3> t)
        {
            return arr[3];
        }

        int SU2DoubletGet(Tag<0> t, ptrdiff_t i)
        {
            return arr[0];
        }
        int SU2DoubletGet(Tag<1> t, ptrdiff_t i)
        {
            return arr[1];
        }
        int SU2DoubletGet(Tag<2> t, ptrdiff_t i)
        {
            return arr[2];
        }
        int SU2DoubletGet(Tag<3> t, ptrdiff_t i)
        {
            return arr[3];
        }

        std::string toString() const{
            return "test";
        }

        std::vector<double> arr;
    };

    auto test = MySU2Doublet(1,2,3,4);

    auto dag = dagger(test);

    tdd.verify( dag.SU2DoubletGet(0_c) == 1 );
    tdd.verify( dag.SU2DoubletGet(1_c) == -2 );
    tdd.verify( dag.SU2DoubletGet(2_c) == 3 );
    tdd.verify( dag.SU2DoubletGet(3_c) == -4 );

    tdd.verify( dag.SU2DoubletGet(0_c, 22) == 1 );
    tdd.verify( dag.SU2DoubletGet(1_c, 22) == -2 );
    tdd.verify( dag.SU2DoubletGet(2_c, 22) == 3 );
    tdd.verify( dag.SU2DoubletGet(3_c, 22) == -4 );

}

#endif
