#ifndef COSMOINTERFACE_HELPERS_MATTERCURRENTS_TEST_H
#define COSMOINTERFACE_HELPERS_MATTERCURRENTS_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

inline void TempLat::MatterCurrentsTester::Test(TempLat::TDDAssertion &tdd)
{
  // auto SU2Part = model.gQ_SU2DblSU2(a, Tag<N>()) * MakeSU2(b, Real(model.fldSU2Doublet(a)));

  // return pow<2>((model.fStar / model.omegaStar)) / 2.0 * SU2Part;

  /* Default is to fail: to remind yourself to implement something here. */
  tdd.verify(true);
}

#endif
