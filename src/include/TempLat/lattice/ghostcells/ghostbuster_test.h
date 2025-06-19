#ifndef TEMPLAT_LATTICE_MEMORY_GHOSTBUSTER_TEST_H
#define TEMPLAT_LATTICE_MEMORY_GHOSTBUSTER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

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
  /* Perhaps a bit elaborate, but it is as consistent as it gets.. */

  /* arbitrary irregular sizing */
  std::array<ptrdiff_t, 3> nGrid{{64, 48, 128}};

  LayoutStruct<3> layout({62, 62, 62});
  layout.setLocalSizes(nGrid);

  auto &&myLittleLambda = [&](auto nGhost, auto nGhostB) {
    ptrdiff_t memSize1 = (nGrid[0] + nGhost[0][0] + nGhost[0][1]) * (nGrid[1] + nGhost[1][0] + nGhost[1][1]) *
                         (nGrid[2] + nGhost[2][0] + nGhost[2][1]);

    ptrdiff_t memSize2 = (nGrid[0] + nGhostB[0][0] + nGhostB[0][1]) * (nGrid[1] + nGhostB[1][0] + nGhostB[1][1]) *
                         (nGrid[2] + nGhostB[2][0] + nGhostB[2][1]);

    JumpsHolder<3> jumperFrom(layout, nGhost);
    JumpsHolder<3> jumperTo(layout, nGhostB);

    MemoryBlock<3, Testing::datum> memory(std::max(memSize1, memSize2));

#ifdef CHECKBOUNDS
    /* where is the last non-ghost entry? */
    ptrdiff_t last1 =
        jumperFrom.toOrigin() + jumperFrom.getJump(layout.getSizesInMemory()) +
        jumperFrom.getJump({{-1, -1, -1}}); /* can use getJump because getSizesInMemory is not transposed. */
    ptrdiff_t last2 =
        jumperTo.toOrigin() + jumperTo.getJump(layout.getSizesInMemory()) +
        jumperTo.getJump({{-1, -1, -1}}); /* can use getJump because getSizesInMemory is not transposed. */
    std::vector<Testing::datum> smallMemory(std::max(last1, last2));

    tdd.verify(Throws<GhostBusterBoundsException>([&]() { GhostBuster(jumperTo, jumperFrom)(smallMemory); }));
#endif
    {
      auto memory_view = memory.getRawHostView();
      /* setup the controlled known memory; each entry equals its position */
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
      memory.pushHostView(); /* push the memory to the device. */
    }

    bool allRight = true;
    /* test once forward, once back. */
    for (int x = 0; x < 2; ++x) {

      GhostBuster<3> egon(x == 0 ? jumperFrom : jumperTo, x == 0 ? jumperTo : jumperFrom);

      egon(memory);

      JumpsHolder<3> jumper(x == 0 ? jumperTo : jumperFrom);

      auto memory_view = memory.getRawHostView();

      /* verify the setup, not controlled,
       assuming jumps are correct (tested elsewhere),
       verify that the GhostBuster did not damage the memory,
       and put stuff in their new correct place. */
      for (ptrdiff_t i = 0; i < nGrid[0]; ++i) {
        for (ptrdiff_t j = 0; j < nGrid[1]; ++j) {
          for (ptrdiff_t k = 0; k < nGrid[2]; ++k) {
            ptrdiff_t pos = jumper.toOrigin() + jumper.getJumpsInMemoryOrder()[0] * i +
                            jumper.getJumpsInMemoryOrder()[1] * j + jumper.getJumpsInMemoryOrder()[2] * k;
            const Testing::datum &dat = memory_view[pos];
            allRight = allRight && dat.x == i && dat.y == j && dat.z == k;
            // std::cerr << i << ", " << j << ", " << k << " => " << dat.x << ", " << dat.y << ", " << dat.z << "\n";
            // if (!allRight) exit(0);
          }
        }
      }
    }

    tdd.verify(allRight);
  };

  /* arbitrary irregular ghosting A */
  std::array<std::array<ptrdiff_t, 2u>, 3> nGhost1{};

  nGhost1[0][0] = 6;
  nGhost1[0][1] = 5;
  nGhost1[1][0] = 4;
  nGhost1[1][1] = 3;
  nGhost1[2][0] = 2;
  nGhost1[2][1] = 1;

  /* arbitrary irregular ghosting B: by choice slightly smaller than A, to be safe. */
  std::array<std::array<ptrdiff_t, 2u>, 3> nGhost2{};
  nGhost2[0][0] = 5;
  nGhost2[0][1] = 6;
  nGhost2[1][0] = 1;
  nGhost2[1][1] = 1;
  nGhost2[2][0] = 5;
  nGhost2[2][1] = 2;

  tdd.verify(Throws<GhostBusterOrderException>([&]() { myLittleLambda(nGhost1, nGhost2); }));

  nGhost2[0][0] = 5;
  nGhost2[0][1] = 4;
  nGhost2[1][0] = 3;
  nGhost2[1][1] = 2;
  nGhost2[2][0] = 1;
  nGhost2[2][1] = 0;

  myLittleLambda(nGhost1, nGhost2);

  myLittleLambda(nGhost2, nGhost1);

  /* Less obvious test: same origin, different layout. */
  nGhost1[0][0] = 0;
  nGhost1[1][0] = 0;
  nGhost1[2][0] = 0;
  nGhost2 = nGhost1;

  nGhost2[0][1]--;
  nGhost2[1][1]--;
  nGhost2[2][1]--;

  myLittleLambda(nGhost1, nGhost2);

  myLittleLambda(nGhost2, nGhost1);
}

#endif
