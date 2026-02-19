#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTCOMPLEXCONJUGATE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTCOMPLEXCONJUGATE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/static_max.h"
#include "TempLat/util/tuple_size.h"
#include "listunaryoperator.h"
#include "TempLat/lattice/algebra/operators/complexconjugate.h"
#include "TempLat/lattice/algebra/conditional/conditionallistunarygetter.h"

namespace TempLat
{

  /** @brief A class which implements the complex conjugation for the list algebra.
   *
   *
   * Unit test: ctest -R test-listcomplexconjugate
   **/

  template <typename R> class ListComplexConjugate : public ListUnaryOperator<R>
  {
  public:
    using ListUnaryOperator<R>::mR;
    ListComplexConjugate(const R &pR) : ListUnaryOperator<R>(pR) {}

    template <int N> auto getComp(Tag<N> t) { return conj(GetComponent::get(mR, t)); }

    virtual std::string operatorString() const { return "conj"; }
    template <int N> void doWeNeedGhosts(Tag<N> i) { GhostsHunter::apply(mR, i); }
    static const size_t size = tuple_size<R>::value;
  };

  template <typename R>
    requires ConditionalListUnaryGetter<R>
  auto conj(const R &r)
  {
    return ListComplexConjugate<R>(r);
  }
} // namespace TempLat

#endif
