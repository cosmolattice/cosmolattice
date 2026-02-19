
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/measuringtools/toolwithownmemory.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ToolWithOwnMemoryTester {
    static void Test(TDDAssertion &tdd);
  };

  void ToolWithOwnMemoryTester::Test(TDDAssertion &tdd)
  {

    auto toolBox = MemoryToolBox<2>::makeShared(16, 1);

    /*typedef double T;

    Field<T> testField("test", toolBox);

    ToolWithOwnMemory<Field<T>> tester(testField);

    auto workSpace = tester.getFieldForMeasurement("");

    tdd.verify( tester.persistentField.get() == nullptr );

    tester.setPersistentMemory();

    auto workSpace2 = tester.getFieldForMeasurement("");

    tdd.verify( tester.persistentField.get() != nullptr && workSpace2.getMemoryManager() ==
    tester.persistentField->getMemoryManager() );*/
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ToolWithOwnMemoryTester> test;
}
