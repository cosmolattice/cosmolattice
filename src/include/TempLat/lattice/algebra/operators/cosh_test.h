#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COSH_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COSH_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

inline void TempLat::CoshTester::Test(TempLat::TDDAssertion& tdd) {

    class myClass{
    public:
        myClass(int b):a(b){}

        auto get(const double& i)
        {
            return a;
        }

    private:
        double a;
    };

    myClass a(3);
    //myClass b(4);
    say << cosh(a).get(0) << "\n";
    tdd.verify( AlmostEqual(cosh(a).get(0),std::cosh(3.))  );
}

#endif
