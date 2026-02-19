/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025
#include "TempLat/util/tuple_tools.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/constexpr_for.h"

namespace TempLat
{

  struct TupleToolsTester {
    static void Test(TDDAssertion &tdd);
  };

  void TupleToolsTester::Test(TDDAssertion &tdd)
  {
    const auto test_tup = device::tuple(1, 2, 3, 4, 5, 6, 5, 8, 9, 10);

    constexpr_for<0, 10>([&](const auto _d) {
      auto temp_res = device::tuple(1, 2, 3, 4, 5, 6, 5, 8, 9, 10);
      constexpr auto idx = decltype(_d)::value;
      // sayShort << "idx = " << idx << "\n";
      temp_res = tuple_add_to_nth<idx, 11>(temp_res);
      constexpr_for<0, 10>([&](const auto _e) {
        constexpr auto jdx = decltype(_e)::value;
        // sayShort << "jdx = " << jdx << "\n";
        if constexpr (jdx == idx) {
          tdd.verify(device::get<jdx>(temp_res) == device::get<jdx>(test_tup) + 11);
          if (!(device::get<jdx>(temp_res) == device::get<jdx>(test_tup) + 11))
            say << "Value evaluated to " << device::get<jdx>(temp_res) << ", should be "
                << device::get<jdx>(test_tup) + 11 << "\n";
        } else {
          tdd.verify(device::get<jdx>(temp_res) == device::get<jdx>(test_tup));
        }
      });
    });

    constexpr_for<0, 10>([&](const auto _d) {
      constexpr auto idx = decltype(_d)::value;
      // sayShort << "idx = " << idx << "\n";
      const auto temp_res = tuple_add_to_nth<idx, 11>(test_tup);
      constexpr_for<0, 10>([&](const auto _e) {
        constexpr auto jdx = decltype(_e)::value;
        // sayShort << "jdx = " << jdx << "\n";
        if constexpr (jdx == idx) {
          tdd.verify(device::get<jdx>(temp_res) == device::get<jdx>(test_tup) + 11);
          if (!(device::get<jdx>(temp_res) == device::get<jdx>(test_tup) + 11))
            say << "Value evaluated to " << device::get<jdx>(temp_res) << ", should be "
                << device::get<jdx>(test_tup) + 11 << "\n";
        } else {
          tdd.verify(device::get<jdx>(temp_res) == device::get<jdx>(test_tup));
        }
      });
    });
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::TupleToolsTester> test;
}
