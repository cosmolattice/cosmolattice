#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/listoperators/vectordotter.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  MakeException(WaveNumberWrongSpaceConfirmation);

  /** @brief A class which allows for accessing (unscaled, dimensionless, index-valued) various
   *  expressions involving the fourier coordinates.
   *
   * Unit test: ctest -R test-wavenumber
   **/
  template <size_t NDim> class WaveNumber /*: public Vector*/
  {
  public:
    // Put public methods here. These should change very little over time.

    WaveNumber(device::memory::host_ptr<MemoryToolBox<NDim>> toolBox)
        : mLayout(toolBox->mLayouts.getFourierSpaceLayout())
    {
    }

    DEVICE_FUNCTION
    WaveNumber(const WaveNumber &) = default;

    constexpr static size_t getVectorSize() { return NDim; }

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      device::IdxArray<NDim> result;
      mLayout.putSpatialLocationFromMemoryIndexInto(result, idx...);
      return result;
    }

    template <int N>
      requires(N > 0)
    auto vectorGet(Tag<N> t) const
    {
      return getVectorComponent(*this, Tag<N - 1>());
    }

    template <int N>
      requires(N > 0)
    auto operator()(Tag<N> t) const
    {
      return vectorGet(t);
    }

    auto norm2() const { return dot(*this, *this); }
    auto norm() const { return sqrt(dot(*this, *this)); }

    static std::string toString(ptrdiff_t j) { return "k_" + std::to_string(j); }
    static std::string toString() { return "k"; }

    void confirmSpace(ptrdiff_t i, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
    {
      switch (spaceType) {
      case SpaceStateType::Configuration:
        throw WaveNumberWrongSpaceConfirmation("WaveNumber explicitly only can be used in fourier space. Abort.");
        break;
      case SpaceStateType::Fourier:
      default:
        break;
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    LayoutStruct<NDim> mLayout;
  };

  template <size_t NDim> using FourierSite = WaveNumber<NDim>;
} // namespace TempLat

#endif
