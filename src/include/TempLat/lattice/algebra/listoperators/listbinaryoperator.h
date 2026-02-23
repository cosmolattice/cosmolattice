#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_BINARYOPERATORVECTOR_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_BINARYOPERATORVECTOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/containsspace.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"
#include "TempLat/lattice/algebra/helpers/getvectorsize.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/confirmghosts.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"

#include "TempLat/lattice/algebra/helpers/preget.h"
#include "TempLat/lattice/algebra/helpers/postget.h"

namespace TempLat
{

  /** @brief Wrapper for algebra with FieldCollection. Here we define the base methods for checking and everything.
   * Can use this algebra to implement matrix and vector algebra! IDEA: statically type MATRIXTYPE type and template
   * methods of the type AddMatrixTypes<R,T>::type.
   *
   * Unit test: ctest -R test-binaryoperator
   **/
  template <typename R, typename T> class ListBinaryOperator
  {
  public:
    ListBinaryOperator(const R &pR, const T &pT) : mR(pR), mT(pT) {}

    DEVICE_FUNCTION
    ListBinaryOperator(const ListBinaryOperator &) = default;

    static consteval size_t getNDim() { return std::max(GetNDim::get<R>(), GetNDim::get<T>()); }

    template <int N, size_t NDim>
    void confirmSpace(Tag<N> i, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
    {
      ConfirmSpace::apply(mR, i, newLayout, spaceType);
      ConfirmSpace::apply(mT, i, newLayout, spaceType);
    }

    template <int N> ptrdiff_t confirmGhostsUpToDate(Tag<N> i) const
    {
      return ConfirmGhosts::apply(mR, i) + ConfirmGhosts::apply(mT, i);
    }

    /** @brief Override this method in your derived class, to have an easy implementation of your toString method. */
    virtual std::string operatorString() const = 0;

    /** @brief If your descending class implements `operatorString()` and your operator is of the type "a OP b" (where
     * OP is * or whatever), this toString method does all the work for you. */
    template <int N> std::string toString(Tag<N> i) const
    {
      std::string tt = GetString::get(mR, i);
      if (ContainsSpace::test(tt)) tt = "(" + tt + ")";

      std::string ss = GetString::get(mT, i);
      if (ContainsSpace::test(ss)) ss = "(" + ss + ")";

      std::string result = tt + " " + operatorString() + " " + ss;
      return result;
    }

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

    void preGet()
    {
      PreGet::apply(mR);
      PreGet::apply(mT);
    }

    void postGet()
    {
      PostGet::apply(mR);
      PostGet::apply(mT);
    }

    using Getter = GetComponent;

  protected:
    R mR;
    T mT;
  };
} // namespace TempLat

#endif
