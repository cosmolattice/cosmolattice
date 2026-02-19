#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTABSOLUTEVALUE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTABSOLUTEVALUE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/static_max.h"
#include "TempLat/util/tuple_size.h"
#include "listunaryoperator.h"
#include "TempLat/lattice/algebra/conditional/conditionallistunarygetter.h"
#include "TempLat/lattice/algebra/operators/absolutevalue.h"

namespace TempLat
{
  /** @brief A class which adds two getters. Holds the expression, only evaluates for a single element when you call
   *Multiply::get(pIterCoords).
   *
   * Unit test: ctest -R test-multiply
   **/
  template <typename R> class ListAbsoluteValue : public ListUnaryOperator<R>
  {
  public:
    using ListUnaryOperator<R>::mR;
    ListAbsoluteValue(const R &pR) : ListUnaryOperator<R>(pR) {}

    template <int N> auto getComp(Tag<N> t) { return abs(GetComponent::get(mR, t)); }

    virtual std::string operatorString() const override { return "abs"; }

    template <int N> void doWeNeedGhosts(Tag<N> i) { GhostsHunter::apply(mR, i); }

    static constexpr size_t size = tuple_size<R>::value;
  };

  template <typename R>
    requires ConditionalListUnaryGetter<R>
  auto abs(const R &r)
  {
    return ListAbsoluteValue<R>(r);
  }
} // namespace TempLat

#endif
