#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LOG_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LOG_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cmath>

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/divide.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /** \brief Enable use of this operator without prefixing std:: or TempLat::. The compiler can distinguish between
   * them. */
#ifndef NOKOKKOS
  using Kokkos::log;
#else
  using std::log;
#endif

  /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** \brief A class which applies a minus sign. Holds the expression, only evaluates for a single element when you
     *call Multiply::get(pIterCoords).
     *
     * Unit test: make test-multiply
     **/
    template <typename T> class Log : public UnaryOperator<T>
    {
    public:
      /* Put public methods here. These should change very little over time. */
      using UnaryOperator<T>::mR;

      KOKKOS_FUNCTION
      Log(const T &a) : UnaryOperator<T>(a) {}

      /** \brief Getter for two instances. */
      template <typename... IDX>
        requires requires(IDX... idx) { GetValue::get(mR, idx...); }
      KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return log(GetValue::get(mR, idx...));
      }

      /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> KOKKOS_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        /* not using pow for 1/mInstanceT because pow imports us, log.h */
        return GetDeriv::get(mR, other) / mR;
      }

      static std::string operatorString() { return "log"; }
    };
  } // namespace Operators

  /** \brief A mini struct for instiating the test case. */
  struct LogTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  /** \brief Exposing our newly define log operation to the world. */
  template <typename T>
    requires ConditionalUnaryGetter<T>
  KOKKOS_FORCEINLINE_FUNCTION auto log(T a)
  {
    return Operators::Log<T>(a);
  }

  /** \brief Specialize for possible zero output! */
  KOKKOS_FORCEINLINE_FUNCTION
  ZeroType log(OneType a) { return ZeroType(); }
} // namespace TempLat

#endif
