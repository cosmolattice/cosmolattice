#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETTOOLBOX_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETTOOLBOX_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hastoolbox.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"

namespace TempLat
{
  MakeException(GetToolBoxException);

  /** \brief A class which gets jumps from all classes, also those that do not have jumps.
   *
   * Unit test: make test-getjumps
   **/
  class GetToolBox
  {
  public:
    GetToolBox() = delete;

    // Put public methods here. These should change very little over time.
    template <typename U>
      requires HasToolBox<U>
    static auto get(U &&obj)
    {
      return obj.getToolBox();
    }

    template <typename U>
      requires(!HasToolBox<U>)
    static std::nullptr_t get(U &&obj)
    {
      throw GetToolBoxException("Object does not have a toolbox: " + GetString::get(obj));
      return std::nullptr_t();
    }

#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
