#ifndef TEMPLAT_LATTICE_ALGEBRA_BINARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_BINARYOPERATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/confirmghosts.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getjumps.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/parallel/kokkos/kokkos.h"

#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"

namespace TempLat
{
  /** \brief A parent class which implements the common methods that all binary operators (*, +, -,... ) share.
   *  Inherits from SpaceStateInterface in order to make the confirm(Config/Fourier)Space methods
   *  accessible from any type of template instance.
   *
   * Unit test: make test-binaryoperator
   **/
  template <typename R, typename T> class BinaryOperator
  {
  public:
    KOKKOS_FUNCTION
    BinaryOperator(const R &pR, const T &pT) : mR(pR), mT(pT) {}

    KOKKOS_FORCEINLINE_FUNCTION
    void doWeNeedGhosts()
    {
      GhostsHunter::apply(mR);
      GhostsHunter::apply(mT);
    }

    template <size_t NDim>
    KOKKOS_FORCEINLINE_FUNCTION void confirmSpace(const LayoutStruct<NDim> &newLayout,
                                                  const SpaceStateInterface<NDim>::SpaceType &spaceType)
    {
      ConfirmSpace::apply(mR, newLayout, spaceType);
      ConfirmSpace::apply(mT, newLayout, spaceType);
    }

    KOKKOS_FORCEINLINE_FUNCTION
    ptrdiff_t confirmGhostsUpToDate() { return ConfirmGhosts::apply(mR) + ConfirmGhosts::apply(mT); }

    template <size_t NDim> KOKKOS_FORCEINLINE_FUNCTION JumpsHolder<NDim> getJumps() const
    {
      auto a = GetJumps::apply<NDim>(mR);
      auto b = GetJumps::apply<NDim>(mT);
      if (a != b && !(a.isEmpty() || b.isEmpty()))
        throw DifferentJumpsHolderException("Two different memory layouts in binary operator" /*, toString()*/);
      return a.isEmpty() ? b : a;
    }

    KOKKOS_FORCEINLINE_FUNCTION
    void eval(ptrdiff_t i)
    {
      DoEval::eval(mR, i);
      DoEval::eval(mT, i);
    }

    KOKKOS_FORCEINLINE_FUNCTION
    auto getDx() const { return HasDx<R>::value ? GetDx::getDx(mR) : (HasDx<T>::value ? GetDx::getDx(mT) : 1); }

    KOKKOS_FORCEINLINE_FUNCTION
    auto getKIR() const { return HasKIR<R>::value ? GetKIR::getKIR(mR) : (HasKIR<T>::value ? GetKIR::getKIR(mT) : 1); }

    /** \brief Override this method in your derived class, to have an easy implementation of your toString method. */
    static std::string operatorString() { return " "; }

    /** \brief If your descending class implements `operatorString()` and your operator is of the type "a OP b" (where
     * OP is * or whatever), this toString method does all the work for you. */
    std::string toString() const
    {
      std::string tt = GetString::get(mR);

      if (ContainsSpace::test(tt)) tt = "(" + tt + ")";

      std::string ss = GetString::get(mT);

      if (ContainsSpace::test(ss)) ss = "(" + ss + ")";

      std::string result = tt + " " + operatorString() + " " + ss;

      return result;
    }

    /** For measurement objects: need the toolbox for easiest access to loopers and whatever else. */
    inline auto getToolBox()
    {
      using AT = decltype(GetToolBox::get(mR));
      using BT = decltype(GetToolBox::get(mT));
      if constexpr (!std::is_same_v<AT, std::nullptr_t>)
        return GetToolBox::get(mR);
      else if constexpr (!std::is_same_v<BT, std::nullptr_t>)
        return GetToolBox::get(mT);
      else
        return nullptr;
    }

  protected:
    R mR;
    T mT;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/binaryoperator_test.h"
#endif

#endif
