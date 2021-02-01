#ifndef TEMPLAT_LATTICE_ALGEBRA_MULTIPLY_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_MULTIPLY_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019



inline void TempLat::MultiplyTester::Test(TempLat::TDDAssertion& tdd) {

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

    myClass c(3);
    myClass b(2);

    //    say << mAdd.get(0, NULL) << " " << GetCPPTypeName::get(decltype(mAdd.get(0, NULL))) << "\n";
    //    say << HasGetMethod<Operators::Add<GetterGetOffset, GetterGetOffset>>::value << "\n";
    tdd.verify( HasGetMethod<Operators::Multiplication<myClass,myClass>>::value == true );
    tdd.verify( (b*c).get(0) == 6 );
    //
    int e = 3, f = 4;
    tdd.verify( HasGetMethod<decltype(e * f)>::value == false );


}

#endif
