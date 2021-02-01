#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_TEST_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2021

inline void TempLat::SU2LieAlgebraFieldTester::Test(TempLat::TDDAssertion& tdd) {

    auto toolBox = MemoryToolBox::makeShared(3, 32, 1);



    Field<double> f1("myField1",toolBox);
    Field<double> f2("myField2",toolBox);
    Field<double> f3("myField3",toolBox);

    auto res = SU2Field<double>(f1,f2,f3);


    tdd.verify( res.SU2Get(2_c).toString() == "myField2(x)" );

    res(3_c) = 24;

    auto ff3 = res(3_c);

    auto it = toolBox->itX();
    it.begin();
    tdd.verify(ff3.get(it()) == 24);

    SU2Field<double> mySU2("pimpin", toolBox, LatticeParameters<double>());

    tdd.verify(mySU2(3_c).toString() == "pimpin_3(x)" );


}

#endif
