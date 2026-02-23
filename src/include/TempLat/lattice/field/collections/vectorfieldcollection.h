#ifndef TEMPLAT_LATTICE_FIELD_COLLECTIONS_VECTORFIELDCOLLECTION_H
#define TEMPLAT_LATTICE_FIELD_COLLECTIONS_VECTORFIELDCOLLECTION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/field/collections/vectorfield.h"

namespace TempLat
{
  /** @brief A class which allows to make collections of vector fields.
   *
   *
   * Unit test: ctest -R test-vectorfieldcollection
   **/
  template <class Arg, int N, int SHIFTIND = 0, bool flatAssign = false> class VectorFieldCollection
  {
  public:
    static constexpr size_t NDim = GetNDim::get<Arg>();
    using RT = GetGetReturnType<Arg>::type;
    using T = typename GetFloatType<RT>::type;

    VectorFieldCollection(std::string name, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox,
                          LatticeParameters<T> pLatPar = LatticeParameters<T>()) //:
    {
      if constexpr (N > 0)
        for (int i = 0; i < N; ++i) {
          fs.push_back(VectorField<Arg, flatAssign>(name + "_" + std::to_string(i), toolBox, pLatPar));
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
    std::vector<VectorField<Arg, flatAssign>> fs;
  };
} // namespace TempLat

#endif
