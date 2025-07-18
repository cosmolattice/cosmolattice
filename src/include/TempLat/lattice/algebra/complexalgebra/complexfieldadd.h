#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDADD_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDADD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "real.h"
#include "imag.h"
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator.h"
#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"

namespace TempLat
{

  /** \brief A class which implements complex field addition.
   *
   * Unit test: make test-complexfieldadd
   **/
  template <class R, class T> class ComplexFieldAddition : public ComplexFieldBinaryOperator<R, T>
  {
  public:
    /* Put public methods here. These should change very little over time. */

    using ComplexFieldBinaryOperator<R, T>::mR;
    using ComplexFieldBinaryOperator<R, T>::mT;

    ComplexFieldAddition(const R &pR, const T &pT) : ComplexFieldBinaryOperator<R, T>(pR, pT) {}

    KOKKOS_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t) { return Real(mR) + Real(mT); }
    KOKKOS_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t) { return Imag(mR) + Imag(mT); }

    KOKKOS_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t, ptrdiff_t i) { return mR.ComplexFieldGet(0_c, i) + mT.ComplexFieldGet(0_c, i); }
    KOKKOS_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t, ptrdiff_t i) { return mR.ComplexFieldGet(1_c, i) + mT.ComplexFieldGet(1_c, i); }

    void eval(ptrdiff_t i)
    {
      DoEval::eval(mR, i);
      DoEval::eval(mT, i);
    }

    static std::string operatorString() { return "+"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  struct ComplexFieldAddTester {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && HasComplexFieldGet<T>)
  auto operator+(const R &r, const T &t)
  {
    return ComplexFieldAddition<R, T>{r, t};
  }

  template <typename R, typename T>
    requires(!HasStaticGetter<R> && HasComplexFieldGet<T>)
  auto operator+(const R &r, const T &t)
  {
    return ComplexFieldAddition<ComplexFieldWrapper<R, ZeroType>, T>{Complexify(r, ZeroType()), t};
  }

  template <typename R, typename T>
    requires(!HasStaticGetter<T> && HasComplexFieldGet<R>)
  auto operator+(const R &r, const T &t)
  {
    return ComplexFieldAddition<R, ComplexFieldWrapper<T, ZeroType>>{r, Complexify(t, ZeroType())};
  }

} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/complexfieldadd_test.h"
#endif

#endif
