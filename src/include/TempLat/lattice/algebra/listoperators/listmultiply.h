#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTMULTIPLY_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/static_max.h"
#include "TempLat/util/tuple_size.h"
#include "listbinaryoperator.h"
#include "TempLat/lattice/algebra/conditional/conditionallistbinarygetter.h"
#include "TempLat/lattice/algebra/operators/multiply.h"

namespace TempLat
{

  template <typename R, typename T> class ListMultiplication : public ListBinaryOperator<R, T>
  {
  public:
    using ListBinaryOperator<R, T>::mR;
    using ListBinaryOperator<R, T>::mT;

    ListMultiplication(const R &pR, const T &pT) : ListBinaryOperator<R, T>(pR, pT) {}

    template <int N> auto getComp(Tag<N> t) { return GetComponent::get(mR, t) * GetComponent::get(mT, t); }

    virtual std::string operatorString() const { return "*"; }
    template <int N> void doWeNeedGhosts(Tag<N> i)
    {
      GhostsHunter::apply(mR, i);
      GhostsHunter::apply(mT, i);
    }
    static const size_t size = static_max<tuple_size<R>::value, tuple_size<T>::value>::value;
  };

  template <typename R, typename T>
    requires ConditionalListBinaryGetter<R, T>
  auto operator*(const R &r, const T &t)
  {
    return ListMultiplication<R, T>(r, t);
  }
} // namespace TempLat

#endif
