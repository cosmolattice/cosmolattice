#ifndef TEMPLAT_UTIL_PARENTHESISSTRIPPER_TEST_H
#define TEMPLAT_UTIL_PARENTHESISSTRIPPER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

inline void TempLat::ParenthesisStripper::Test(TempLat::TDDAssertion& tdd) {

    std::string tmp("(a*2)");
    strip(tmp);
    tdd.verify(tmp == "a*2");
    tmp ="[a*2]";
    strip(tmp);
    tdd.verify(tmp == "a*2");
    tmp ="{a*2}";
    strip(tmp);
    tdd.verify(tmp == "a*2");
    tmp ="{a*2}+3";
    strip(tmp);
    tdd.verify(tmp == "{a*2}+3");
    tmp ="({a*2}+3)";
    strip(tmp);
    tdd.verify(tmp == "{a*2}+3");

    tdd.verify(findClosing("(a+13)+4",'(',')')==5);

    tmp = "(3*(2+2/2)-2*8-5)*9/(2+4*4)";
    strip(tmp);
    tdd.verify(tmp == "(3*(2+2/2)-2*8-5)*9/(2+4*4)");



}

#endif
