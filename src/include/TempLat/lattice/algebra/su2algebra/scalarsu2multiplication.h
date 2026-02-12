#ifndef COSMOINTERFACE_SU2ALGEBRA_COMPLEXFIELDSU2MULTIPLY_H
#define COSMOINTERFACE_SU2ALGEBRA_COMPLEXFIELDSU2MULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldmultiply.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator.h"
#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which multiplies SU2 field by complex fields.
   *
   *
   * Unit test: ctest -R test-complexfieldsu2multiply
   **/
  template <typename R, typename T> class ScalarSU2Multiplication : public SU2BinaryOperator<R, T>
  {
  public:
    // Put public methods here. These should change very little over time.
    using SU2BinaryOperator<R, T>::mR;
    using SU2BinaryOperator<R, T>::mT;

    DEVICE_FUNCTION
    ScalarSU2Multiplication(const R &pR, const T &pT) : SU2BinaryOperator<R, T>(pR, pT) {}

    DEVICE_FUNCTION
    ScalarSU2Multiplication(const ScalarSU2Multiplication &other) : SU2BinaryOperator<R, T>(other.mR, other.mT) {}

    using SV = typename SU2GetGetReturnType<T>::type;

    template <int N>
      requires(N >= 0 && N <= 3)
    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t) const
    {
      return mR * mT.SU2Get(t);
    }

    template <int N, typename... IDX>
      requires requires(R r, T t, IDX... idx) {
        GetValue::get(r, idx...);
        t.SU2Get(Tag<N>(), idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t, const IDX &...idx) const
    {
      return GetValue::get(mR, idx...) * mT.SU2Get(t, idx...);
    }

    template <int N> DEVICE_FORCEINLINE_FUNCTION auto operator()(Tag<N> t) const { return SU2Get(t); }

    virtual std::string operatorString() const override { return "*"; }
  };

#ifdef TEMPLATTEST
  template <size_t NDim, typename T> struct ComplexFieldSU2MultiplyTester {
    static inline void Test(TDDAssertion &tdd);
  };
#endif

  template <typename R, typename T>
    requires(HasGetMethod<R> && HasSU2Get<T>)
  auto operator*(const R &r, const T &t)
  {
    return ScalarSU2Multiplication{r, t};
  }

  template <typename T>
    requires HasSU2Get<T>
  auto operator*(double r, const T &t)
  {
    return ScalarSU2Multiplication{r, t};
  }

  template <typename T>
    requires HasSU2Get<T>
  auto operator*(float r, const T &t)
  {
    return ScalarSU2Multiplication{r, t};
  }
} // namespace TempLat

#endif
