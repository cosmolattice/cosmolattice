
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2024

#include "TempLat/lattice/algebra/constants/symbols.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct SymbolsTester {
    static void Test(TDDAssertion &tdd);
  };

  void SymbolsTester::Test(TDDAssertion &tdd)
  {
    tdd.verify(Symbols::epsilon(Tag<1>(), Tag<2>(), Tag<3>()).eval(0) == 1);
    tdd.verify(Symbols::epsilon(Tag<3>(), Tag<3>(), Tag<3>()).eval(0) == 0);
    tdd.verify(Symbols::epsilon(Tag<3>(), Tag<2>(), Tag<3>()).eval(0) == 0);
    tdd.verify(Symbols::epsilon(Tag<3>(), Tag<2>(), Tag<1>()).eval(0) == -1);
    tdd.verify(Symbols::epsilon(Tag<2>(), Tag<1>(), Tag<3>()).eval(0) == -1);
    tdd.verify(Symbols::epsilon(Tag<2>(), Tag<1>(), Tag<1>()).eval(0) == 0);
    tdd.verify(Symbols::epsilon(Tag<2>(), Tag<3>(), Tag<1>()).eval(0) == 1);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SymbolsTester> test;
}
