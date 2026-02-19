#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTLOG_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTLOG_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/static_max.h"
#include "TempLat/util/tuple_size.h"
#include "listunaryoperator.h"
#include "TempLat/lattice/algebra/conditional/conditionallistunarygetter.h"
#include "TempLat/lattice/algebra/operators/log.h"

namespace TempLat
{
  /** @brief A class which implements the logarithm for the list algebra.
   *
   *
   * Unit test: ctest -R test-listlog
   **/
  template <typename R> class ListLog : public ListUnaryOperator<R>
  {
  public:
    using ListUnaryOperator<R>::mR;
    ListLog(const R &pR) : ListUnaryOperator<R>(pR) {}

    template <int N> auto getComp(Tag<N> t) { return log(GetComponent::get(mR, t)); }

    virtual std::string operatorString() const { return "log"; }
    template <int N> void doWeNeedGhosts(Tag<N> i) { GhostsHunter::apply(mR, i); }
    static const size_t size = tuple_size<R>::value;
  };

  template <typename R>
    requires ConditionalListUnaryGetter<R>
  auto log(const R &r)
  {
    return ListLog<R>(r);
  }
} // namespace TempLat

#endif
