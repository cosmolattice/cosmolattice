#ifndef TEMPLAT_UTIL_TempLatARRAY_H
#define TEMPLAT_UTIL_TempLatARRAY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/rangeiteration/for_in_range.h"

namespace TempLat
{
  /** @brief A class which makes array compatible which getComp algebra.
   *
   *
   * Unit test: ctest -R test-templatarray
   **/
  template <typename T, int N, int shift = 0> class TempLatArray
  {
  public:
    // Put public methods here. These should change very little over time.
    TempLatArray() = default;

    TempLatArray(const std::array<T, N> &vec) : mVec(vec) {}
    TempLatArray(std::array<T, N> &&vec) : mVec(std::move(vec)) {}

    void operator=(const std::vector<T> &vec)
    {
      for (size_t i = 0; i < N; ++i)
        mVec[i] = vec[i];
    }

    template <typename S> void operator=(S &&vec) { ForLoop(i, 0, N - 1, mVec[i] = DoEval::eval(vec.getComp(i))); }

    std::array<T, N> &asArr() { return mVec; }

    template <int M> T getComp(Tag<M> t) const noexcept
    {
      static_assert(M >= 0 && M < N, "Index out of bounds in TempLatArray::getComp");
      return mVec[M];
    }

    T operator[](ptrdiff_t i) const
    {
      // Index checking here makes sense:
      // - access happens very few times (accessing fields, not lattice sites)
      // - user errors are very likely here, so it's nice to tell them when they do something wrong
      if (i < 0 || i >= ptrdiff_t(N)) {
        throw std::out_of_range("Index out of bounds in TempLatArray::operator[], bounds are [0," + std::to_string(N) +
                                "), got " + std::to_string(i));
      }
      return mVec[i];
    }

    T operator()(ptrdiff_t i) const
    {
      // see above
      if (i < shift || i >= ptrdiff_t(N) + shift) {
        throw std::out_of_range("Index out of bounds in TempLatArray::operator[], bounds are [0," + std::to_string(N) +
                                "), got " + std::to_string(i));
      }
      return mVec[i - shift];
    }

    T &operator[](ptrdiff_t i)
    {
      // see above
      if (i < 0 || i >= ptrdiff_t(N)) {
        throw std::out_of_range("Index out of bounds in TempLatArray::operator[], bounds are [0," + std::to_string(N) +
                                "), got " + std::to_string(i));
      }
      return mVec[i];
    }

    T &operator()(ptrdiff_t i)
    {
      // see above
      if (i < shift || i >= ptrdiff_t(N) + shift) {
        throw std::out_of_range("Index out of bounds in TempLatArray::operator[], bounds are [0," + std::to_string(N) +
                                "), got " + std::to_string(i));
      }
      return mVec[i - shift];
    }

    // static constexpr size_t size() { return N; }

    using Getter = GetComponent;
    static constexpr size_t size = N;

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::array<T, N> mVec;
  };
} // namespace TempLat

#endif
