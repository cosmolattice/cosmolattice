#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTSHIFT_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTSHIFT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/static_max.h"
#include "TempLat/util/tuple_size.h"
#include "listunaryoperator.h"
#include "TempLat/lattice/algebra/conditional/conditionallistunarygetter.h"
#include "TempLat/lattice/algebra/spatialderivatives/normgradientsquare.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"

namespace TempLat
{

  /** @brief A class which implements shifts for the list algebra.
   *
   *
   * Unit test: ctest -R test-listshift
   **/

  template <class R, int... N> class ListShifter : public ListUnaryOperator<R>
  {
  public:
    using ListUnaryOperator<R>::mR;
    ListShifter(const R &pR) : ListUnaryOperator<R>(pR) {}

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto getComp(Tag<M> t) const
    {
      return shift<N...>(GetComponent::get(mR, t));
    }

    virtual std::string operatorString() const override { return ""; }
    template <int M> void doWeNeedGhosts(Tag<M> i) { GetComponent::get(mR, i).confirmGhostsUpToDate(); }

    static const size_t size = tuple_size<R>::value;
  };

  template <typename R, int N> class ListShifterByOne : public ListUnaryOperator<R>
  {
  public:
    // Put public methods here. These should change very little over time.
    ListShifterByOne(const R &pR) : ListUnaryOperator<R>(pR), mR(pR) {}

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto getComp(Tag<M> t) const
    {
      return shift<N>(GetComponent::get(mR, t));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    R mR;
  };

  template <typename R, int... N>
    requires(IsSTDGettable<0, R> || IsTempLatGettable<0, R>)
  DEVICE_FORCEINLINE_FUNCTION auto shift(const R &r)
  {
    return ListShifter<R, N...>(r);
  }

  template <int N, class R>
    requires(IsSTDGettable<0, R> || IsTempLatGettable<0, R>)
  DEVICE_FORCEINLINE_FUNCTION auto shift(const R &pR)
  {
    return ListShifterByOne<R, N>(pR);
  }

  template <class R, int N>
    requires(IsSTDGettable<0, R> || IsTempLatGettable<0, R>)
  DEVICE_FORCEINLINE_FUNCTION auto shift(const R &pR, const Tag<N> &t)
  {
    return ListShifterByOne<R, N>(pR);
  }

} // namespace TempLat

#endif
