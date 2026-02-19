#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTLAPLACIAN_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTLAPLACIAN_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/static_max.h"
#include "TempLat/util/tuple_size.h"
#include "listunaryoperator.h"
#include "TempLat/lattice/algebra/conditional/conditionallistunarygetter.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

namespace TempLat
{

  /** @brief A class which applies a minus sign. Holds the expression, only evaluates for a single element when you call
   *Multiply::get(pIterCoords).
   *
   * Unit test: ctest -R test-multiply
   **/
  template <int NDim, typename R> class ListLaplacian : public ListUnaryOperator<R>
  {
  public:
    using ListUnaryOperator<R>::mR;
    ListLaplacian(const R &pR) : ListUnaryOperator<R>(pR) {}

    template <int N> auto getComp(Tag<N> t) { return LatLapl<NDim>(GetComponent::get(mR, t)); }

    virtual std::string operatorString() const { return "lapl"; }
    template <int N> void doWeNeedGhosts(Tag<N> i) { GetComponent::get(mR, i).confirmGhostsUpToDate(); }

    static const size_t size = tuple_size<R>::value;
  };

  template <class T> using LapList3D = ListLaplacian<3, T>;

  template <int NDim, typename R>
    requires(IsSTDGettable<0, R> || IsTempLatGettable<0, R>)
  auto LatLapl(const R &r)
  {
    return ListLaplacian<NDim, R>(r);
  }
} // namespace TempLat

#endif
