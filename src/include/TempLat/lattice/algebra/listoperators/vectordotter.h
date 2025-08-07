#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_VECTORDOTTER_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_VECTORDOTTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/util/constexpr_for.h"
#include "TempLat/lattice/algebra/helpers/getvectorvalue.h"

namespace TempLat
{
  /** \brief A class which takes two VectorGetters, and implements a
   *  special get method which returns the dot-product / contraction of
   *  the two vectors.
   *
   * Unit test: make test-vectordotter
   **/
  template <typename R, typename T> class VectorDotter : public BinaryOperator<R, T>
  {
  public:
    using BinaryOperator<R, T>::mR;
    using BinaryOperator<R, T>::mT;
    VectorDotter(R &a, T &b) : BinaryOperator<R, T>(a, b) {}

    static_assert(R::getVectorSize() == T::getVectorSize(), "VectorDotter: R and T must have the same vector size.");

    /** \brief Getter for two instances: return type automatically determined by the type which we get by multiplying
     * one element of T with one element of S. */
    template <std::integral... IDX>
      requires requires(R mR, T mT, IDX... idx) { mR.vectorGet(idx..., 0) * mT.vectorGet(idx..., 0); }
    KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
    {
      decltype(GetVectorValue::vectorGet(mR, idx..., 0) * GetVectorValue::vectorGet(mT, idx..., 0)) result = 0;

      /* sorry, an if-statement inside a getter function: if T and S are the same thing, let's not call its getter twice
       * (it might be an expensive algebraic operation. */
      if ((void *)&mR == (void *)&mT) {
        constexpr_for<0, mVectorSize, 1>([&](auto _j) {
          constexpr size_t j = decltype(_j)::value;
          result += pow<2>(GetVectorValue::vectorGet(mR, idx..., j));
        });
      } else {
        constexpr_for<0, mVectorSize, 1>([&](auto _j) {
          constexpr size_t j = decltype(_j)::value;
          result += GetVectorValue::vectorGet(mR, idx..., j) * GetVectorValue::vectorGet(mT, idx..., j);
        });
      }
      return result;
    }

    template <size_t NDim> void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
    {
      BinaryOperator<R, T>::confirmSpace(newLayout, spaceType);
    }
    virtual std::string operatorString() const { return "."; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    static constexpr size_t mVectorSize = R::getVectorSize();
  };

  template <typename R, typename T> VectorDotter<R, T> dot(R &r, T &t) { return VectorDotter<R, T>(r, t); }

  template <typename R>
    requires HasVectorGetMethod<R>
  auto norm2(R r)
  {
    return VectorDotter<R, R>(r, r);
  }

  template <typename R> auto norm(R r) { return sqrt(norm2(r)); }

  class VectorDotterTester
  {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
