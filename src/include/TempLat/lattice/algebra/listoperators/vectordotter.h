#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_VECTORDOTTER_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_VECTORDOTTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/operators/squareroot.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/util/constexpr_for.h"
#include "TempLat/util/tuple_tools.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which takes two VectorGetters, and implements a
   *  special get method which returns the dot-product / contraction of
   *  the two vectors.
   *
   * Unit test: ctest -R test-vectordotter
   **/
  template <typename R, typename T> class VectorDotter : public BinaryOperator<R, T>
  {
  public:
    using BinaryOperator<R, T>::mR;
    using BinaryOperator<R, T>::mT;

    DEVICE_FUNCTION
    VectorDotter(R &a, T &b) : BinaryOperator<R, T>(a, b) {}

    DEVICE_FUNCTION
    VectorDotter(const VectorDotter &) = default;

    static_assert(std::decay_t<R>::getVectorSize() == std::decay_t<T>::getVectorSize(),
                  "VectorDotter: R and T must have the same vector size.");

    template <typename... IDX>
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      /* sorry, an if-statement inside a getter function: if T and S are the same thing, let's not call its getter twice
       * (it might be an expensive algebraic operation. */
      if ((void *)&mR == (void *)&mT) {
        auto cache = DoEval::eval(mR, idx...);
        return device::apply([&](const auto &...args) { return (pow<2>(args) + ...); }, cache);
      } else {
        auto cache1 = DoEval::eval(mR, idx...);
        auto cache2 = DoEval::eval(mT, idx...);
        return device::apply([&](const auto &...args) { return ((cache1[args] * cache2[args]) + ...); },
                             make_tuple_sequence<mVectorSize>());
      }
    }

    template <size_t NDim> void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
    {
      BinaryOperator<R, T>::confirmSpace(newLayout, spaceType);
    }

    virtual std::string operatorString() const override { return "."; }

    std::string toString() const
    {
      std::string tt = GetString::get(mR);
      if (ContainsSpace::test(tt)) tt = "(" + tt + ")";

      std::string ss = GetString::get(mT);
      if (ContainsSpace::test(ss)) ss = "(" + ss + ")";

      std::string result = "<" + tt + "," + ss + ">";
      return result;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    static constexpr size_t mVectorSize = std::decay_t<R>::getVectorSize();
  };

  template <typename R, typename T> VectorDotter<R, T> dot(R r, T t) { return VectorDotter<R, T>(r, t); }

  template <typename R>
    requires HasVectorGetMethod<R>
  auto norm2(R r)
  {
    return VectorDotter<R, R>(r, r);
  }

  template <typename R> auto norm(R r) { return sqrt(norm2(r)); }
} // namespace TempLat

#endif
