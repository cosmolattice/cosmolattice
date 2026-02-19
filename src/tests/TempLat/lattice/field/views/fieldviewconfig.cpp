
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/field/views/fieldviewconfig.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/util/ndloop.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"

namespace TempLat
{

  template <size_t NDim, typename T> struct ConfigViewTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim, typename T> inline void ConfigViewTester<NDim, T>::Test(TDDAssertion &tdd)
  {
    const ptrdiff_t nGrid = 16, nGhost = 2;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    Field<NDim, T> a("a", toolBox);
    Field<NDim, T> b("b", toolBox);

    constexpr T value = 100. * nGrid * NDim * nGhost;

    a = value;
    b = value;

    tdd.verify(HasSpaceConfirmationMethods<Field<NDim, T>, NDim>);

    // get host views
    auto a_host = a.getLocalNDHostView();
    auto b_host = b.getLocalNDHostView();

    bool same = true;
    NDLoop<NDim>(a_host, [&](const auto &...idx) {
      same = same && AlmostEqual(a_host(idx...), b_host(idx...)) && AlmostEqual(a_host(idx...), value);
    });
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
  TempLat::TDDContainer<TempLat::ConfigViewTester<1, double>> test1;
  TempLat::TDDContainer<TempLat::ConfigViewTester<2, double>> test2;
  TempLat::TDDContainer<TempLat::ConfigViewTester<3, double>> test3;
  TempLat::TDDContainer<TempLat::ConfigViewTester<4, double>> test4;
  TempLat::TDDContainer<TempLat::ConfigViewTester<1, float>> test1f;
  TempLat::TDDContainer<TempLat::ConfigViewTester<2, float>> test2f;
  TempLat::TDDContainer<TempLat::ConfigViewTester<3, float>> test3f;
  TempLat::TDDContainer<TempLat::ConfigViewTester<4, float>> test4f;
} // namespace
