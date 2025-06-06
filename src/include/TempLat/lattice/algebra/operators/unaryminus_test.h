#ifndef TEMPLAT_LATTICE_ALGEBRA_UNARYMINUS_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_UNARYMINUS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::UnaryMinusTester::Test(TempLat::TDDAssertion& tdd) {


  class myClass{
  public:
    KOKKOS_FUNCTION
    myClass(int b):a(b){}

    KOKKOS_FORCEINLINE_FUNCTION
    auto get(const double& i) const
    {
      return a;
    }

  private:
    double a;
  };

    myClass a(4);
    //myClass b(4);
    tdd.verify( AlmostEqual((-a).get(0),-4) );

}

#endif
