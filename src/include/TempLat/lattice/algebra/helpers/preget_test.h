#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_PREGET_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_PREGET_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

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

inline void TempLat::PreGet::Test(TempLat::TDDAssertion &tdd)
{
  NoPreGet noPreGet;
  WithPreGet withPreGet;

  PreGet::apply(noPreGet);   // should compile
  PreGet::apply(withPreGet); // should compile

  tdd.verify(WithPreGet::called == true); // should be true, since we called preGet
}

#endif
