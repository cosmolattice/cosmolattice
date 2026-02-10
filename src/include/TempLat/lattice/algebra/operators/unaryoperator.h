#ifndef TEMPLAT_LATTICE_ALGEBRA_UNARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_UNARYOPERATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/util/tdd/tdd.h"
// #include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/confirmghosts.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"

#include "TempLat/lattice/algebra/helpers/preget.h"
#include "TempLat/lattice/algebra/helpers/postget.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"

#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getjumps.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"

#include "TempLat/parallel/device.h"

// #include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"

namespace TempLat
{
  /** @brief A parent class which implements the common methods that all unary operators (-, sqrt ) share.
   *
   * Unit test: make test-unaryoperator
   **/
  template <typename R> class UnaryOperator
  {
  public:
    DEVICE_FUNCTION
    UnaryOperator(const R &pR) : mR(pR) {}

    DEVICE_FUNCTION
    UnaryOperator(const UnaryOperator &other) : mR(other.mR) {}

    DEVICE_FUNCTION
    ~UnaryOperator() {}

    static consteval size_t getNDim() { return GetNDim::get<R>(); }

    void doWeNeedGhosts() const { GhostsHunter::apply(mR); }

    void preGet() const { PreGet::apply(mR); }

    void postGet() const { PostGet::apply(mR); }

    ptrdiff_t confirmGhostsUpToDate() const { return ConfirmGhosts::apply(mR); }

    template <size_t NDim> void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
    {
      ConfirmSpace::apply(mR, newLayout, spaceType);
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
    }

    template <size_t NDim> JumpsHolder<NDim> getJumps() const { return GetJumps::apply<NDim>(mR); }

    /** For measurement objects. */
    auto getToolBox() const { return GetToolBox::get(mR); }

    /** @brief Override this method in your derived class, to have an easy implementation of your toString method. */
    virtual std::string operatorString() const { return " "; }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return GetDx::getDx(mR); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(mR); }

    /** @brief If your descending class implements `operatorString()` and your operator is of the type "OP b" (where OP
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
