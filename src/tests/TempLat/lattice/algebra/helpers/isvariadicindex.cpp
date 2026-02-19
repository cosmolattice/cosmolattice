/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  template <> void IsVariadicIndexTester::Test<TDDAssertion>(TDDAssertion &tdd)
  {
    struct some_type {
    };

    tdd.verify(IsVariadicIndex<size_t, const size_t &, size_t &&> == true);
    tdd.verify(IsVariadicIndex<size_t, const size_t &, size_t &&, int> == true);
    tdd.verify(IsVariadicIndex<some_type> == false);
    tdd.verify(IsVariadicIndex<size_t, const size_t &, size_t &&, double &&> == false);

    tdd.verify(IsVariadicNDIndex<3, size_t, const size_t &, size_t &&> == true);
    tdd.verify(IsVariadicNDIndex<4, size_t, const size_t &, size_t &&> == false);
    tdd.verify(IsVariadicNDIndex<4, size_t, const size_t &, size_t &&, int> == true);
    tdd.verify(IsVariadicNDIndex<2, size_t, const size_t &, size_t &&, int> == false);
    tdd.verify(IsVariadicNDIndex<1, some_type> == false);
    tdd.verify(IsVariadicNDIndex<4, size_t, const size_t &, size_t &&, double &&> == false);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::IsVariadicIndexTester> test;
}