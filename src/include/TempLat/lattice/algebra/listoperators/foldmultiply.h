#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_FOLDMULTIPLY_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_FOLDMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/util/tuple_size.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"

namespace TempLat
{
  template <class Tuple, typename Function, typename Unit, int... INT>
  constexpr auto fold_multiply_impl(Tuple &&tup, Function &&func, Unit &&unit, std::integer_sequence<int, INT...> iseq)
  {
    return (unit * ... * func(GetComponent::get(tup, Tag<INT>())));
  }

  /** @brief Multiplies all the components of a tuple after applying func to them.
   *
   * Unit test: ctest -R test-foldmultiply
   *
   *  @param tup the tuple to multiply over
   *  @param func the function to apply to each component before multiplying
   **/
  template <class Tuple, typename Function> constexpr auto fold_multiply(Tuple &&tup, Function &&func)
  {
    return fold_multiply_impl(std::forward<Tuple>(tup), std::forward<Function>(func), OneType(),
                              std::make_integer_sequence<int, tuple_size<Tuple>::value>());
  }

  /** @brief Multiplies all the components of a tuple.
   *
   * Unit test: ctest -R test-foldmultiply
   *
   *  @param tup the tuple to multiply over
   **/
  template <class Tuple> constexpr auto fold_multiply(Tuple &&tup)
  {
    if constexpr (tuple_size<Tuple>::value > 0)
      return fold_multiply_impl(
          std::forward<Tuple>(tup), [](auto x) { return x; }, OneType(),
          std::make_integer_sequence<int, tuple_size<Tuple>::value>());
    else
      return OneType();
  }

  /** @brief Multiplies all the components of a tuple after applying func to them, starting from unit.
   *
   * Unit test: ctest -R test-foldmultiply
   *
   *  @param tup the tuple to multiply over
   *  @param func the function to apply to each component before multiplying
   *  @param unit the unit to start multiplying from
   **/
  template <class Tuple, class Unit> constexpr auto fold_multiply_unit(Tuple &&tup, Unit &&unit)
  {
    if constexpr (tuple_size<Tuple>::value > 0)
      return fold_multiply_impl(
          std::forward<Tuple>(tup), [](auto x) { return x; }, std::forward<Unit>(unit),
          std::make_integer_sequence<int, tuple_size<Tuple>::value>());
    else
      return OneType();
  }
} // namespace TempLat

#endif
