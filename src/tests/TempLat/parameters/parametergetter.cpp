
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/parameters/parametergetter.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

  struct ParameterGetterTester {
    static void Test(TDDAssertion &tdd);
  };

  void ParameterGetterTester::Test(TDDAssertion &tdd)
  {
    double d = 9.876;

    ParameterGetter<double> pgd(d, "");

    ParameterGetter<int> pgi(d, "xr");

    std::string str = "9.876";

    ParameterGetter<std::string> pgs(str, "str");

    std::string strbis = pgs;

    say << pgs;

    /* Default is to fail: to remind yourself to implement something here. */

    tdd.verify(AlmostEqual(d, pgd()));

    tdd.verify((int)d == pgi());

    tdd.verify(str == pgs());
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ParameterGetterTester> test;
}
