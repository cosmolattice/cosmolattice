
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#include "TempLat/lattice/algebra/complexalgebra/complexfieldtester.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/lattice/algebra/complexalgebra/complexalgebra.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"

namespace TempLat
{

  struct ComplexFieldTester {
    static void Test(TDDAssertion &tdd);
  };

  void ComplexFieldTester::Test(TDDAssertion &tdd)
  {
    constexpr size_t NDim = 3;
    constexpr int nGrid = 32;
    constexpr int nGhost = 0;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    WaveNumber k(toolBox);

    ComplexField<NDim, double> phi("phi", toolBox);
    ComplexField<NDim, double> xi("xi", toolBox);

    tdd.verify(phi.inFourierSpace()(0_c).isFourierSpace());
    tdd.verify(phi.inFourierSpace()(1_c).isFourierSpace());

    auto test = k(1_c) * phi.inFourierSpace();

    say << test;

    xi.inFourierSpace() = k(1_c) * phi.inFourierSpace();
    xi.inFourierSpace() = xi.inFourierSpace() * phi.inFourierSpace();

    tdd.verify(xi.inFourierSpace()(0_c).isFourierSpace());
    tdd.verify(xi.inFourierSpace()(1_c).isFourierSpace());

    xi = phi;
    tdd.verify(xi(0_c).isConfigSpace());
    tdd.verify(xi(1_c).isConfigSpace());
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ComplexFieldTester> test;
}
