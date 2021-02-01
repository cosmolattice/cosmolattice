#ifndef TEMPLAT_UTIL_TUPLEMAKERTESTER_TEST_H
#define TEMPLAT_UTIL_TUPLEMAKERTESTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/util/tuplemaker.h"
#include "TempLat/util/flattentuple.h"

inline void TempLat::TupleMakerTester::Test(TempLat::TDDAssertion& tdd) {

    auto toolBox = MemoryToolBox::makeShared(3, 32, 1);
    VectorFieldCollection<SU2Field, double, 3, 2> fldSU2("SU2Fld",toolBox,LatticeParameters<double>());


    auto t1 = flatten_tuple(make_tuple_from(fldSU2));

    tdd.verify(tuple_size<decltype(flatten_tuple(t1))>::value == 18); // 2* 3 * 3, 2=# flds, 3 cause vector, 3 cause 3 su2 (skip the constraint).
    tdd.verify( (std::get<10>(t1)).toString() == "SU2Fld_1_1_2(x)");




}

#endif
