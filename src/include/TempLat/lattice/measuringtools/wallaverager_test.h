#ifndef TEMPLAT_LATTICE_MEASUREMENTS_WALLAVERAGER_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_WALLAVERAGER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2025

inline void TempLat::WallAveragerTester::Test(TempLat::TDDAssertion &tdd)
{
  // TODO (Franz)
  /*
  static constexpr size_t NDim = 4;

  struct myTmpStruct {
    myTmpStruct() : mt(MemoryToolBox<NDim>::makeShared(16, 1)) {}
    double get(ptrdiff_t i)
    {
      auto ii = mt->getCoordConfiguration0N(i);
      return ii[0] * pow(16, 3) + ii[1] * pow(16, 2) + ii[2] * 16 + ii[3];
    }

    double expectedAnswer(int l)
    {
      return (0.5 * (15) * 16 * (pow(16, 2) + 16 + 1)) + l; // sum_0^N-1 = (N-1)N/2
    }

    double dummy_coord(ptrdiff_t i, ptrdiff_t j, ptrdiff_t k, ptrdiff_t l)
    {
      return i * pow(16, 3) + j * pow(16, 2) + k * 16 + l;
    }

    auto getToolBox() const { return mt; }
    void confirmSpace(const LayoutStruct &newLayout, const SpaceStateType &spaceType) const {}
    device::memory::host_ptr<MemoryToolBox> mt;
    std::string toString() const { return "myTmpStruct"; }
  };

  myTmpStruct myInstance;

  std::array<double, 16> wallx;
  std::array<double, 16> wally;
  std::array<double, 16> wallz;
  std::array<double, 16> wallt;

  for (int i = 0; i < 16; ++i) {
    wallx[i] = 0;
    wally[i] = 0;
    wallz[i] = 0;
    wallt[i] = 0;
  }

  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 16; ++j) {
      for (int k = 0; k < 16; ++k) {
        for (int l = 0; l < 16; ++l) {
          wallx[i] += myInstance.dummy_coord(i, j, k, l);
          wally[j] += myInstance.dummy_coord(i, j, k, l);
          wallz[k] += myInstance.dummy_coord(i, j, k, l);
          wallt[l] += myInstance.dummy_coord(i, j, k, l);
        }
      }
    }
  }

  auto avs = wallAverager(myInstance);
  avs.compute();
  auto nwallx = avs.getWall(0);
  auto nwally = avs.getWall(1);
  auto nwallz = avs.getWall(2);
  auto nwallt = avs.getWall(3);

  for (int i = 0; i < 15; ++i) {
    tdd.verify(AlmostEqual(nwallx[i], wallx[i] / pow(16, 3)));
    tdd.verify(AlmostEqual(nwally[i], wally[i] / pow(16, 3)));
    tdd.verify(AlmostEqual(nwallz[i], wallz[i] / pow(16, 3)));
    tdd.verify(AlmostEqual(nwallt[i], wallt[i] / pow(16, 3)));

    say << i;
    say << nwallt[i];
    say << wallt[i] / pow(16, 3);
  }
*/
}

#endif
