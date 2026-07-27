
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/couplingsmanager.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct CouplingsManagerTester {
    static void Test(TDDAssertion &tdd);
  };

  void CouplingsManagerTester::Test(TDDAssertion &tdd)
  {
    // CouplingsManager only carries the compile-time coupling pattern; the effective charges g*Q live
    // in its Container, which is what a model instantiates for a given FloatType.
    using Couplings = CouplingsManager<2, 4, true, false, true, false, true, true, true, false>;

    // static assert instead of usual test to be sure everything is accessible at compile time.

    static_assert(Couplings::couples(0_c, 0_c) == true, "Should be accessible at compile time");
    static_assert(Couplings::couples(0_c, 1_c) == false, "Should be accessible at compile time");
    static_assert(Couplings::couples(0_c, 2_c) == true, "Should be accessible at compile time");
    static_assert(Couplings::couples(0_c, 3_c) == false, "Should be accessible at compile time");
    static_assert(Couplings::couples(1_c, 0_c) == true, "Should be accessible at compile time");
    static_assert(Couplings::couples(1_c, 1_c) == true, "Should be accessible at compile time");
    static_assert(Couplings::couples(1_c, 2_c) == true, "Should be accessible at compile time");
    static_assert(Couplings::couples(1_c, 3_c) == false, "Should be accessible at compile time");
    static_assert(Couplings::howManyCouples() == 5, "Should be accessible at compile time");

    // The Container mirrors the same compile-time pattern.
    using Container = Couplings::Container<double>;
    static_assert(Container::couples(1_c, 1_c) == true, "Should be accessible at compile time");
    static_assert(Container::howManyCouples() == 5, "Should be accessible at compile time");

    Container cm1;
    cm1.setEffectiveCharges(std::vector<double>{1, 2, 3, 4, 5}, std::vector<double>{1, 2, 3, 4});

    // Effective charge is g*Q: charges are consumed in order over the coupled pairs only,
    // while the coupling g is picked by gauge index.
    tdd.verify(cm1(0_c, 0_c) == 1);  // Q = 1, g = 1
    tdd.verify(cm1(0_c, 2_c) == 6);  // Q = 2, g = 3
    tdd.verify(cm1(1_c, 0_c) == 3);  // Q = 3, g = 1
    tdd.verify(cm1(1_c, 1_c) == 8);  // Q = 4, g = 2
    tdd.verify(cm1(1_c, 2_c) == 15); // Q = 5, g = 3

    // Uncoupled pairs carry no charge.
    tdd.verify(cm1(0_c, 1_c) == 0);
    tdd.verify(cm1(1_c, 3_c) == 0);

    // The gauge couplings g themselves are accessible separately.
    tdd.verify(cm1.coupling(0_c) == 1);
    tdd.verify(cm1.coupling(3_c) == 4);

    // Passing the wrong number of charges or couplings throws.
    tdd.verify(Throws<NotEnoughChargesForThisCouplingsManager>(
        [&]() { cm1.setEffectiveCharges(std::vector<double>{1, 1, 2, 3, 4, 5}, std::vector<double>{1, 2, 3, 4}); }));
    tdd.verify(Throws<NotEnoughCouplingsForThisCouplingsManager>(
        [&]() { cm1.setEffectiveCharges(std::vector<double>{1, 2, 3, 4, 5}, std::vector<double>{1, 3, 4}); }));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::CouplingsManagerTester> test;
}
