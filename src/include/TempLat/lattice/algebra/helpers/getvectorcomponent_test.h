#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENT_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENT_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/field/collections/fieldcollection.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/helpers/hasgetmethod.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"

template <int N, typename T> inline void TempLat::GetVectorComponentHelperTester<N, T>::Test(TempLat::TDDAssertion &tdd)
{
  // TODO but this is much.
  auto toolBox = MemoryToolBox<3>::makeShared(32, 1);
  toolBox->setVerbose();
  FieldCollection<Field<3, double>, double, 3> fc("abcdefg", toolBox);

  fc[1] = 1;
  fc[2].inFourierSpace() = 2;
  tdd.verify(fc[1].isFourierSpace() == false);
  tdd.verify(fc[2].isFourierSpace() == true);

  auto test1 = GetVectorComponentHelper<1, FieldCollection<Field<3, double>, double, 3>>(fc);
  auto test2 = GetVectorComponentHelper<2, FieldCollection<Field<3, double>, double, 3>>(fc);

  auto test_wavenumber = GetVectorComponentHelper<1, WaveNumber<3>>(WaveNumber<3>(toolBox));

  tdd.verify(HasGetMethod<decltype(test_wavenumber)>);
  static_assert(HasGetMethod<decltype(test_wavenumber)>);

  test1.confirmSpace(toolBox->mLayouts.getConfigSpaceLayout(), SpaceStateType::Configuration);
  tdd.verify(fc[2].isFourierSpace() == true);
  test2.confirmSpace(toolBox->mLayouts.getConfigSpaceLayout(), SpaceStateType::Configuration);
  tdd.verify(fc[2].isFourierSpace() == false);

  fc[1].setGhostsAreStale();
  test2.confirmGhostsUpToDate();
  tdd.verify(fc[1].areGhostsStale() == true);
  test1.confirmGhostsUpToDate();
  tdd.verify(fc[1].areGhostsStale() == false);

  tdd.verify(test1.toString() == "abcdefg_1(x)");
}

#endif
