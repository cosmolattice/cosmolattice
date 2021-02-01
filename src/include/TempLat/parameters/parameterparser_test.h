#ifndef TEMPLAT_PARAMETERS_PARAMETERS_TEST_H
#define TEMPLAT_PARAMETERS_PARAMETERS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/almostequal.h"

inline void TempLat::ParameterParser::Test(TempLat::TDDAssertion& tdd) {
/*Uncomment and put the correct file path*/
    bool UncommentAndPutTheCorrectFilePathForTheTest =true;
    tdd.verify(UncommentAndPutTheCorrectFilePathForTheTest);
    /*char t1[256] ("aaa=bbb"), t2[256]("  aa = 25.78"), t3[256]("cc=true"), t4[256]("d= 24 45 76 143"), t5[256]("input=../../src/tests/TempLat/parameters/test_filereader.txt");
    char* argv[6];
    argv[0] = t1;
    argv[1] = t1;
    argv[2] = t2;
    argv[3] = t3;
    argv[4] = t4;
    argv[5] = t5;
    ParameterParser test(6, argv);// we pass number of string plus 1 to argc because in real situation ther eis also the program name
    std::cout<<"Parameters: "<<test;


    auto mystr = test.get<std::string>("aaa");
    auto mystr2 = test.get<std::string>("bbb");
    auto myd = test.get<double>("aa");
    auto myI = test.get<int, 4>("d");
    auto myOptInt = test.get<int, 2>("iii1", {2, 3});
    auto myOptInt2 = test.get<int, 8>("iii2", 1);
    auto myOptInt3 = test.get<int, 8>("iii3", {1,2,3,4,5,6,7,8});
    auto myOptInt4 = test.get<int, 8>("iii4", 8);
    auto myOptInt5 = test.get<int, 0>("iii5"); //Just to make sure it does not crash even if mandatory. Useful if the size of array depends on templated size that can be zero.

    say << myOptInt2(7);
   // "Throws"  to check exceptions.
    //auto myMissInt = test.get<int, 2>("iii");
    //auto myMisMatchInt = test.get<int, 2>("iii", {2});
    //auto myMisMa = test.get<int, 2>("ii");
    //auto myMisMa = test.get<int, 2>("");

    std::cout<<"mystr : "<<mystr()<<std::endl;
    std::cout<<"mystr2 : "<<mystr2()<<std::endl;
    std::cout<<"myd : "<<myd()<<std::endl;
    for(size_t i = 0; i<myI.size();++i) std::cout<<myI(i)<<std::endl;
    tdd.verify( AlmostEqual(25.78, myd()) );*/

}

#endif
