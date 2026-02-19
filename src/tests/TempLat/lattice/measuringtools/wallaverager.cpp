
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2025
#include "TempLat/lattice/measuringtools/wallaverager.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"

namespace TempLat
{

  struct WallAveragerTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t _NDim> struct WallAveragerTestStruct {
    static constexpr size_t NDim = _NDim;

    WallAveragerTestStruct() : mt(MemoryToolBox<NDim>::makeShared(16, 1)), mLayout(mt->mLayouts.getConfigSpaceLayout())
    {
    }

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION double get(const IDX &...idx) const
    {
      device::IdxArray<NDim> ii;
      mLayout.putSpatialLocationFromMemoryIndexInto0N(ii, idx...);
      return ii[0] * pow(16, 3) + ii[1] * pow(16, 2) + ii[2] * 16 + ii[3];
    }

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION double eval(const IDX &...idx) const
    {
      return get(idx...);
    }

    auto getToolBox() const { return mt; }
    void confirmSpace(const LayoutStruct<NDim> &, const SpaceStateType &) const {}
    device::memory::host_ptr<MemoryToolBox<NDim>> mt;
    LayoutStruct<NDim> mLayout;
    std::string toString() const { return "WallAveragerTestStruct"; }
  };

  void WallAveragerTester::Test(TDDAssertion &tdd)
  {
    static constexpr size_t NDim = 4;
    WallAveragerTestStruct<NDim> myInstance;

    // Compute expected wall averages manually
    auto dummy_coord = [](int i, int j, int k, int l) -> double {
      return i * pow(16, 3) + j * pow(16, 2) + k * 16 + l;
    };

    std::array<std::array<double, 16>, NDim> expected{};
    for (auto &wall : expected)
      wall.fill(0.0);

    for (int i = 0; i < 16; ++i)
      for (int j = 0; j < 16; ++j)
        for (int k = 0; k < 16; ++k)
          for (int l = 0; l < 16; ++l) {
            double val = dummy_coord(i, j, k, l);
            expected[0][i] += val;
            expected[1][j] += val;
            expected[2][k] += val;
            expected[3][l] += val;
          }

    // Compute wall averages using WallAverager
    auto avs = wallAverager(myInstance);
    avs.compute();

    double codim = pow(16, 3); // N^(d-1) for isotropic 16^4 lattice
    for (size_t t = 0; t < NDim; ++t) {
      auto wall = avs.getWall(t);
      for (int i = 0; i < 16; ++i) {
        tdd.verify(AlmostEqual(wall[i], expected[t][i] / codim));
      }
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::WallAveragerTester> test;
}
