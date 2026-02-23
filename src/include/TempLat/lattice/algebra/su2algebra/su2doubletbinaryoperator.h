#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETBINARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETBINARYOPERATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletget.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2get.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"

namespace TempLat
{
  /** @brief A class which implements common features to SU2Doublet binary operators.
   *
   * Unit test: ctest -R test-su2doubletbinaryoperator
   **/
  template <typename R, typename T> class SU2DoubletBinaryOperator
  {
  public:
    // Put public methods here. These should change very little over time.
    SU2DoubletBinaryOperator(const R &pR, const T &pT) : mR(pR), mT(pT) {}

    DEVICE_FUNCTION
    SU2DoubletBinaryOperator(const SU2DoubletBinaryOperator &) = default;

    static constexpr size_t size = 4;
    using Getter = SU2DoubletGetter;

    static consteval size_t getNDim() { return std::max(GetNDim::get<R>(), GetNDim::get<T>()); }

    /** @brief Override this method in your derived class, to have an easy implementation of your toString method. */
    virtual std::string operatorString() const { return " "; }

    /** @brief If your descending class implements `operatorString()` and your operator is of the type "a OP b" (where
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

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return GetDx::getDx(mR); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(mR); }

    inline auto getToolBox() const
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
    /* Put all member variables and private methods here. These may change arbitrarily. */

    R mR;
    T mT;
  };

} // namespace TempLat

#endif
