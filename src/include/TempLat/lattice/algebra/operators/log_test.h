#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LOG_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LOG_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::LogTester::Test(TempLat::TDDAssertion& tdd) {

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

    tdd.verify( log(a).get(0) == std::log(3) );
  //  std::cerr << log(a).d(a).toString() << "\n";

    /* Default is to fail: to remind yourself to implement something here. */
//    tdd.verify( true );

}

#endif
