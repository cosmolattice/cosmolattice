
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/definitions/mattercurrents.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct MatterCurrentsTester {
    static void Test(TDDAssertion &tdd);
  };

  void MatterCurrentsTester::Test(TDDAssertion &tdd)
  {
    // auto SU2Part = model.gQ_SU2DblSU2(a, Tag<N>()) * MakeSU2(b, Real(model.fldSU2Doublet(a)));

    // return pow<2>((model.fStar / model.omegaStar)) / 2.0 * SU2Part;

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MatterCurrentsTester> test;
}
