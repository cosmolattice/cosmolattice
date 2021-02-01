#ifndef TEMPLAT_LATTICE_ALGEBRA_SUBTRACT_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SUBTRACT_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::SubtractTester::Test(TempLat::TDDAssertion& tdd) {

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

  myClass a(3);
  myClass b(4);

//    say << mAdd.get(0, NULL) << " " << GetCPPTypeName::get(decltype(mAdd.get(0, NULL))) << "\n";
//    say << HasGetMethod<Operators::Add<GetterGetOffset, GetterGetOffset>>::value << "\n";
tdd.verify( HasGetMethod<Operators::Substraction<myClass,myClass>>::value == true );
tdd.verify( (a-b).get(0) == -1 );
//
int c = 3, d = 4;
tdd.verify( HasGetMethod<decltype(c - d)>::value == false );

}

#endif
