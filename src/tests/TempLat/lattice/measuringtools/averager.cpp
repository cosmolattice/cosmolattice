
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/measuringtools/averager.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

  struct AveragerTester {
    static void Test(TDDAssertion &tdd);
  };

  struct myWorkspace {
    myWorkspace() : value(0.) {}
    myWorkspace &operator+=(const myWorkspace &other)
    {
      value += other.value;
      return *this;
    }
    myWorkspace &operator+=(const double &other)
    {
      value += other;
      return *this;
    }
    std::string toString() const { return "myWorkspace"; }
    double value;
  };

  template <size_t _NDim> struct myTmpStruct {
    static constexpr size_t NDim = _NDim;
    myTmpStruct() : mt(MemoryToolBox<NDim>::makeShared(16, 1)) {}

    template <typename... IDX> DEVICE_FORCEINLINE_FUNCTION double eval(const IDX &...idx) const { return NDim; }

    auto getToolBox() const { return mt; }

    void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const {}

    device::memory::host_ptr<MemoryToolBox<NDim>> mt;

    std::string toString() const { return "myTmpStruct"; }
  };

  template <size_t _NDim> struct myTmpStructComplex {
    static constexpr size_t NDim = _NDim;
    myTmpStructComplex(ptrdiff_t ngr)
        : mt(MemoryToolBox<NDim>::makeShared(ngr, 1)), mLayout(mt->mLayouts.getFourierSpaceLayout())
    {
    }

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION complex<double> eval(const IDX &...idx) const
    {
      device::IdxArray<NDim> global_coord;
      mLayout.putSpatialLocationFromMemoryIndexInto(global_coord, idx...);
      double imagPart =
          mLayout.getHermitianPartners().qualify(global_coord) == HermitianRedundancy::realValued ? 0. : NDim;
      return complex<double>(1., imagPart);
    }

    auto getToolBox() const { return mt; }

    void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const {}

    device::memory::host_ptr<MemoryToolBox<NDim>> mt;

    LayoutStruct<NDim> mLayout;
  };

  void AveragerTester::Test(TDDAssertion &tdd)
  {
    myTmpStruct<3> myInstance;

    auto aget = getAverager(myInstance);
    say << "result of " << aget << ": " << aget.compute() << "\n";

    tdd.verify(AlmostEqual(aget.compute(), 3));

    auto myLambda = [&](auto dim, ptrdiff_t ngr_) {
      constexpr size_t NDim = decltype(dim)::value;
      myTmpStructComplex<NDim> myInstanceCp(ngr_);

      auto agetCp = getAverager(myInstanceCp);

      auto result = agetCp.compute();

      say << result << " vs hypothetical " << complex<double>(1., NDim) << "\n";

      tdd.verify(AlmostEqual(result, complex<double>(1., NDim)));
      if (!AlmostEqual(result, complex<double>(1., NDim))) {
        std::cout << " got " << result << ", expected " << complex<double>(1., NDim) << std::endl;
      }
    };

    myLambda(Tag<4>(), 16);
    myLambda(Tag<3>(), 32);
    myLambda(Tag<2>(), 64);

#ifndef HAVE_MPI
    /** this one fails correctly under MPI: each process would do the full 1d rod. */
    myLambda(Tag<1>(), 12);
#endif
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::AveragerTester> test;
}
