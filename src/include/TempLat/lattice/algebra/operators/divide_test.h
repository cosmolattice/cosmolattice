#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIVIDE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIVIDE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/almostequal.h"

inline void TempLat::DivideTester::Test(TempLat::TDDAssertion& tdd) {
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
      myClass b(4);

    //    say << mAdd.get(0, NULL) << " " << GetCPPTypeName::get(decltype(mAdd.get(0, NULL))) << "\n";
    //    say << HasGetMethod<Operators::Add<GetterGetOffset, GetterGetOffset>>::value << "\n";
    tdd.verify( HasGetMethod<Operators::Division<myClass,myClass>>::value == true );


    tdd.verify( AlmostEqual((a/b).get(0) , 0.75 ) );
    //
    int c = 3, d = 4;
    tdd.verify( HasGetMethod<decltype(c / d)>::value == false );

}

#endif
