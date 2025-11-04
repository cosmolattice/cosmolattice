#ifndef TEMPLAT_LATTICE_FIELD_COLLECTIONS_FIELDCOLLECTION_H
#define TEMPLAT_LATTICE_FIELD_COLLECTIONS_FIELDCOLLECTION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/abstractfield.h"
#include "TempLat/util/latinindiceslist.h"
#include "TempLat/lattice/field/collections/helpers/id.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include <string>

namespace TempLat
{
  /** \brief A class which
   * Field collections. Allows to have collection of composite vector fields as well.
   *
   *
   * Unit test: make test-fieldcollection
   **/
  template <class Arg, class T, int N, bool flatAssign = false, int SHIFTIND = 0> class FieldCollection
  {
  public:
    static constexpr size_t NDim = GetNDim::get<Arg>();

    FieldCollection(std::string name, std::shared_ptr<MemoryToolBox<NDim>> toolBox,
                    LatticeParameters<T> pLatPar = LatticeParameters<T>())
    {
      for (int i = 0; i < N; ++i) {
        fs.push_back(Arg(name + "_" + std::to_string(i + SHIFTIND), toolBox, pLatPar));
      }
    }

    template <int M> auto operator()(Tag<M> t) const { return fs[t - Tag<SHIFTIND>()]; }
    template <int M> auto operator[](Tag<M> t) const { return fs[t - Tag<SHIFTIND>()]; }
    auto operator()(size_t i) const { return fs[i - Tag<SHIFTIND>()]; }
    auto operator[](size_t i) const { return fs[i - Tag<SHIFTIND>()]; }

    template <int M> auto getComp(Tag<M> t) { return fs[t]; }

    template <typename R> void operator=(R &&r)
    {
      for_in_range<0, std::remove_reference<R>::type::size>(
          [&](auto i) { (*this).getComp(i) = std::remove_reference<R>::type::Getter::get(r, i); });
    }

    template <typename R> void operator+=(R &&r) { (*this) = (*this) + r; }

    void operator=(const FieldCollection<Arg, T, N, flatAssign, SHIFTIND> &other)
    { // overwrite the default = operator.
      for (size_t i = 0; i < fs.size(); ++i)
        fs[i] = other.fs[i];
    }

    std::string toString(ptrdiff_t i) const { return fs[i - SHIFTIND].toString(); }

    using Getter = GetComponent;
    static constexpr size_t size = N;

  private:
    std::vector<Arg> fs;
  };

  struct FieldCollectionTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
