#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2LIEALGEBRAFIELD_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2LIEALGEBRAFIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2021

#include "TempLat/lattice/algebra/su2algebra/su2field.h"

namespace TempLat
{
  /** \brief A class which implements a SU2 field (algebra). Same than the group, except set 0 component to 0 and has a
   *function which returns its components wrt to sigma/2 instead of sigma, which is more conventional for the algebra
   *(but internally, also expanded as a function of sigma).
   *
   * Unit test: make test-su2liealgebrafield
   **/
  template <size_t NDim, typename T> class SU2LieAlgebraField : public SU2FieldBase<NDim, T>
  {
  public:
    // Put public methods here. These should change very little over time.

    using SU2FieldBase<NDim, T>::fs;

    SU2LieAlgebraField(Field<NDim, T> f1, Field<NDim, T> f2, Field<NDim, T> f3) : SU2FieldBase<NDim, T>(f1, f2, f3) {}

    SU2LieAlgebraField(std::string name, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox,
                       LatticeParameters<T> pLatPar)
        : SU2FieldBase<NDim, T>(name, toolBox, pLatPar)
    {
    }

    template <int N> DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t) const { return operator()(t); }

    template <int N> DEVICE_FORCEINLINE_FUNCTION auto SU2LieAlgebraGet(Tag<N> t) const { return 2 * SU2Get(t); }

    template <int M> auto &operator()(Tag<M> t) { return fs[M - 1]; }
    template <int M> DEVICE_FORCEINLINE_FUNCTION const auto &operator()(Tag<M> t) const { return fs[M - 1]; }
    DEVICE_FORCEINLINE_FUNCTION ZeroType operator()(Tag<0> t) const { return ZeroType(); }

    template <int N, typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t, const IDX &...idx) const
    {
      if constexpr (N > 0)
        return fs[N - 1].get(idx...);
      else
        return ZeroType();
    }

    template <typename R> void operator=(R &&r) { SU2FieldBase<NDim, T>::operator=(r); }

    std::string toString() const { return SU2FieldBase<NDim, T>::toString(); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return SU2FieldBase<NDim, T>::getDx(); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return SU2FieldBase<NDim, T>::getKIR(); }

    using Getter = SU2Getter;
    static constexpr size_t size = 4;
    static constexpr size_t numberToSkipAsTuple = 1;
  };

  struct SU2LieAlgebraFieldTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
