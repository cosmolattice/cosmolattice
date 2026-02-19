/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/preget.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct PreGetTester {
    static void Test(TDDAssertion &tdd);
  };

  class NoPreGet
  {
  public:
    NoPreGet() = default;
  };

  class WithPreGet
  {
  public:
    void preGet() { called = true; }
    static bool called;
  };
  bool WithPreGet::called = false; // Initialize static member variable

  void PreGetTester::Test(TDDAssertion &tdd)
  {
    NoPreGet noPreGet;
    WithPreGet withPreGet;

    PreGet::apply(noPreGet);   // should compile
    PreGet::apply(withPreGet); // should compile

    tdd.verify(WithPreGet::called == true); // should be true, since we called preGet
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::PreGetTester> test;
}