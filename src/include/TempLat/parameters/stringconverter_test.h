#ifndef TEMPLAT_PARAMETERS_STRINGCONVERTER_TEST_H
#define TEMPLAT_PARAMETERS_STRINGCONVERTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::StringConverterTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */

    StringConverter<double> strc;
    MultipleParameterGetter<double> dbles;
    strc("54 67 89", dbles, "test");
    //for(auto d: dbles) say<<d();
    for (size_t i = 0; i < dbles.size(); ++i) say << dbles(i);
    tdd.verify( dbles[0]() == 54 && dbles[1]() == 67 && dbles[2]() == 89 );

}

#endif
