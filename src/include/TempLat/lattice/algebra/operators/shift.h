
#ifndef TEMPLAT_LATTICE_ALGEBRA_SHIFT_H
#define TEMPLAT_LATTICE_ALGEBRA_SHIFT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tuple_size.h"
#include "shiftedcoordinatesmanager.h"

namespace TempLat
{
  /** \brief A class which implements shifts in coordinates.
   *
   * Unit test: make test-shift
   **/
  template <typename R, int... SHIFTS> class ExpressionShifter : public UnaryOperator<R>
  {
  public:
    /* Put public methods here. These should change very little over time. */
    using UnaryOperator<R>::mR;

    KOKKOS_FUNCTION
    ExpressionShifter(const R &pR) : UnaryOperator<R>(pR) { shift = computeShifts({SHIFTS...}); }

    template <typename... IDX> KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
    {
      static_assert(sizeof...(IDX) == 1, "Shift must be deprecated...");
      return GetValue::get(mR, (idx + shift)...);
    }

    void eval(ptrdiff_t i) { DoEval::eval(mR, i + shift); }

    void doWeNeedGhosts() { mR.confirmGhostsUpToDate(); }

    static std::string operatorString() { return getString({SHIFTS...}); }

    KOKKOS_FORCEINLINE_FUNCTION
    ptrdiff_t getShift() { return shift; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    KOKKOS_FORCEINLINE_FUNCTION
    ptrdiff_t computeShifts(const std::vector<ptrdiff_t> &shifts)
    {
      ShiftedCoordinatesManager mShifts(shifts);

      mShifts.setJumps(GetJumps::apply(mR));
      return mShifts.memoryJump();
    }

    static std::string getString(const std::vector<ptrdiff_t> &shifts)
    {
      std::string res = "_(";
      for (auto x : shifts)
        res += std::to_string(x) + ",";
      res.pop_back();
      return res + ")";
    }

    ptrdiff_t shift;
  };

  template <typename R, int N> class ExpressionShifterByOne : public UnaryOperator<R>
  {
    // TODO: override confirmspace to force configuration space.
  public:
    /* Put public methods here. These should change very little over time. */
    using UnaryOperator<R>::mR;

    KOKKOS_FUNCTION
    ExpressionShifterByOne(const R &pR) : UnaryOperator<R>(pR), shift(mR.getToolBox()->getCoordinatesShiftByOne(N)) {}

    KOKKOS_FORCEINLINE_FUNCTION
    auto get(ptrdiff_t i) const { return mR.get(i + shift); }

    void doWeNeedGhosts() { mR.confirmGhostsUpToDate(); }

    std::string toString() const { return GetString::get(mR) + "_(->" + std::to_string(N) + ")"; }

    KOKKOS_FORCEINLINE_FUNCTION
    ptrdiff_t getShift() const { return shift; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    ptrdiff_t shift;
  };

  template <int... shifts, class R>
  KOKKOS_FORCEINLINE_FUNCTION typename std::enable_if<(sizeof...(shifts) > 1) && tuple_size<R>::value == 1,
                                                      ExpressionShifter<R, shifts...>>::type
  shift(const R &pR)
  {
    return ExpressionShifter<R, shifts...>(pR);
  }

  template <int N, class R>
  KOKKOS_FORCEINLINE_FUNCTION typename std::enable_if<tuple_size<R>::value == 1, ExpressionShifterByOne<R, N>>::type
  shift(const R &pR)
  {
    return ExpressionShifterByOne<R, N>(pR);
  }

  template <class R, int N>
  KOKKOS_FORCEINLINE_FUNCTION typename std::enable_if<tuple_size<R>::value == 1, ExpressionShifterByOne<R, N>>::type
  shift(const R &pR, Tag<N> t)
  {
    return ExpressionShifterByOne<R, N>(pR);
  }

  template <int N> KOKKOS_FORCEINLINE_FUNCTION OneType shift(OneType) { return OneType(); }

  template <int N> KOKKOS_FORCEINLINE_FUNCTION OneType shift(OneType, Tag<N>) { return OneType(); }

  struct ExpressionShifterTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  //
  //    template <typename R>
  //    class ExpressionShifter<0,R> : public UnaryOperator<R> {
  //    public:
  //        /* Put public methods here. These should change very little over time. */
  //        using UnaryOperator<R>::mR;
  //
  //        ExpressionShifter(const R& pR, const std::vector<ptrdiff_t>& shifts) :
  //        UnaryOperator<R>(pR)//,
  //        //shift(computeShifts(shifts)),
  //        //shiftString(getString(shifts))
  //        {
  //
  //            shift = computeShifts(shifts);
  //            shiftString = getString(shifts) ;
  //        }
  //
  //        inline
  //        auto get(ptrdiff_t i)
  //        {
  //            return GetValue::get(mR,i + shift);
  //        }
  //
  //        void doWeNeedGhosts() override
  //        {
  //            mR.confirmGhostsUpToDate();
  //        }
  //        std::string operatorString() const override{
  //            return shiftString;
  //        }
  //
  //
  //    private:
  //        /* Put all member variables and private methods here. These may change arbitrarily. */
  //        ptrdiff_t computeShifts(const std::vector<ptrdiff_t>& shifts)
  //        {
  //            ShiftedCoordinatesManager mShifts(shifts);
  //
  //            mShifts.setJumps(GetJumps::apply(mR));
  //            return mShifts.memoryJump();
  //
  //        }
  //        std::string getString(const std::vector<ptrdiff_t>& shifts) const
  //        {
  //            std::string res = "_(";
  //            for(auto x : shifts) res+=std::to_string(x)+",";
  //            res.pop_back();
  //            return res+")";
  //        }
  //        ptrdiff_t shift;
  //        std::string shiftString;
  //
  //
  //
  //    public:
#ifdef TEMPLATTEST
  //        static inline void Test(TDDAssertion& tdd);
#endif
  //    };
  //
  //    template <typename R>
  //    auto shift(const R& pR, const std::vector<ptrdiff_t>& shifts)
  //    {
  //        return ExpressionShifter<0,R>(pR,shifts);
  //    }
  //
  //    template <int N, class R>
  //    auto shift(const R& pR)
  //    {
  //        return ExpressionShifter<N,R>(pR);
  //    }
} // namespace TempLat

#ifdef TEMPLATTEST
#include "shift_test.h"
#endif

#endif
