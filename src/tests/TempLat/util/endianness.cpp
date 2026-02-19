
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/endianness.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct EndiannessTester {
    static void Test(TDDAssertion &tdd);
  };

  void EndiannessTester::Test(TDDAssertion &tdd)
  {

    Endianness endian;
    tdd.verify(endian.isLittle());
    tdd.verify(!endian.isBig());
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::EndiannessTester> test;
}
