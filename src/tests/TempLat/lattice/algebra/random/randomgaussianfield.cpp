
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"

namespace TempLat
{

  struct RandomGaussianFieldTester {
    static void Test(TDDAssertion &tdd);
  };

  void RandomGaussianFieldTester::Test(TDDAssertion &tdd)
  {
    /* test the stability of the getter at various coordinates. */

    const ptrdiff_t nGrid = 32, nGhost = 2;
    const ptrdiff_t fourierGridPoints = nGrid * nGrid * (nGrid / 2 + 1); // +1 for the zero frequency.
    auto toolBox = MemoryToolBox<3>::makeShared(nGrid, nGhost);
    const ptrdiff_t localFourierGridPoints = fourierGridPoints / toolBox->getNProcesses();

    RandomGaussianField<3, double> myField("Hello world", toolBox);

    Field<3, double> a("a", toolBox);
    Field<3, double> b("b", toolBox);

    // Get random values
    a.inFourierSpace() = myField;

    // Get more random values
    b.inFourierSpace() = myField;

    {
      auto a_host = a.inFourierSpace().getRawHostView();
      auto b_host = b.inFourierSpace().getRawHostView();

      // Check that the values are different
      bool different = false;
      for (ptrdiff_t i = 0; i < localFourierGridPoints; ++i) {
        // show the first few values for debugging
        if (i < 8) std::cout << "index " << i << " a: " << a_host(i) << " b: " << b_host(i) << "\n";
        bool local =
            !AlmostEqual(a_host(i), b_host(i), 1e-2) && std::isfinite(abs(a_host(i))) && std::isfinite(abs(b_host(i)));
        different = local || different;
      }
      tdd.verify(different);
    }

    // test rewinding
    myField.reset(); // reset the field to the initial generation.
    b.inFourierSpace() = myField;

    {
      auto a_host = a.inFourierSpace().getRawHostView();
      auto b_host = b.inFourierSpace().getRawHostView();

      // Check that the values are identical
      bool rewinding = true;
      for (ptrdiff_t i = 0; i < localFourierGridPoints; ++i)
        rewinding &=
            AlmostEqual(a_host(i), b_host(i)) && std::isfinite(abs(a_host(i))) && std::isfinite(abs(b_host(i)));
      tdd.verify(rewinding);
    }

    {
      // Test saveState/loadState round-trip
      RandomGaussianField<3, double> rng("serialization_test", toolBox);

      // Generate some values after saving state
      Field<3, double> field2("field2", toolBox);
      field2.inFourierSpace() = rng;

      std::string savedState = rng.saveState();

      // Generate again, to see we get different values
      Field<3, double> field1("field1", toolBox);
      field1.inFourierSpace() = rng;

      RandomGaussianField<3, double> rng2("different_seed", toolBox);
      rng2.loadState(savedState);

      tdd.verify(rng.getCurrentSeed() == rng2.getCurrentSeed());
      tdd.verify(savedState == rng2.saveState());

      // Generate again from rng2, to see we get same values as field1
      Field<3, double> field3("field3", toolBox);
      field3.inFourierSpace() = rng2;

      auto field1_host = field1.inFourierSpace().getRawHostView();
      auto field2_host = field2.inFourierSpace().getRawHostView();
      auto field3_host = field3.inFourierSpace().getRawHostView();

      bool match13 = true;
      bool match12 = true;
      for (ptrdiff_t i = 0; i < localFourierGridPoints; ++i) {
        // Skip the zeros
        if (!(abs(field1_host(i)) < 1e-14 && abs(field3_host(i)) < 1e-14))
          match13 &= AlmostEqual(field1_host(i), field3_host(i)) && std::isfinite(abs(field1_host(i))) &&
                     std::isfinite(abs(field3_host(i)));
        match12 &= AlmostEqual(field1_host(i), field2_host(i), 1e-2) && std::isfinite(abs(field1_host(i))) &&
                   std::isfinite(abs(field2_host(i)));

        if (!(AlmostEqual(field1_host(i), field3_host(i)) && std::isfinite(abs(field1_host(i))) &&
              std::isfinite(abs(field3_host(i)))))
          sayMPI << "Error at index " << i << ", field1: " << field1_host(i) << ", field3: " << field3_host(i)
                 << ", data did not match after save/load.\n";
      }
      tdd.verify(match13, "saveState/loadState round-trip produces identical sequence");
      tdd.verify(!match12, "Different seeds produce different sequences");
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::RandomGaussianFieldTester> test;
}
