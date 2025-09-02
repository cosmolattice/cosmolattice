#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COSINE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COSINE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/operators/sine.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

#include "TempLat/lattice/algebra/helpers/getderiv.h"

namespace TempLat
{
#ifndef NOKOKKOS
  using Kokkos::cos;
#else
  using std::cos;
#endif

  /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** \brief A class which applies cosine.
     *
     * Unit test: make test-multiply
     **/
    template <typename T> class Cosine : public UnaryOperator<T>
    {
    public:
      /* Put public methods here. These should change very little over time. */
      using UnaryOperator<T>::mR;

      KOKKOS_FUNCTION
      Cosine(const T &a) : UnaryOperator<T>(a) {}

      /** \brief Getter for two instances. */
      template <typename... IDX>
        requires requires(IDX... idx) { GetValue::get(mR, idx...); }
      KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return cos(GetValue::get(mR, idx...));
      }

      /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> KOKKOS_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        return -GetDeriv::get(mR, other) * sin(mR);
      }

      virtual std::string operatorString() const override { return "cos"; }
    };
  } // namespace Operators

  /** \brief A mini struct for instiating the test case. */
  struct CosineTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  /** \brief Exposing our newly define exp operation to the world. */
  template <typename T>
    requires ConditionalUnaryGetter<T>
  KOKKOS_FORCEINLINE_FUNCTION auto cos(T a)
  {
    return Operators::Cosine<T>(a);
  }

  /** \brief Specialize for possible zero input! */
  KOKKOS_FORCEINLINE_FUNCTION
  OneType cos(ZeroType a) { return OneType(); }
} // namespace TempLat

#endif
