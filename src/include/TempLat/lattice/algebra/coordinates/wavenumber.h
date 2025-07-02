#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/listoperators/vectordotter.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/util/rangeiteration/tag.h"
#include <Kokkos_Macros.hpp>
// #include "TempLat/lattice/algebra/vector.h"

namespace TempLat
{
  /** \brief A class which allows for accessing (unscaled, dimensionless, index-valued) various
   *  expressions involving the fourier coordinates.
   *
   * Unit test: make test-wavenumber
   **/
  template <size_t NDim> class WaveNumber /*: public Vector*/
  {
  public:
    /* Put public methods here. These should change very little over time. */
    WaveNumber(std::shared_ptr<MemoryToolBox<NDim>> toolBox) : mToolBox(toolBox)
    {
      mLayout = mToolBox->mLayouts.getFourierSpaceLayout();
    }

    constexpr static size_t getVectorSize() { return NDim; }

    template <std::integral I, std::integral... JDX>
      requires(sizeof...(JDX) == NDim)
    KOKKOS_FORCEINLINE_FUNCTION auto vectorGet(const I i, const JDX... j) const
    {
      return mToolBox->getCoordFourier(i)[j];
    }

    template <int N> auto operator()(Tag<N> t) { return getVectorComponent(*this, N - 1); }

    auto operator[](const ptrdiff_t &i) { return getVectorComponent(*this, i); }
    auto norm2() { return dot(*this, *this); }
    auto norm() { return pow(this->norm2(), 0.5); }

    std::string toString(ptrdiff_t j) const { return "k_" + std::to_string(j); }

    std::string toString() const { return "k"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::shared_ptr<MemoryToolBox<NDim>> mToolBox;
    LayoutStruct<NDim> mLayout;
  };

  template <size_t NDim> using FourierSite = WaveNumber<NDim>;

  struct WaveNumberTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat
#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/coordinates/wavenumber_test.h"
#endif

#endif
