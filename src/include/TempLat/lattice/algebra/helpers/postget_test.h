#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_POSTGET_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_POSTGET_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

class NoPostGet
{
public:
  NoPostGet() = default;
};

class WithPostGet
{
public:
  void postGet() { called = true; }
  static bool called;
};
bool WithPostGet::called = false; // Initialize static member variable

inline void TempLat::PostGet::Test(TempLat::TDDAssertion &tdd)
{
  NoPostGet noPostGet;
  WithPostGet withPostGet;

  PostGet::apply(noPostGet);   // should compile
  PostGet::apply(withPostGet); // should compile

  tdd.verify(WithPostGet::called == true); // should be true, since we called postGet
}

#endif
