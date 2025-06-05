#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_TANH_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_TANH_TEST_H
#include <Kokkos_Macros.hpp>

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

inline void TempLat::TanhTester::Test(TempLat::TDDAssertion& tdd) {

    class myClass{
    public:
        KOKKOS_FUNCTION
        myClass(int b):a(b){}

        KOKKOS_FORCEINLINE_FUNCTION
        auto get(const double& i)
        {
            return a;
        }

    private:
        double a;
    };

    myClass a(3);
    //myClass b(4);
    say << tanh(a).get(0) << "\n";
    tdd.verify( AlmostEqual(tanh(a).get(0),std::tanh(3.))  );
}

#endif
