
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2026

#include "CosmoInterface/definitions/phaseBunchDavies.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"

namespace TempLat
{

  template <size_t NDim> struct PhaseBunchDaviesTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> void PhaseBunchDaviesTester<NDim>::Test(TDDAssertion &tdd)
  {
    const ptrdiff_t nGrid = 32, nGhost = 2;
    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    const double kIR = 0.1;
    const double aI = 1.0;

    // Test both gauge=true (BDPhasePi2A) and gauge=false (BDPhasePi2E)
    BDPhasePi2A<double, NDim> phaseA(toolBox, kIR, aI);
    BDPhasePi2E<double, NDim> phaseE(toolBox, kIR, aI);

    // Assign phases to fields — this exercises the variadic eval path
    Field<double, NDim> fieldA("fieldA", toolBox);
    Field<double, NDim> fieldE("fieldE", toolBox);

    fieldA.inFourierSpace() = phaseA;
    fieldE.inFourierSpace() = phaseE;

    // Verify that all values are finite
    {
      auto hostA = fieldA.inFourierSpace().getRawHostView();
      auto hostE = fieldE.inFourierSpace().getRawHostView();

      ptrdiff_t nFourier = 1;
      for (size_t d = 0; d < NDim - 1; ++d)
        nFourier *= nGrid;
      nFourier *= (nGrid / 2 + 1);
      const ptrdiff_t localFourier = nFourier / toolBox->getNProcesses();

      bool allFiniteA = true;
      bool allFiniteE = true;
      for (ptrdiff_t i = 0; i < localFourier; ++i) {
        allFiniteA &= std::isfinite(abs(hostA(i)));
        allFiniteE &= std::isfinite(abs(hostE(i)));
      }
      tdd.verify(allFiniteA, "BDPhasePi2A produces finite values");
      tdd.verify(allFiniteE, "BDPhasePi2E produces finite values");
    }

    // Verify that A and E phases are different (they use cos vs sin)
    {
      auto hostA = fieldA.inFourierSpace().getRawHostView();
      auto hostE = fieldE.inFourierSpace().getRawHostView();

      ptrdiff_t nFourier = 1;
      for (size_t d = 0; d < NDim - 1; ++d)
        nFourier *= nGrid;
      nFourier *= (nGrid / 2 + 1);
      const ptrdiff_t localFourier = nFourier / toolBox->getNProcesses();

      bool different = false;
      for (ptrdiff_t i = 0; i < localFourier; ++i) {
        if (!AlmostEqual(hostA(i), hostE(i), 1e-2)) different = true;
      }
      tdd.verify(different, "BDPhasePi2A and BDPhasePi2E produce different phases");
    }

    // Verify that the zero mode has magnitude 1 (cos^2 + sin^2 = 1 for k=0 => phase=0)
    // At k=0: gauge=true gives (cos(0), sin(0)) = (1, 0), gauge=false gives (sin(0), -cos(0)) = (0, -1)
    // Both should have magnitude 1
    {
      auto hostA = fieldA.inFourierSpace().getRawHostView();
      auto hostE = fieldE.inFourierSpace().getRawHostView();

      tdd.verify(AlmostEqual(abs(hostA(0)), 1.0), "BDPhasePi2A zero mode has unit magnitude");
      tdd.verify(AlmostEqual(abs(hostE(0)), 1.0), "BDPhasePi2E zero mode has unit magnitude");
    }

    // Verify determinism: creating the same phase twice gives identical results
    {
      BDPhasePi2A<double, NDim> phaseA2(toolBox, kIR, aI);
      Field<double, NDim> fieldA2("fieldA2", toolBox);
      fieldA2.inFourierSpace() = phaseA2;

      auto hostA = fieldA.inFourierSpace().getRawHostView();
      auto hostA2 = fieldA2.inFourierSpace().getRawHostView();

      ptrdiff_t nFourier = 1;
      for (size_t d = 0; d < NDim - 1; ++d)
        nFourier *= nGrid;
      nFourier *= (nGrid / 2 + 1);
      const ptrdiff_t localFourier = nFourier / toolBox->getNProcesses();

      bool identical = true;
      for (ptrdiff_t i = 0; i < localFourier; ++i)
        identical &= AlmostEqual(hostA(i), hostA2(i));
      tdd.verify(identical, "PhaseHelper is deterministic");
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::PhaseBunchDaviesTester<2>> test2d;
  TempLat::TDDContainer<TempLat::PhaseBunchDaviesTester<3>> test3d;
} // namespace
