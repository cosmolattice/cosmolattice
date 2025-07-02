#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENT_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENT_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/field/collections/fieldcollection.h"

template <typename T> inline void TempLat::GetVectorComponentHelper<T>::Test(TempLat::TDDAssertion &tdd)
{
  // TODO but this is much.
  auto toolBox = MemoryToolBox<3>::makeShared(32, 1);
  toolBox->setVerbose();
  FieldCollection<double> fc(3, "abcdefg", toolBox);
  fc[2].inFourierSpace() = 2;
  auto test1 = GetVectorComponentHelper<FieldCollection<double>>(fc, 1);
  auto test2 = GetVectorComponentHelper<FieldCollection<double>>(fc, 2);

  test1.confirmSpace(toolBox->mLayouts.getConfigSpaceLayout(), SpaceStateInterface::SpaceType::Configuration);
  tdd.verify(fc[2].isFourierSpace() == true);
  test2.confirmSpace(toolBox->mLayouts.getConfigSpaceLayout(), SpaceStateInterface::SpaceType::Configuration);
  tdd.verify(fc[2].isFourierSpace() == false);

  fc[1].setGhostsAreStale();
  test2.confirmGhostsUpToDate();
  tdd.verify(fc[1].areGhostsStale() == true);
  test1.confirmGhostsUpToDate();
  tdd.verify(fc[1].areGhostsStale() == false);

  tdd.verify(test1.toString() == "abcdefg#1");
}

#endif
