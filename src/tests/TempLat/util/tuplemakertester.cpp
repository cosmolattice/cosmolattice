
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#include "TempLat/util/tuplemakertester.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/util/tuplemaker.h"
#include "TempLat/util/flattentuple.h"

namespace TempLat
{

  struct TupleMakerTester {
    static void Test(TDDAssertion &tdd);
  };

  void TupleMakerTester::Test(TDDAssertion &tdd)
  {

    auto toolBox = MemoryToolBox<3>::makeShared(32, 1);
    VectorFieldCollection<SU2Field<3, double>, 2> fldSU2("SU2Fld", toolBox, LatticeParameters<double>());

    auto t1 = flatten_tuple(make_tuple_from(fldSU2));

    tdd.verify(tuple_size<decltype(flatten_tuple(t1))>::value ==
               24); // 2 * 3 * 4, 2=# flds, 3 cause vector, 4 cause 4 su2 components.
    tdd.verify((std::get<10>(t1)).toString() == "SU2Fld_0_3_2(x)");
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::TupleMakerTester> test;
}
