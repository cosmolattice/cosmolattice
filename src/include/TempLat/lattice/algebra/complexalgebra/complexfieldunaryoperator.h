#ifndef TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_COMPLEXFIELDUNARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_COMPLEXFIELDUNARYOPERATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

#include "TempLat/lattice/algebra/helpers/preget.h"
#include "TempLat/lattice/algebra/helpers/postget.h"

namespace TempLat
{
  /** @brief A class which groups common features of unary complex field operators.
   *
   *
   * Unit test: ctest -R test-complexfieldunaryoperator
   **/
  template <typename R> class ComplexFieldUnaryOperator
  {
  public:
    // Put public methods here. These should change very little over time.
    DEVICE_FUNCTION
    ComplexFieldUnaryOperator(const R &pR) : mR(pR) {}

    DEVICE_FUNCTION
    ComplexFieldUnaryOperator(const ComplexFieldUnaryOperator &) = default;

    static consteval size_t getNDim() { return GetNDim::get<R>(); }

    /** @brief Override this method in your derived class, to have an easy implementation of your toString method. */
    virtual std::string operatorString() const { return " "; }

    /** @brief If your descending class implements `operatorString()` and your operator is of the type "OP b" (where OP
     * is * or whatever), this toString method does all the work for you, only adding parentheses if b contains spaces.
     */
    std::string toString() const
    {
      std::string result = GetString::get(mR);

      if (ContainsSpace::test(result)) result = "(" + result + ")";

      return operatorString() + result;
    }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return GetDx::getDx(mR); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(mR); }

    void preGet() { PreGet::apply(mR); }

    void postGet() { PostGet::apply(mR); }

    static constexpr size_t size = 2;
    using Getter = ComplexFieldGetter;

  protected:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    R mR;
  };

} // namespace TempLat

#endif
