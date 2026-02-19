
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025
#include "TempLat/lattice/ghostcells/ghostbuster.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"

#include <sstream>
#include <iostream>
#include <functional>

namespace TempLat
{

  namespace Testing
  {
    /* single datum on a grid: 24 bytes making up x, y, and z. */
    struct datum {
      ptrdiff_t x, y, z;
    };
  } // namespace Testing

  template <size_t NDim> void run_nd_test(TDDAssertion &tdd)
  {
    sayMPI << "Testing GhostBuster in " << NDim << " dimensions.\n";

    // Set up grid sizes for each dimension (smaller than 3D test to reduce complexity)
    device::IdxArray<NDim> nGrid;
    for (size_t i = 0; i < NDim; ++i) {
      nGrid[i] = 8; // Small grid size for all dimensions
    }

    // Create layout
    device::IdxArray<NDim> globalSizes;
    for (size_t i = 0; i < NDim; ++i) {
      globalSizes[i] = 16; // Global size larger than local
    }
    LayoutStruct<NDim> layout(globalSizes, 0);
    layout.setLocalSizes(nGrid);

    // Define two different ghost layouts to test transformation between them
    device::array<device::IdxArray<2>, NDim> nGhost1{};
    device::array<device::IdxArray<2>, NDim> nGhost2{};

    // Set up asymmetric ghost configurations for testing
    for (size_t i = 0; i < NDim; ++i) {
      nGhost1[i][0] = 2; // left ghost cells
      nGhost1[i][1] = 1; // right ghost cells
      nGhost2[i][0] = 1; // left ghost cells
      nGhost2[i][1] = 2; // right ghost cells
    }

    auto testGhostTransformation = [&](const auto &nGhostFrom, const auto &nGhostTo) {
      // Calculate memory sizes needed for both layouts
      ptrdiff_t memSize1 = 1, memSize2 = 1;
      for (size_t i = 0; i < NDim; ++i) {
        memSize1 *= (nGrid[i] + nGhostFrom[i][0] + nGhostFrom[i][1]);
        memSize2 *= (nGrid[i] + nGhostTo[i][0] + nGhostTo[i][1]);
      }

      LayoutStruct<NDim> layoutFrom(layout);
      layoutFrom.setPadding(nGhostFrom);
      LayoutStruct<NDim> layoutTo(layout);
      layoutTo.setPadding(nGhostTo);

      MemoryBlock<NDim, Testing::datum> memory(std::max(memSize1, memSize2));

      // Initialize memory with coordinate values
      {
        auto memory_view = memory.getRawHostView();

        // Use a lambda to handle N-dimensional initialization recursively
        std::function<void(device::IdxArray<NDim> &, size_t, ptrdiff_t)> initMemory =
            [&](device::IdxArray<NDim> &coords, size_t dim, ptrdiff_t offset) {
              if (dim == NDim) {
                // Base case: set the memory value
                memory_view[offset].x = coords[0];
                memory_view[offset].y = (NDim > 1) ? coords[1] : 0;
                memory_view[offset].z = (NDim > 2) ? coords[2] : 0;
                return;
              }

              ptrdiff_t stride = 1;
              for (size_t j = dim + 1; j < NDim; ++j) {
                stride *= (nGrid[j] + nGhostFrom[j][0] + nGhostFrom[j][1]);
              }

              for (ptrdiff_t i = -nGhostFrom[dim][0]; i < nGrid[dim] + nGhostFrom[dim][1]; ++i) {
                coords[dim] = i;
                ptrdiff_t newOffset = offset + (i + nGhostFrom[dim][0]) * stride;
                initMemory(coords, dim + 1, newOffset);
              }
            };

        device::IdxArray<NDim> coords{};
        initMemory(coords, 0, 0);
        memory.pushHostView();
      }

      // Perform ghost transformation
      GhostBuster<NDim> ghostBuster(layoutFrom, layoutTo);
      ghostBuster(memory);

      // Verify the transformation
      memory.flagHostMirrorOutdated();
      auto memory_view = memory.getRawHostView();

      bool allCorrect = true;

      // Use recursive lambda to verify N-dimensional memory
      std::function<void(device::IdxArray<NDim> &, size_t)> verifyMemory = [&](device::IdxArray<NDim> &coords,
                                                                               size_t dim) {
        if (dim == NDim) {
          // Base case: check this coordinate
          ptrdiff_t pos = layoutTo.getOrigin();
          for (size_t i = 0; i < NDim; ++i) {
            pos += layoutTo.stride(i) * coords[i];
          }

          const Testing::datum &dat = memory_view[pos];
          bool thisCorrect = (dat.x == coords[0]) && (dat.y == ((NDim > 1) ? coords[1] : 0)) &&
                             (dat.z == ((NDim > 2) ? coords[2] : 0));
          allCorrect = allCorrect && thisCorrect;
          return;
        }

        for (ptrdiff_t i = 0; i < nGrid[dim]; ++i) {
          coords[dim] = i;
          verifyMemory(coords, dim + 1);
        }
      };

      device::IdxArray<NDim> coords{};
      verifyMemory(coords, 0);

      return allCorrect;
    };

    // Test transformation from layout 1 to layout 2
    bool nd_test1 = testGhostTransformation(nGhost1, nGhost2);
    tdd.verify(nd_test1);

    // Test transformation from layout 2 to layout 1 (reverse)
    bool nd_test2 = testGhostTransformation(nGhost2, nGhost1);
    tdd.verify(nd_test2);

    // Test with uniform ghost layout
    device::array<device::IdxArray<2>, NDim> nGhostUniform{};
    for (size_t i = 0; i < NDim; ++i) {
      nGhostUniform[i][0] = 1;
      nGhostUniform[i][1] = 1;
    }

    bool nd_test3 = testGhostTransformation(nGhost1, nGhostUniform);
    tdd.verify(nd_test3);

    bool nd_test4 = testGhostTransformation(nGhostUniform, nGhost2);
    tdd.verify(nd_test4);
  }

