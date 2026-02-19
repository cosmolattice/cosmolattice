
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/getcpptypename.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct GetCPPTypeNameTester {
    static void Test(TDDAssertion &tdd);
  };

  void GetCPPTypeNameTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(GetCPPTypeName::get(std::vector<ptrdiff_t>()).find("std::") != std::string::npos);
    tdd.verify(GetCPPTypeName::get(std::vector<ptrdiff_t>()).find("::vector") != std::string::npos);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::GetCPPTypeNameTester> test;
}
