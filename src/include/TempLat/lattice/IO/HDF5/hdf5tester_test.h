#ifndef TEMPLAT_LATTICE_IO_HDF5_HDF5TESTER_TEST_H
#define TEMPLAT_LATTICE_IO_HDF5_HDF5TESTER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/IO/HDF5/filesaverhdf5.h"
#include "TempLat/lattice/IO/HDF5/fileloaderhdf5.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/util/almostequal.h"

inline void TempLat::HDF5Tester::Test(TempLat::TDDAssertion &tdd)
{
  FileSaverHDF5 fs;
  FileLoaderHDF5 fl;

  const ptrdiff_t nGrid = 16, nGhost = 1;
  auto toolBox = MemoryToolBox<3>::makeShared(nGrid, nGhost);

  Field<3, double> phi("phi", toolBox);
  SpatialCoordinate<3> coords(toolBox);
  auto x = TempLat::getVectorComponent(coords, 0);
  auto y = TempLat::getVectorComponent(coords, 1);
  auto z = TempLat::getVectorComponent(coords, 2);
  auto local_idx = x * nGrid * nGrid + y * nGrid + z;
  phi = local_idx + 42.0;

  fs.create("./FILE.h5");
  fs.save(phi);
  fs.save(0.45, "aDot");
  fs.close();

  Field<3, double> psi("phi", toolBox);
  double aDot = 0;

  fl.open("./FILE.h5");
  fl.load(psi);
  fl.load(aDot, "aDot");
  fl.close();

  tdd.verify(AlmostEqual(aDot, 0.45));

  {
    auto localView = psi.getLocalNDHostView();
    bool all_correct = true;
    for (ptrdiff_t i = 0; i < localView.extent(0); ++i)
      for (ptrdiff_t j = 0; j < localView.extent(1); ++j)
        for (ptrdiff_t k = 0; k < localView.extent(2); ++k) {
          all_correct &= (AlmostEqual(localView(i, j, k), 42.0 + local_idx.get(i + nGhost, j + nGhost, k + nGhost)));
          if (!AlmostEqual(localView(i, j, k), 42.0 + local_idx.get(i + nGhost, j + nGhost, k + nGhost))) {
            std::cout << "Error at " << i << " " << j << " " << k << " got " << localView(i, j, k) << " expected "
                      << 42.0 + local_idx.get(i + nGhost, j + nGhost, k + nGhost) << std::endl;
          }
        }
    tdd.verify(all_correct);
  }
}

#endif
