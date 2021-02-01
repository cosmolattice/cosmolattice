#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2ALGEBRATESTER_TEST_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2ALGEBRATESTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/lattice/algebra/complexalgebra/real.h"
#include "TempLat/lattice/algebra/complexalgebra/imag.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/lattice/algebra/su2algebra/su2doublet.h"
#include "TempLat/lattice/algebra/su2algebra/su2generators.h"
#include "TempLat/lattice/algebra/operators/operators.h"


using namespace TempLat::Constants;

inline void TempLat::SU2AlgebraTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */

    tdd.verify( HasSU2DoubletGet<decltype(sigma3)>::value == false );
    tdd.verify( HasSU2Get<decltype(sigma3)>::value == true );
    tdd.verify( HasSU2DoubletGet<decltype(sigma1)>::value == false );
    tdd.verify( HasSU2Get<decltype(sigma1)>::value == true );


    auto test1 = i_sigma(1_c) * i_sigma(3_c) * i_sigma(1_c) * i_sigma(3_c);

    tdd.verify( test1.SU2Get(0_c).get(0) == -1 );
    tdd.verify( test1.SU2Get(1_c).get(0) == 0 );
    tdd.verify( test1.SU2Get(2_c).get(0) == 0 );
    tdd.verify( test1.SU2Get(3_c).get(0) == 0 );

    auto test2 = i_sigma(1_c) * i_sigma(3_c);

    tdd.verify( test2.SU2Get(0_c).get(0) == 0 );
    tdd.verify( test2.SU2Get(1_c).get(0) == 0 );
    tdd.verify( test2.SU2Get(2_c).get(0) == 1 );
    tdd.verify( test2.SU2Get(3_c).get(0) == 0 );

    tdd.verify(dagger(i_sigma(3_c)).SU2Get(3_c).get(0) == -1);

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

        std::vector<double> arr;
    };

    auto test3 = scalar_prod(MySU2Doublet(2,0,1,1), dagger(i_sigma(1_c)) * MySU2Doublet(1,1,1,-1));



    tdd.verify(Real(test3).get(0) == -2);
    tdd.verify(Imag(test3).get(0) == -4 );
    tdd.verify( HasComplexFieldGet<decltype(test3)>::value == true );


    auto test4 = test3 * MySU2Doublet(1,1,1,1);
    tdd.verify( test4.SU2DoubletGet(0_c).get(0) == 2 );
    tdd.verify( test4.SU2DoubletGet(1_c).get(0) == -6 );
    tdd.verify( test4.SU2DoubletGet(2_c).get(0) == 2 );
    tdd.verify( test4.SU2DoubletGet(3_c).get(0) == -6 );






    auto toolBox = MemoryToolBox::makeShared(3, 32, 1);


    //Multilplication, test again.

    auto it = toolBox->itX();
    it.begin();

    auto tmp = it();

    SU2Field<double> A("pimpin2", toolBox, LatticeParameters<double>());
    SU2Field<double> B ("pimpin3", toolBox, LatticeParameters<double>());
    SU2DoubletBase<double> CC ("pimpin3", toolBox, LatticeParameters<double>());


    A = SU2Wrap(1.,0.2,0.13,0.14);
    B = SU2Wrap(1.,0.2,0.13,0.14);
    CC = SU2DoubletWrap(1.,0.2,0.13,0.14);



    //Multiplication by a complex number

    auto D = Complexify(1.0,2.0) * CC;
    tdd.verify( AlmostEqual(D.SU2DoubletGet(0_c).get(tmp),0.6));
    tdd.verify( AlmostEqual(D.SU2DoubletGet(0_c,tmp),0.6));
    tdd.verify( AlmostEqual(D.SU2DoubletGet(1_c).get(tmp),2.2));
    tdd.verify( AlmostEqual(D.SU2DoubletGet(1_c,tmp),2.2));
    tdd.verify( AlmostEqual(D.SU2DoubletGet(2_c).get(tmp),-0.15));
    tdd.verify( AlmostEqual(D.SU2DoubletGet(2_c,tmp),-0.15));
    tdd.verify( AlmostEqual(D.SU2DoubletGet(3_c).get(tmp),0.4));
    tdd.verify( AlmostEqual(D.SU2DoubletGet(3_c,tmp),0.4));


    auto A2 = A*A;
    auto A3 = A*A*A;


   /* say << A2.SU2Get(1_c);

    say << (A*B).SU2Get(0_c);
    say << (A2*A2*A2).SU2Get(0_c);
    say << (A3*A3).SU2Get(0_c);
    say << (A*A*A*A*A*A).SU2Get(0_c);
    say << ((A*A)*(A*A)*(A*A)).SU2Get(0_c);


    auto B2 = A2*A2*A2;
    B2.eval(tmp);
    say << (B2).SU2Get(2_c,tmp);
    say << (A3*A3).SU2Get(2_c).get(tmp);
    say << (A*A*A*A*A*A).SU2Get(2_c).get(tmp);
    say << ((A*A)*(A*A)*(A*A)).SU2Get(2_c).get(tmp);


    //Funny stuff: uncomment to get a segfault! Morale: use too much memory on the stack and crashes
  //  say << (A*A*A*A*A*A*A*A).SU2Get(0_c);
  //  say << (A*A*A*A*A*A*A*A).SU2Get(0_c).get(tmp);
    say << ((A*A*A*A)*(A*A*A*A)).SU2Get(0_c);
    say << ((A*A*A*A)*(A*A*A*A)).SU2Get(0_c).get(tmp);*/

}

#endif
