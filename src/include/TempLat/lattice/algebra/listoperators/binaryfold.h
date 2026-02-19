#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_BINARYFOLD_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_BINARYFOLD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/util/tuple_size.h"

// NOTE: unfolding of binary operators is part of c++17, could change that in future version.

namespace TempLat
{

  template <class BinaryOp, class Tuple, typename Function, typename Unity>
  constexpr auto binary_fold_impl(BinaryOp &&op, Tuple &&tup, Function &&func, Unity &&unit, Tag<-1> tg)
  {
    return unit;
  }

  template <class BinaryOp, class Tuple, typename Function, typename Unity, int Index>
  constexpr auto binary_fold_impl(BinaryOp &&op, Tuple &&tup, Function &&func, Unity &&unit, Tag<Index> tg)
  {
    return op(func(GetComponent::get(tup, tg)),
              binary_fold_impl(std::forward<BinaryOp>(op), std::forward<Tuple>(tup), std::forward<Function>(func),
                               std::forward<Unity>(unit), Tag<Index - 1>()));
  }
  template <class BinaryOp, class Tuple, typename Function, typename Unity>
  constexpr auto binary_fold(BinaryOp &&op, Tuple &&tup, Function &&func, Unity &&unit)
  {
    constexpr int tupSize = tuple_size<std::decay_t<Tuple>>::value;
    if constexpr (tupSize > 0)
      return binary_fold_impl(std::forward<BinaryOp>(op), std::forward<Tuple>(tup), std::forward<Function>(func),
                              std::forward<Unity>(unit), Tag<tupSize - 1>());
    else
      return unit;
  }

} // namespace TempLat

#endif
