
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#ifdef HAVE_HDF5
#include "TempLat/lattice/IO/HDF5/hdf5tester.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/IO/HDF5/filesaverhdf5.h"
#include "TempLat/lattice/IO/HDF5/fileloaderhdf5.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

  struct HDF5Tester {
    static void Test(TDDAssertion &tdd);
  };

  void HDF5Tester::Test(TDDAssertion &tdd)
  {
    // Existing test: save/load field round-trip
    {
      FileSaverHDF5 fs;
      FileLoaderHDF5 fl;

      const ptrdiff_t nGrid = 16, nGhost = 1;
      auto toolBox = MemoryToolBox<3>::makeShared(nGrid, nGhost);

      Field<3, double> phi("phi", toolBox);
      SpatialCoordinate<3> coords(toolBox);
      auto x = coords(1_c);
      auto y = coords(2_c);
      auto z = coords(3_c);
      auto local_idx = x * nGrid * nGrid + y * nGrid + z;
      phi = local_idx + 42.0;

      fs.create("./FILE.h5");
      fs.save(phi);
      fs.save(x * 1.);
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
              all_correct &=
                  (AlmostEqual(localView(i, j, k), 42.0 + local_idx.eval(i + nGhost, j + nGhost, k + nGhost)));
              if (!AlmostEqual(localView(i, j, k), 42.0 + local_idx.eval(i + nGhost, j + nGhost, k + nGhost))) {
                std::cout << "Error at " << i << " " << j << " " << k << " got " << localView(i, j, k) << " expected "
                          << 42.0 + local_idx.eval(i + nGhost, j + nGhost, k + nGhost) << std::endl;
              }
            }
        tdd.verify(all_correct);
      }
    }

    // Time series test
    {
      HDF5File file;
      file.create("test_extendible.h5");
      bool amIRoot = true;
#ifdef HAVE_MPI
      int rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      amIRoot = (rank == 0);
#endif

      auto group = file.createOrOpenGroup("av");
      auto dataset = group.createTimeSeries<int>("t", {0}, {10});

      for (int i = 0; i < 10; ++i) {
        dataset.push(i);
      }

      dataset.extend(10);
      dataset.flush(amIRoot);

      auto sizes = dataset.getSizes();
      tdd.verify(sizes[0] == 10, "Time series has correct size after flush");

      dataset.close();

      auto dataset2 = group.createTimeSeries<double>("s", {0, 4}, {10, 1});

      for (int i = 0; i < 10; ++i) {
        dataset2.push(std::vector<double>{i / 6.0, 8, 9.0 / (i + 1), 3});
      }
      dataset2.extend(10);
      dataset2.flush(amIRoot);

      dataset2.close();
      group.close();
      file.close();

      tdd.verify(true);
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::HDF5Tester> test;
}

#endif
