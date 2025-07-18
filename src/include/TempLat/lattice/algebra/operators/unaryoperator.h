#ifndef TEMPLAT_LATTICE_ALGEBRA_UNARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_UNARYOPERATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/util/tdd/tdd.h"
// #include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/confirmghosts.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getjumps.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"

// #include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"

namespace TempLat
{
  /** \brief A parent class which implements the common methods that all unary operators (-, sqrt ) share.
   *  Inherits from SpaceStateInterface in order to make the confirm(Config/Fourier)Space methods
   *  accessible from any type of template instance.
   *
   * Unit test: make test-unaryoperator
   **/
  template <typename R> class UnaryOperator
  {
  public:
    KOKKOS_FUNCTION
    UnaryOperator(const R &pR) : mR(pR) {}

    void doWeNeedGhosts() { GhostsHunter::apply(mR); }

    ptrdiff_t confirmGhostsUpToDate() { return ConfirmGhosts::apply(mR); }

    template <size_t NDim>
    void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateInterface<NDim>::SpaceType &spaceType)
    {
      ConfirmSpace::apply(mR, newLayout, spaceType);
    }

    KOKKOS_FORCEINLINE_FUNCTION
    void eval(ptrdiff_t i) { DoEval::eval(mR, i); }

    template <size_t NDim> JumpsHolder<NDim> getJumps() { return GetJumps::apply<NDim>(mR); }

    /** For measurement objects: need the toolbox for easiest access to loopers and whatever else. */
    auto getToolBox() { return GetToolBox::get(mR); }

    /** \brief Override this method in your derived class, to have an easy implementation of your toString method. */
    static std::string operatorString() { return " "; }

    KOKKOS_FORCEINLINE_FUNCTION
    auto getDx() const { return GetDx::getDx(mR); }

    KOKKOS_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(mR); }

    /** \brief If your descending class implements `operatorString()` and your operator is of the type "OP b" (where OP
     * is * or whatever), this toString method does all the work for you, only adding parentheses if b contains spaces.
     */
    std::string toString() const
    {
      std::string result = GetString::get(mR);
      if (ContainsSpace::test(result)) result = "(" + result + ")";

      return operatorString() + result;
    }

  protected:
    R mR;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
