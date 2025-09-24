#ifndef TEMPLAT_LATTICE_MEMORY_GHOSTBUSTER_TEST_H
#define TEMPLAT_LATTICE_MEMORY_GHOSTBUSTER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"

#include <sstream>

namespace TempLat
{
  namespace Testing
  {
    /* single datum on a grid: 24 bytes making up x, y, and z. */
    struct datum {
      ptrdiff_t x, y, z;
    };
  } // namespace Testing
} // namespace TempLat

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
template <size_t NDim> inline void TempLat::GhostBuster<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  // Test the ghostbuster on a single node.
  {
    // arbitrary irregular sizing. If you want to see debug prints of what happens, set them to <= 4
    std::array<ptrdiff_t, 3> nGrid{{256, 64, 128}};

    LayoutStruct<3> layout({62, 62, 62}, 1);
    layout.setLocalSizes(nGrid);

    auto &&myLittleLambda = [&](auto nGhost, auto nGhostB) {
      ptrdiff_t memSize1 = (nGrid[0] + nGhost[0][0] + nGhost[0][1]) * (nGrid[1] + nGhost[1][0] + nGhost[1][1]) *
                           (nGrid[2] + nGhost[2][0] + nGhost[2][1]);

      ptrdiff_t memSize2 = (nGrid[0] + nGhostB[0][0] + nGhostB[0][1]) * (nGrid[1] + nGhostB[1][0] + nGhostB[1][1]) *
                           (nGrid[2] + nGhostB[2][0] + nGhostB[2][1]);

      JumpsHolder<3> jumperFrom(layout, nGhost);
      JumpsHolder<3> jumperTo(layout, nGhostB);

      MemoryBlock<3, Testing::datum> memory(std::max(memSize1, memSize2));

      // setup the controlled known memory; each entry equals its position
      {
        auto memory_view = memory.getRawHostView();
        for (ptrdiff_t i = -nGhost[0][0]; i < nGrid[0] + nGhost[0][1]; ++i) {
          ptrdiff_t iPos =
              (i + nGhost[0][0]) * (nGrid[1] + nGhost[1][0] + nGhost[1][1]) * (nGrid[2] + nGhost[2][0] + nGhost[2][1]);
          for (ptrdiff_t j = -nGhost[1][0]; j < nGrid[1] + nGhost[1][1]; ++j) {
            ptrdiff_t jPos = (j + nGhost[1][0]) * (nGrid[2] + nGhost[2][0] + nGhost[2][1]);
            for (ptrdiff_t k = -nGhost[2][0]; k < nGrid[2] + nGhost[2][1]; ++k) {
              ptrdiff_t kPos = k + nGhost[2][0];
              ptrdiff_t pos = iPos + jPos + kPos;
              // std::cerr << "Hoi " << pos << " " << i << " " << j << " " << k << "\n";
              memory_view[pos].x = i;
              memory_view[pos].y = j;
              memory_view[pos].z = k;
            }
          }
        }
        // push the memory to the device.
        memory.pushHostView();
      }

      auto print_it = [&](const auto gh, const std::string name) {
        std::array<ptrdiff_t, 3> fullSizes;
        for (size_t i = 0; i < 3; ++i) {
          fullSizes[i] = nGrid[i] + gh[i][0] + gh[i][1];
          // no debug print for too large arrays
          if (fullSizes[i] > 4) return;
        }

        std::cout << "\n" << name << "\n";

        memory.flagHostMirrorOutdated();
        auto full_view = memory.getNDHostView(fullSizes);

        auto sub_view = Kokkos::subview(full_view, Kokkos::make_pair(gh[0][0], gh[0][0] + nGrid[0]),
                                        Kokkos::make_pair(gh[1][0], gh[1][0] + nGrid[1]),
                                        Kokkos::make_pair(gh[2][0], gh[2][0] + nGrid[2]));

        // print a 2D slice of the 3D data, for x=0
        for (ptrdiff_t x = 0; x < nGrid[0]; ++x) {
          std::cout << "x=" << x << "  ";
          for (int i = 0; i < nGrid[2]; ++i) {
            if (i == 0)
              std::cout << "   z    ";
            else
              std::cout << "   -    ";
          }
          std::cout << "\n";
          for (ptrdiff_t i = 0; i < nGrid[1]; ++i) {
            if (i == 0)
              std::cout << "  y  ";
            else
              std::cout << "  |  ";

            for (ptrdiff_t j = 0; j < nGrid[2]; ++j) {
              std::cerr << "(" << sub_view(x, i, j).x << "," << sub_view(x, i, j).y << "," << sub_view(x, i, j).z
                        << ") ";
            }
            std::cerr << "\n";
          }
          std::cerr << "\n";
        }
        std::cerr << "\n";
      };

      bool SingleDeviceAllRight[] = {true, true};
      // test once forward, once back.
      for (int x = 0; x < 1; ++x) {

        if (x == 0)
          print_it(nGhost, "Memory at x=0 BEFORE GhostBuster:\n");
        else
          print_it(nGhostB, "Memory at x=1 BEFORE GhostBuster back:\n");

        GhostBuster<3> egon(x == 0 ? jumperFrom : jumperTo, x == 0 ? jumperTo : jumperFrom);

        egon(memory);

        JumpsHolder<3> jumper(x == 0 ? jumperTo : jumperFrom);

        if (x == 0)
          print_it(nGhostB, "Memory at x=0 AFTER GhostBuster:\n");
        else
          print_it(nGhost, "Memory at x=1 AFTER GhostBuster back:\n");

        memory.flagHostMirrorOutdated();
        auto memory_view = memory.getRawHostView();

        // verify the setup, not controlled,
        // assuming jumps are correct (tested elsewhere),
        // verify that the GhostBuster did not damage the memory,
        // and put stuff in their new correct place.
        for (ptrdiff_t i = 0; i < nGrid[0]; ++i) {
          for (ptrdiff_t j = 0; j < nGrid[1]; ++j) {
            for (ptrdiff_t k = 0; k < nGrid[2]; ++k) {
              ptrdiff_t pos = jumper.toOrigin() + jumper.getJumpsInMemoryOrder()[0] * i +
                              jumper.getJumpsInMemoryOrder()[1] * j + jumper.getJumpsInMemoryOrder()[2] * k;
              const Testing::datum &dat = memory_view[pos];
              const bool thisPosRight = dat.x == i && dat.y == j && dat.z == k;
              SingleDeviceAllRight[x] = SingleDeviceAllRight[x] && thisPosRight;
              if (!thisPosRight)
                std::cerr << i << ", " << j << ", " << k << " => " << dat.x << ", " << dat.y << ", " << dat.z << "\n";
            }
          }
        }
      }
      tdd.verify(SingleDeviceAllRight[0]);
      tdd.verify(SingleDeviceAllRight[1]);
    };

    // arbitrary irregular ghosting A
    std::array<std::array<ptrdiff_t, 2u>, 3> nGhost1{};

    nGhost1[0][0] = 6;
    nGhost1[0][1] = 5;
    nGhost1[1][0] = 4;
    nGhost1[1][1] = 3;
    nGhost1[2][0] = 2;
    nGhost1[2][1] = 1;

    std::array<std::array<ptrdiff_t, 2u>, 3> nGhost2{};

    // arbitrary irregular ghosting B: by choice slightly smaller than A, to be safe.
    nGhost2[0][0] = 5;
    nGhost2[0][1] = 6;
    nGhost2[1][0] = 9;
    nGhost2[1][1] = 9;
    nGhost2[2][0] = 0;
    nGhost2[2][1] = 1;

    tdd.verify(Throws<GhostBusterOrderException>([&]() { myLittleLambda(nGhost1, nGhost2); }));

    nGhost2[0][0] = 5;
    nGhost2[0][1] = 4;
    nGhost2[1][0] = 3;
    nGhost2[1][1] = 2;
    nGhost2[2][0] = 1;
    nGhost2[2][1] = 0;

    sayMPI << "Now testing GhostBuster from nGhost1 to nGhost2, with nGhost1=" << nGhost1 << " and nGhost2=" << nGhost2
           << "\n";
    myLittleLambda(nGhost1, nGhost2);
    myLittleLambda(nGhost2, nGhost1);

    // Less obvious test: same origin, different layout.
    nGhost1[0][0] = 0;
    nGhost1[0][1] = 5;
    nGhost1[1][0] = 0;
    nGhost1[1][1] = 3;
    nGhost1[2][0] = 0;
    nGhost1[2][1] = 1;

    nGhost2[0][0] = 0;
    nGhost2[0][1] = 4;
    nGhost2[1][0] = 0;
    nGhost2[1][1] = 2;
    nGhost2[2][0] = 0;
    nGhost2[2][1] = 0;

    sayMPI << "Now testing GhostBuster from nGhost1 to nGhost2, with nGhost1=" << nGhost1 << " and nGhost2=" << nGhost2
           << "\n";
    myLittleLambda(nGhost1, nGhost2);
    myLittleLambda(nGhost2, nGhost1);
  }
  return;

  // MPI test. This is a bit of an integration test, as it uses the MemoryManager and Field classes.
  for (size_t dir = 0; dir < NDim; ++dir) {
    const ptrdiff_t nGrid = 4;
    const ptrdiff_t nGhost = 1;

    auto toolBox = MemoryToolBox<3>::makeShared(nGrid, nGhost);

    auto mField = Field<3, double>("mField", toolBox);
    auto mOriginal = Field<3, double>("mOriginal", toolBox);

    SpatialCoordinate x(toolBox);
    mField = getVectorComponent(x, dir);
    mOriginal = mField; // copy the original.

    // verify the setup, before ghostbusting, no MPI communication yet.
    {
      auto mView = mField.getLocalNDHostView();
      auto oView = mOriginal.getLocalNDHostView();

      tdd.verify(mView.extent(0) == oView.extent(0));
      tdd.verify(mView.extent(1) == oView.extent(1));
      tdd.verify(mView.extent(2) == oView.extent(2));
      tdd.verify(mView.size() > 0);

      bool verifySetup = true;
      for (ptrdiff_t i = 0; i < mView.extent(0); ++i) {
        for (ptrdiff_t j = 0; j < mView.extent(1); ++j) {
          for (ptrdiff_t k = 0; k < mView.extent(2); ++k) {
            bool localRight = (mView(i, j, k) == oView(i, j, k)) &&
                              (mView(i, j, k) == x.get(nGhost + i, nGhost + j, nGhost + k, dir));
            verifySetup = verifySetup && localRight;
            if (!localRight) {
              std::stringstream ss;
              ss << "Setting up the test failed at " << i << ", " << j << ", " << k << "\n";
              ss << "mView: " << mView(i, j, k) << "\n";
              ss << "oView: " << oView(i, j, k) << "\n";
              ss << "x:     " << x.get(nGhost + i, nGhost + j, nGhost + k, dir) << "\n";
              sayMPI << ss.str();
            }
          }
        }
      }
      tdd.verify(verifySetup);
    }

    // Perform ghostbusting to FFT config space and back, to test MPI communication.
    auto mManager = mField.getMemoryManager();
    mManager->confirmConfigSpace();
    mManager->confirmFFTConfigSpace();
    mManager->confirmConfigSpace();
    mManager->confirmGhostsUpToDate();

    {
      auto mView = mField.getLocalNDHostView();
      auto oView = mOriginal.getLocalNDHostView();

      tdd.verify(mView.extent(0) == oView.extent(0));
      tdd.verify(mView.extent(1) == oView.extent(1));
      tdd.verify(mView.extent(2) == oView.extent(2));
      tdd.verify(mView.size() > 0);

      bool MPIallRight = true;
      for (ptrdiff_t i = 0; i < mView.extent(0); ++i) {
        for (ptrdiff_t j = 0; j < mView.extent(1); ++j) {
          for (ptrdiff_t k = 0; k < mView.extent(2); ++k) {
            bool localRight = (mView(i, j, k) == oView(i, j, k)) &&
                              (mView(i, j, k) == x.get(nGhost + i, nGhost + j, nGhost + k, dir));
            MPIallRight = MPIallRight && localRight;
            if (!localRight) {
              std::stringstream ss;
              ss << "MPI GhostBuster test failed at " << i << ", " << j << ", " << k << "\n";
              ss << "mView: " << mView(i, j, k) << "\n";
              ss << "oView: " << oView(i, j, k) << "\n";
              ss << "x:     " << x.get(nGhost + i, nGhost + j, nGhost + k, dir) << "\n";
              sayMPI << ss.str();
            }
          }
        }
      }
      tdd.verify(MPIallRight);
    }

    // Let's do a worse test: ghostbust to FFT config space, fourier transform back and forth, back to FFT config
    // space, and back to config space.
    mManager->confirmConfigSpace();
    mManager->confirmFFTConfigSpace();
    mManager->confirmFourierSpace();
    mManager->confirmFFTConfigSpace();
    mManager->confirmConfigSpace();
    mManager->confirmGhostsUpToDate();

    {
      auto mView = mField.getLocalNDHostView();
      auto oView = mOriginal.getLocalNDHostView();

      tdd.verify(mView.extent(0) == oView.extent(0));
      tdd.verify(mView.extent(1) == oView.extent(1));
      tdd.verify(mView.extent(2) == oView.extent(2));
      tdd.verify(mView.size() > 0);

      bool MPIallRight = true;
      for (ptrdiff_t i = 0; i < mView.extent(0); ++i) {
        for (ptrdiff_t j = 0; j < mView.extent(1); ++j) {
          for (ptrdiff_t k = 0; k < mView.extent(2); ++k) {
            bool localRight = (mView(i, j, k) == oView(i, j, k)) &&
                              (mView(i, j, k) == x.get(nGhost + i, nGhost + j, nGhost + k, dir));
            MPIallRight = MPIallRight && localRight;
            if (!localRight) {
              std::stringstream ss;
              ss << "MPI GhostBuster test failed at " << i << ", " << j << ", " << k << "\n";
              ss << "mView: " << mView(i, j, k) << "\n";
              ss << "oView: " << oView(i, j, k) << "\n";
              ss << "x:     " << x.get(nGhost + i, nGhost + j, nGhost + k, dir) << "\n";
              sayMPI << ss.str();
            }
          }
        }
      }
      tdd.verify(MPIallRight);
    }
  }
}

#endif
