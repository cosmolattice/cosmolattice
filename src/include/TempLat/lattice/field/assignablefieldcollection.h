#ifndef TEMPLAT_LATTICE_FIELD_ASSIGNABLEFIELDCOLLECTION_H
#define TEMPLAT_LATTICE_FIELD_ASSIGNABLEFIELDCOLLECTION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/field/field.h"
#include "TempLat/util/shiftedindexsequence.h"
#include "TempLat/util/foreach.h"
#include "TempLat/util/concat.h"
#include "TempLat/util/istuplelike.h"
#include "TempLat/util/tuple_size.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/tuplemaker.h"
#include "TempLat/util/flattentuple.h"

namespace TempLat
{
  // if want to try to have dynamical access somehow, see
  // https://www.justsoftwaresolutions.co.uk/cplusplus/getting-tuple-elements-with-runtime-index.html
  // seems hardly compatible with indexed fields though.

  /** @brief A class which implements collections of object which are assignable.
   *
   *
   * Unit test: ctest -R test-assignablefieldcollection
   **/
  template <class Q, class... Args> class AssignableCollectionBase
  {
  public:
    static constexpr size_t size = sizeof...(Args);

    AssignableCollectionBase(Args... args)
        : // This constructor is used to create composite structure such as SU(N) matrices, from fields.
          fs(std::make_tuple(args...))
    {
    }

    void operator=(const AssignableCollectionBase<Args...> &other)
    {
      for_in_range<0, size>([&](auto j) { std::get<j>(fs) = std::get<j>(other.fs); });
    }

    template <typename R>
      requires(!std::is_same_v<R, AssignableCollectionBase<Args...>>)
    void operator=(R &&r)
    {
      using nakedR = std::decay_t<R>;
      for_in_range<0, size>([&](auto j) { std::get<j>(fs) = nakedR::Getter::get(r, j); });
    }

    template <int M> auto &operator()(Tag<M> t)
    {
      // Also work with operator [], but might be confusing that it is NOT an int (cant call f[1] -> f[1_c])
      return std::get<M - Q::SHIFTIND>(fs);
    }

  protected:
    std::tuple<Args...> fs;
  };

  template <class Q> // Specialise to empty field collection, that does nothing. Useful to define general models for
                     // example, which may have some empty collections.
  class AssignableCollectionBase<Q>
  {
  public:
    // Put public methods here. These should change very little over time.
    AssignableCollectionBase() = default;
    template <int N> int operator()(Tag<N> t) { return 0; }
    template <int N> int getComp(Tag<N> t) { return 0; }
    template <typename R> void operator=(R &&g) {}
    static constexpr size_t size = 0;
  };

  template <class Q, size_t NDim, typename T, CANONICALTYPE ISMOMENTUM, int... I>
  class CollectionBase : public AssignableCollectionBase<Q, Field<NDim, T>>
  {
  public:
    using AssignableCollectionBase<Q, Field<NDim, T>>::operator=;

    // Put public methods here. These should change very little over time.
    template <class... Args>
    CollectionBase(std::string name, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox,
                   LatticeParameters<T> pLatPar = LatticeParameters<T>())
        : AssignableCollectionBase<Q, Args...>(
              Args(name + std::to_string(I - std::get<0>(std::make_tuple(I...))), toolBox, pLatPar)...)
    {
    }

    template <class... Args>
    CollectionBase(Args... args)
        : // This constructor is used to create composite structure such as SU(N) matrices, from fields.
          AssignableCollectionBase<Q, Args...>(args...)
    {
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  template <class Q, size_t NDim, typename T,
            CANONICALTYPE ISMOMENTUM> // Specialise to empty field collection, that does nothing. Useful to define
                                      // general models for example, which may have some empty collections.
  class CollectionBase<Q, NDim, T, ISMOMENTUM> : public AssignableCollectionBase<Q>
  {
  public:
    // Put public methods here. These should change very little over time.
    CollectionBase(std::string name, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox,
                   LatticeParameters<T> pLatPar = LatticeParameters<T>())
    {
    }
  };

  // Now we want to be able to use it like an array; we need to have a way to unpack an integer into a list.
  // This comes from  http://spraetor.github.io/2016/01/02/template-integer-sequence.html
  // Unpack the FieldCollection<T,N> in CollectionBase<T,0,1,2,...> in Olog(N) compile time.

  template <class Q, size_t NDim, typename T, CANONICALTYPE ISMOMENTUM, int Start, int End> struct CollectionHelper;

  template <class Q, size_t NDim, typename T, CANONICALTYPE ISMOMENTUM, int Start, int End>
  using MakeSeqImpl_t = typename CollectionHelper<Q, NDim, T, ISMOMENTUM, Start, End>::type;

  template <class Q, size_t NDim, typename T, CANONICALTYPE ISMOMENTUM, int N>
  using MakeSeq_t = typename CollectionHelper<Q, NDim, T, ISMOMENTUM, 0, N - 1>::type;

  template <class Q, size_t NDim, typename T, CANONICALTYPE ISMOMENTUM, int... I1s, int... I2s>
  struct Concat<CollectionBase<Q, NDim, T, ISMOMENTUM, I1s...>, CollectionBase<Q, NDim, T, ISMOMENTUM, I2s...>> {
    using type = CollectionBase<Q, NDim, T, ISMOMENTUM, I1s..., I2s...>;
  };

  template <class Q, size_t NDim, typename T, CANONICALTYPE ISMOMENTUM, int Start, int End> struct CollectionHelper {
    using type = Concat_t<MakeSeqImpl_t<Q, NDim, T, ISMOMENTUM, Start, (Start + End) / 2>,
                          MakeSeqImpl_t<Q, NDim, T, ISMOMENTUM, (Start + End) / 2 + 1, End>>;
  };

  // break condition:
  template <class Q, size_t NDim, typename T, CANONICALTYPE ISMOMENTUM, int I>
  struct CollectionHelper<Q, NDim, T, ISMOMENTUM, I, I> {
    using type = CollectionBase<Q, NDim, T, ISMOMENTUM, I>;
  };

  // template <typename T, CANONICALTYPE ISMOMENTUM, int N, int SHIFT = 0>
  // using FieldCollection = typename CollectionHelper<T,ISMOMENTUM, SHIFT,N+SHIFT-1>::type;

  // This allows to switch between empty and non empty field collection.

  template <class Q, size_t NDim, typename T, CANONICALTYPE ISMOMENTUM, int N, int SHIFT, bool ENABLE>
  struct CollectionSelector {
    using type = CollectionBase<Q, NDim, T, ISMOMENTUM>;
  };
  template <class Q, size_t NDim, typename T, CANONICALTYPE ISMOMENTUM, int N, int SHIFT>
  struct CollectionSelector<Q, NDim, T, ISMOMENTUM, N, SHIFT, false> {
    using type = typename CollectionHelper<Q, NDim, T, ISMOMENTUM, SHIFT, N + SHIFT - 1>::type;
  };

  template <class Q, size_t NDim, typename T, CANONICALTYPE ISMOMENTUM, int N, int SHIFT>
  using Collection = typename CollectionSelector<Q, NDim, T, ISMOMENTUM, N, SHIFT, N == 0>::type;
} // namespace TempLat

#endif
