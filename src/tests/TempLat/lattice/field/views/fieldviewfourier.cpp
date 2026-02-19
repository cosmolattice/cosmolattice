
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/field/views/fieldviewfourier.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/util/ndloop.h"

namespace TempLat
{

  template <size_t NDim, typename T> struct FourierViewTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim, typename T> inline void FourierViewTester<NDim, T>::Test(TDDAssertion &tdd)
  {
    const ptrdiff_t nGrid = 16, nGhost = 1;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);
    toolBox->setVerbose();

    Field<NDim, T> a("a", toolBox);
    Field<NDim, T> x("x", toolBox);

    constexpr T value = 100. * nGrid * NDim * nGhost;

    a.inFourierSpace() = value;
    x.inFourierSpace() = value;

    // get host views
    auto a_host = a.inFourierSpace().getLocalNDHostView();
    auto x_host = x.inFourierSpace().getLocalNDHostView();

    bool same = true;
    NDLoop<NDim>(a_host, [&](const auto &...idx) { same = same && AlmostEqual(a_host(idx...), x_host(idx...)); });
    tdd.verify(same);

    /*ptrdiff_t nGrid = 256, nGhost = 2;

    auto toolBox = MemoryToolBox::makeShared(3, nGrid, nGhost);

    toolBox->setVerbose();

    FieldChainFinal<T> phiBase("phi", toolBox);
    FieldViewFourier<T> phi(phiBase);*/

    /*std::atomic<bool> lastDimPositiveDefinite(true);

    WaveNumber k;

    phi.iterate( [&](IterationCoordinates &pIterCoords) {
        bool localSuccess = pIterCoords[2] >= 0;*/
    /* atomically put the new value in place, but if we had success, it is important
      that we don't overwrite another thread's failure. */
    /*bool expected = true;
    lastDimPositiveDefinite.compare_exchange_weak(expected, localSuccess);
    //        say << offset << " localSuccess: " << localSuccess << " pIterCoords[2]: " << pIterCoords[2] << "\n";
    return localSuccess;
    });

    tdd.verify(lastDimPositiveDefinite == true);*/
  }

} // namespace TempLat

namespace
{
  // TempLat::TDDContainer<TempLat::FourierViewTester<1, double>> test1;
  TempLat::TDDContainer<TempLat::FourierViewTester<2, double>> test2;
  TempLat::TDDContainer<TempLat::FourierViewTester<3, double>> test3;
  TempLat::TDDContainer<TempLat::FourierViewTester<4, double>> test4;

#ifdef HAVE_FFTFLOAT
  TempLat::TDDContainer<TempLat::FourierViewTester<1, float>> test1f;
  TempLat::TDDContainer<TempLat::FourierViewTester<2, float>> test2f;
  TempLat::TDDContainer<TempLat::FourierViewTester<3, float>> test3f;
  TempLat::TDDContainer<TempLat::FourierViewTester<4, float>> test4f;
#endif
} // namespace
