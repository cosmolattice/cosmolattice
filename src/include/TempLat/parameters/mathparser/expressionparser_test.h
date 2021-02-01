#ifndef TEMPLAT_PARAMETERS_MATHPARSER_EXPRESSIONPARSER_TEST_H
#define TEMPLAT_PARAMETERS_MATHPARSER_EXPRESSIONPARSER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

inline void TempLat::ExpressionParser::Test(TempLat::TDDAssertion& tdd) {


    char t1[256] ("a=5"), t2[256]("  b = 25"), t3[256]("cc=\'a\'*\'b\'+245-ln(987)"), t4[256]("d= 24 45 76 143"), t5[256]("t=../../src/tests/fcn/parameters/test_filereader.txt");
    char* argv[6];
    argv[0] = t1;
    argv[1] = t1;
    argv[2] = t2;
    argv[3] = t3;
    argv[4] = t4;
    argv[5] = t5;
    ParameterParser params(6, argv);// we pass number of string plus 1 to argc because in real situation ther eis also the program name

    say << get("cc",params);



}

#endif