  template <size_t NDim> struct GhostBusterTester {
    static void Test(TDDAssertion &tdd);
  };

  // File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
  template <size_t NDim> void GhostBusterTester<NDim>::Test(TDDAssertion &tdd)
  {
    // Test the ghostbuster on a single node.
    {
      // arbitrary irregular sizing. If you want to see debug prints of what happens, set them to <= 4
      device::IdxArray<3> nGrid{{256, 64, 128}};

      LayoutStruct<3> layout({62, 62, 62}, 0);
      layout.setLocalSizes(nGrid);

      auto &&myLittleLambda = [&](auto nGhost, auto nGhostB) {
        ptrdiff_t memSize1 = (nGrid[0] + nGhost[0][0] + nGhost[0][1]) * (nGrid[1] + nGhost[1][0] + nGhost[1][1]) *
                             (nGrid[2] + nGhost[2][0] + nGhost[2][1]);

        ptrdiff_t memSize2 = (nGrid[0] + nGhostB[0][0] + nGhostB[0][1]) * (nGrid[1] + nGhostB[1][0] + nGhostB[1][1]) *
                             (nGrid[2] + nGhostB[2][0] + nGhostB[2][1]);

        LayoutStruct<3> layoutFrom(layout);
        layoutFrom.setPadding(nGhost);
        LayoutStruct<3> layoutTo(layout);
        layoutTo.setPadding(nGhostB);

        MemoryBlock<3, Testing::datum> memory(std::max(memSize1, memSize2));

        // setup the controlled known memory; each entry equals its position
        {
          auto memory_view = memory.getRawHostView();
          for (ptrdiff_t i = -nGhost[0][0]; i < nGrid[0] + nGhost[0][1]; ++i) {
            ptrdiff_t iPos = (i + nGhost[0][0]) * (nGrid[1] + nGhost[1][0] + nGhost[1][1]) *
                             (nGrid[2] + nGhost[2][0] + nGhost[2][1]);
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
          device::IdxArray<3> fullSizes;
          for (size_t i = 0; i < 3; ++i) {
            fullSizes[i] = nGrid[i] + gh[i][0] + gh[i][1];
            // no debug print for too large arrays
            if (fullSizes[i] > 4) return;
          }

          std::cout << "\n" << name << "\n";

          memory.flagHostMirrorOutdated();
          auto full_view = memory.getNDHostView(fullSizes);

          auto sub_view = device::memory::subview(full_view, std::make_pair(gh[0][0], gh[0][0] + nGrid[0]),
                                                  std::make_pair(gh[1][0], gh[1][0] + nGrid[1]),
                                                  std::make_pair(gh[2][0], gh[2][0] + nGrid[2]));

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

          GhostBuster<3> egon(x == 0 ? layoutFrom : layoutTo, x == 0 ? layoutTo : layoutFrom);

          egon(memory);

          LayoutStruct<3> curLayout(x == 0 ? layoutTo : layoutFrom);

          if (x == 0)
            print_it(nGhostB, "Memory at x=0 AFTER GhostBuster:\n");
          else
            print_it(nGhost, "Memory at x=1 AFTER GhostBuster back:\n");

          memory.flagHostMirrorOutdated();
          auto memory_view = memory.getRawHostView();

          // verify the setup, assuming strides are correct (tested elsewhere),
          // verify that the GhostBuster did not damage the memory, and put stuff in their new correct place.
          for (ptrdiff_t i = 0; i < nGrid[0]; ++i) {
            for (ptrdiff_t j = 0; j < nGrid[1]; ++j) {
              for (ptrdiff_t k = 0; k < nGrid[2]; ++k) {
                ptrdiff_t pos =
                    curLayout.getOrigin() + curLayout.stride(0) * i + curLayout.stride(1) * j + curLayout.stride(2) * k;
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
      device::array<device::IdxArray<2>, 3> nGhost1{};

      nGhost1[0][0] = 6;
      nGhost1[0][1] = 5;
      nGhost1[1][0] = 4;
      nGhost1[1][1] = 3;
      nGhost1[2][0] = 2;
      nGhost1[2][1] = 1;

      device::array<device::IdxArray<2>, 3> nGhost2{};

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

      sayMPI << "Now testing GhostBuster from nGhost1 to nGhost2, with nGhost1=" << nGhost1
             << " and nGhost2=" << nGhost2 << "\n";
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

      sayMPI << "Now testing GhostBuster from nGhost1 to nGhost2, with nGhost1=" << nGhost1
             << " and nGhost2=" << nGhost2 << "\n";
      myLittleLambda(nGhost1, nGhost2);
      myLittleLambda(nGhost2, nGhost1);
    }

    // quick N-dimensional test
    constexpr_for<1, 5>([&](auto N) {
      constexpr size_t nd = decltype(N)::value;
      run_nd_test<nd>(tdd);
    });
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::GhostBusterTester<3>> test;
}
