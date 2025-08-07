
#ifndef TEMPLAT_LATTICE_ALGEBRA_SHIFT_H
#define TEMPLAT_LATTICE_ALGEBRA_SHIFT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tuple_size.h"

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

    static constexpr size_t dim = sizeof...(SHIFTS);
    static constexpr auto shifts = std::make_tuple(SHIFTS...);

    KOKKOS_FUNCTION
    ExpressionShifter(const R &pR) : UnaryOperator<R>(pR) {}

    template <std::integral... IDX> KOKKOS_FORCEINLINE_FUNCTION auto get(IDX... idx) const
    {
      constexpr_for<0, dim, 1>([&](const auto _d) {
        constexpr size_t d = decltype(_d)::value;
        tuple_add_to_nth<d>(std::tie(idx...), std::get<d>(shifts));
      });
      return GetValue::get(mR, idx...);
    }

    template <std::integral... IDX> KOKKOS_FORCEINLINE_FUNCTION void eval(IDX... idx) const
    {
      constexpr_for<0, dim, 1>([&](const auto _d) {
        constexpr size_t d = decltype(_d)::value;
        tuple_add_to_nth<d>(std::tie(idx...), std::get<d>(shifts));
      });
      return DoEval::eval(mR, idx...);
    }

    void doWeNeedGhosts() { mR.confirmGhostsUpToDate(); }

    static std::string operatorString()
    {
      std::string res = "_(";
      for (auto x : std::tie(SHIFTS...))
        res += std::to_string(x) + ",";
      res.pop_back();
      return res + ")";
    }
  };

  template <typename R, int N> class ExpressionShifterByOne : public UnaryOperator<R>
  {
    // TODO: override confirmspace to force configuration space.
  public:
    /* Put public methods here. These should change very little over time. */
    using UnaryOperator<R>::mR;

    KOKKOS_FUNCTION
    ExpressionShifterByOne(const R &pR) : UnaryOperator<R>(pR) {}

    template <std::integral... IDX> KOKKOS_FORCEINLINE_FUNCTION auto get(IDX... idx) const
    {
      tuple_add_to_nth<N>(std::tie(idx...), 1);
      return GetValue::get(mR, idx...);
    }

    template <std::integral... IDX> KOKKOS_FORCEINLINE_FUNCTION void eval(IDX... idx) const
    {
      tuple_add_to_nth<N>(std::tie(idx...), 1);
      return DoEval::eval(mR, idx...);
    }

    void doWeNeedGhosts() { mR.confirmGhostsUpToDate(); }

    std::string toString() const { return GetString::get(mR) + "_(->" + std::to_string(N) + ")"; }
  };

  template <int... shifts, class R>
    requires((sizeof...(shifts) > 1) && tuple_size<R>::value == 1)
  auto shift(const R &pR)
  {
    return ExpressionShifter<R, shifts...>(pR);
  }

  template <int N, class R>
    requires(tuple_size<R>::value == 1)
  auto shift(const R &pR)
  {
    return ExpressionShifterByOne<R, N>(pR);
  }

  template <class R, int N>
    requires(tuple_size<R>::value == 1)
  auto shift(const R &pR, Tag<N> t)
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
  //    template <typename r>
  //    class expressionshifter<0,r> : public unaryoperator<r> {
  //    public:
  //        /* put public methods here. these should change very little over time. */
  //        using unaryoperator<r>::mr;
  //
  //        expressionshifter(const r& pr, const std::vector<ptrdiff_t>& shifts) :
  //        unaryoperator<r>(pr)//,
  //        //shift(computeshifts(shifts)),
  //        //shiftstring(getstring(shifts))
  //        {
  //
  //            shift = computeshifts(shifts);
  //            shiftstring = getstring(shifts) ;
  //        }
  //
  //        inline
  //        auto get(ptrdiff_t i)
  //        {
  //            return getvalue::get(mr,i + shift);
  //        }
  //
  //        void doweneedghosts() override
  //        {
  //            mr.confirmghostsuptodate();
  //        }
  //        std::string operatorstring() const override{
  //            return shiftstring;
  //        }
  //
  //
  //    private:
  //        /* put all member variables and private methods here. these may change arbitrarily. */
  //        ptrdiff_t computeshifts(const std::vector<ptrdiff_t>& shifts)
  //        {
  //            shiftedcoordinatesmanager mshifts(shifts);
  //
  //            mshifts.setjumps(getjumps::apply(mr));
  //            return mshifts.memoryjump();
  //
  //        }
  //        std::string getstring(const std::vector<ptrdiff_t>& shifts) const
  //        {
  //            std::string res = "_(";
  //            for(auto x : shifts) res+=std::to_string(x)+",";
  //            res.pop_back();
  //            return res+")";
  //        }
  //        ptrdiff_t shift;
  //        std::string shiftstring;
  //
  //
  //
  //    public:
#ifdef templattest
  //        static inline void test(tddassertion& tdd);
#endif
  //    };
  //
  //    template <typename r>
  //    auto shift(const r& pr, const std::vector<ptrdiff_t>& shifts)
  //    {
  //        return expressionshifter<0,r>(pr,shifts);
  //    }
  //
  //    template <int n, class r>
  //    auto shift(const r& pr)
  //    {
  //        return expressionshifter<n,r>(pr);
  //    }
} // namespace TempLat

#ifdef templattest
#include "shift_test.h"
#endif

#endif
