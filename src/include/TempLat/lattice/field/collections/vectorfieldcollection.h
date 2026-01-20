#ifndef TEMPLAT_LATTICE_FIELD_COLLECTIONS_VECTORFIELDCOLLECTION_H
#define TEMPLAT_LATTICE_FIELD_COLLECTIONS_VECTORFIELDCOLLECTION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/collections/vectorfield.h"

namespace TempLat
{
  /** @brief A class which allows to make collections of vector fields.
   *
   *
   * Unit test: make test-vectorfieldcollection
   **/
  template <class Arg, class T, int NDim, int N, int SHIFTIND = 0, bool flatAssign = false> class VectorFieldCollection
  {
  public:
    VectorFieldCollection(std::string name, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox,
                          LatticeParameters<T> pLatPar = LatticeParameters<T>()) //:
    {
      if constexpr (N > 0)
        for (size_t i = 0; i < N; ++i) {
          fs.push_back(VectorField<Arg, T, NDim, flatAssign>(name + "_" + std::to_string(i), toolBox, pLatPar));
        }
    }

    template <int M> auto operator()(Tag<M> t) const { return fs[t - Tag<SHIFTIND>()]; }
    template <int M> auto getComp(Tag<M> t) const { return fs[t]; }

    template <typename R> void operator=(R &&r)
    {
      for_in_range<0, std::decay_t<R>::size>([&](auto i) { (*this).getComp(i) = std::decay_t<R>::Getter::get(r, i); });
    }

    template <typename R> void operator+=(R &&r)
    {
      //  (*this) = (*this) + r;
      for_in_range<0, std::decay_t<R>::size>(
          [&](auto i) { (*this).getComp(i) = (*this).getComp(i) + std::decay_t<R>::Getter::get(r, i); });
    }

    std::string toString(ptrdiff_t i) const { return fs[i - SHIFTIND].toString(); }

    using Getter = GetComponent;
    static constexpr size_t size = N;

  private:
    std::vector<VectorField<Arg, T, NDim, flatAssign>> fs;
  };

  struct VectorFieldCollectionTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
