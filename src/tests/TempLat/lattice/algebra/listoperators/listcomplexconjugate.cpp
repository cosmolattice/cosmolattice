
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/listoperators/listcomplexconjugate.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ListComplexConjugateTester {
    static void Test(TDDAssertion &tdd);
  };

  void ListComplexConjugateTester::Test(TDDAssertion &tdd)
  {
    complex<double> tmp(1, 1);
    auto t1 = std::make_tuple(tmp, tmp, tmp, tmp);

    auto t3 = conj(t1);
    tdd.verify(GetComponent::get(t3, Tag<0>()) == conj(tmp));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ListComplexConjugateTester> test;
}
