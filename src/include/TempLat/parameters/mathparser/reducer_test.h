#ifndef TEMPLAT_PARAMETERS_MATHPARSER_REDUCER_TEST_H
#define TEMPLAT_PARAMETERS_MATHPARSER_REDUCER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

inline void TempLat::Reducer::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */


    char t1[256] ("a=5"), t2[256]("  b = 25"), t3[256]("cc=true"), t4[256]("d= 24 45 76 143"), t5[256]("t=../../src/tests/fcn/parameters/test_filereader.txt");
    char* argv[6];
    argv[0] = t1;
    argv[1] = t1;
    argv[2] = t2;
    argv[3] = t3;
    argv[4] = t4;
    argv[5] = t5;
    ParameterParser params(6, argv);// we pass number of string plus 1 to argc because in real situation ther eis also the program name
    std::cout<<"Parameters: "<<params;

    tdd.verify(reduce("3*2",params)==6);
    tdd.verify(reduce("3*2+2",params)==8);
    tdd.verify(reduce("3*(2+2)",params)==12);
    tdd.verify(reduce("3*(2+2)-2*8",params)==-4);
    tdd.verify(reduce("(3*(2+2)-2*8-5)",params)==-9);
    tdd.verify(reduce("(3*(2+2)-2*8-5)*9",params)==-81);
    tdd.verify(AlmostEqual(reduce("(3*(2+2)-2*8-5)*9/2",params),-40.5));
    tdd.verify(AlmostEqual(reduce("(3*(2+2/2)-2*8-5)*9/2",params),-54));
    tdd.verify(AlmostEqual(reduce("(3*(2+2/2)-2*8-5)*9/(2+4*4)",params),-6));
    tdd.verify(AlmostEqual(reduce("(3*(2+2/2)-2*8-5)*9/(2+4*4/(2*4))",params),-27));
    tdd.verify(AlmostEqual(reduce("(3*(2+2/2)-2*8-5)*9/(2+4*4/(2*4))^2",params),-6.75));
    tdd.verify(AlmostEqual(reduce("(3*(2+2/2)-2*8-5)*9/(2+4*4/(2*4))^2^2",params),-0.421875));

    tdd.verify(AlmostEqual(reduce("exp(8)",params),2980.9579870417283));

    tdd.verify(AlmostEqual(reduce("(3*(2+2/exp(2))-2*8-5)*9/(2+4*4/(2*4))^2^2",params),-0.4987964636922769));
    tdd.verify(AlmostEqual(reduce("(3*(2+2/exp(2+2*8-12+2*4-5^2))-2*8-5)*9/(2+4*4/(2*4))^2^2",params),12629.17442429954));
    tdd.verify(AlmostEqual(reduce("(3*(2+2/ln(abs(2+2*8-12+2*4-5^2)))-2*8-5)*9/(2+4*4/(2*4))^2^2",params),-0.4393759799339481));
    tdd.verify(AlmostEqual(reduce("(3*(2+2/sin(2+2*8-12+2*4-5^2))-2*8-5)*9/(2+4*4/(2*4))^2^2",params),-0.31640418417405747));
    tdd.verify(AlmostEqual(reduce("(3*(2+2/cos(2+2*8-12+2*4-5^2))-2*8-5)*9/(2+4*4/(2*4))^2^2",params),47.13462969905214));
    tdd.verify(AlmostEqual(reduce("(3*(2+2/ln(-(2+2*8-12+2*4-5^2)))-2*8-5)*9/(2+4*4/(2*4))^2^2",params),-0.4393759799339481));

    tdd.verify(AlmostEqual(reduce("2*(-6+3)",params),-6));
    tdd.verify(AlmostEqual(reduce("exp(2*(-6+3))",params),0.0024787521766663585));

    tdd.verify(reduce("2*\'a\'",params) == 10);
    tdd.verify(reduce("\'b\'*\'a\'",params) == 125);




}

#endif
