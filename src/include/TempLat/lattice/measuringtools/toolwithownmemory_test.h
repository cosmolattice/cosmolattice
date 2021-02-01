#ifndef TEMPLAT_LATTICE_MEASUREMENTS_TOOLWITHOWNMEMORY_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_TOOLWITHOWNMEMORY_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::ToolWithOwnMemoryTester::Test(TempLat::TDDAssertion& tdd) {

    auto toolBox = MemoryToolBox::makeShared(2, 16, 1);

    /*typedef double T;

    Field<T> testField("test", toolBox);

    ToolWithOwnMemory<Field<T>> tester(testField);

    auto workSpace = tester.getFieldForMeasurement("");

    tdd.verify( tester.persistentField.get() == nullptr );

    tester.setPersistentMemory();

    auto workSpace2 = tester.getFieldForMeasurement("");

    tdd.verify( tester.persistentField.get() != nullptr && workSpace2.getMemoryManager() == tester.persistentField->getMemoryManager() );*/

}

#endif
