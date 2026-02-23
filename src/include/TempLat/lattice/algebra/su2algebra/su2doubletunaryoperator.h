#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETUNARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETUNARYOPERATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/containsspace.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletget.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

#include "TempLat/lattice/algebra/helpers/preget.h"
#include "TempLat/lattice/algebra/helpers/postget.h"

namespace TempLat
{
  /** @brief A class which groups common features to su2doublets unary operators.
   *
   * Unit test: ctest -R test-su2doubletunaryoperator
   **/
  template <typename R> class SU2DoubletUnaryOperator
  {
  public:
    // Put public methods here. These should change very little over time.
    SU2DoubletUnaryOperator(const R &pR) : mR(pR) {}

    DEVICE_FUNCTION
    SU2DoubletUnaryOperator(const SU2DoubletUnaryOperator &) = default;

    static consteval size_t getNDim() { return GetNDim::get<R>(); }

    /** @brief Override this method in your derived class, to have an easy implementation of your toString method. */
    virtual std::string operatorString() const { return " "; }

    void preGet() { PreGet::apply(mR); }

    void postGet() { PostGet::apply(mR); }

    /** @brief If your descending class implements `operatorString()` and your operator is of the type "OP b" (where OP
     * is * or whatever), this toString method does all the work for you, only adding parentheses if b contains spaces.
     */
    virtual std::string toString() const
    {
      std::string result = GetString::get(mR);

      if (ContainsSpace::test(result)) result = "(" + result + ")";

      return operatorString() + result;
    }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return GetDx::getDx(mR); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(mR); }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      return DoEval::eval(mR, idx...);
    }

    static constexpr size_t size = 4;
    using Getter = SU2DoubletGetter;

  protected:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    R mR;
  };

} // namespace TempLat

#endif
