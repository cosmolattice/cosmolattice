
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/shiftedindexsequence.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ShiftedIndexSequenceTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t... I> auto myConvertToTuple(std::index_sequence<I...> seq) { return std::make_tuple(I...); }

  void ShiftedIndexSequenceTester::Test(TDDAssertion &tdd)
  {
    auto test = myConvertToTuple(shifted_index_sequence<12>(std::make_index_sequence<3>{}));

    tdd.verify(std::get<0>(test) == 12);
    tdd.verify(std::get<1>(test) == 13);
    tdd.verify(std::get<2>(test) == 14);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ShiftedIndexSequenceTester> test;
}
