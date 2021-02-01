#ifndef TEMPLAT_LATTICE_ALGEBRA_ADD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_ADD_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/getcpptypename.h"

inline void TempLat::AddTester::Test(TempLat::TDDAssertion& tdd) {

//    GetterGetOffset a, b;
//    Operators::Add<GetterGetOffset, GetterGetOffset> mAdd(a, b);

      class myClass{
      public:
        myClass(int b):a(b){}

        auto get(ptrdiff_t i)
        {
          return a;
        }

      private:
        int a;
      };

      class myClass2{
      public:
        myClass2(int b):a(b){}

        double get(ptrdiff_t i)
        {
          return a;
        }

        auto getDx() const
        {
          return 2.89;
        }

        auto getKIR() const
        {
          return 9.89;
        }

      private:
        int a;
      };

      myClass a(3);
      myClass b(4);
      myClass2 e(4);

//    say << mAdd.get(0, NULL) << " " << GetCPPTypeName::get(decltype(mAdd.get(0, NULL))) << "\n";
//    say << HasGetMethod<Operators::Add<GetterGetOffset, GetterGetOffset>>::value << "\n";
    tdd.verify( HasGetMethod<Operators::Addition<myClass,myClass>>::value == true );
    tdd.verify( (a+b).get(0) == 7 );
    tdd.verify( (a+b).getDx() == 1 );
    tdd.verify( (a+b).getKIR() == 1 );
    tdd.verify( (a+e).getDx() == 2.89 );
    tdd.verify( (a+e).getKIR() == 9.89 );
  //
    int c = 3, d = 4;
    tdd.verify( HasGetMethod<decltype(c + d)>::value == false );

}

#endif
