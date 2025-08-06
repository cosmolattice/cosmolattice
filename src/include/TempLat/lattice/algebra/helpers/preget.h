#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_PREGET_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_PREGET_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /** \brief A class which wraps the calls to confirm(Configuration/Fourier)Space, for classes that have it and objects
   *that don't.
   *
   * Unit test: make test-confirmspace
   **/
  class PreGet
  {
  public:
    /* Put public methods here. These should change very little over time. */
    template <typename U>
      requires requires(U obj) { obj.preGet(); }
    static inline void apply(U &obj)
    {
      obj.preGet();
    }

    template <typename U>
      requires(!requires(U obj) { obj.preGet(); })
    static inline void apply(U &)
    {
      // do nothing
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    PreGet() = delete;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
