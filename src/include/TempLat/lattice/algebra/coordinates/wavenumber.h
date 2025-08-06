#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/util/exception.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/listoperators/vectordotter.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/util/rangeiteration/tag.h"
#include <Kokkos_Macros.hpp>

namespace TempLat
{
  MakeException(WaveNumberWrongSpaceConfirmation);

  /** \brief A class which allows for accessing (unscaled, dimensionless, index-valued) various
   *  expressions involving the fourier coordinates.
   *
   * Unit test: make test-wavenumber
   **/
  template <size_t NDim> class WaveNumber /*: public Vector*/
  {
  public:
    /* Put public methods here. These should change very little over time. */

    WaveNumber(std::shared_ptr<MemoryToolBox<NDim>> toolBox)
        : mToolBox(toolBox), mLayout(toolBox->mLayouts.getFourierSpaceLayout())
    {
    }

    constexpr static size_t getVectorSize() { return NDim; }

    template <std::integral... IDX>
      requires(sizeof...(IDX) == NDim + 1)
    KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
    {
      return get_impl(std::tie(idx...), std::make_index_sequence<sizeof...(IDX) - 1>{});
    }

    template <std::integral... IDX>
      requires(sizeof...(IDX) == NDim + 1)
    KOKKOS_FORCEINLINE_FUNCTION auto vectorGet(const IDX &...idx) const
    {
      return get_impl(std::tie(idx...), std::make_index_sequence<sizeof...(IDX) - 1>{});
    }

    template <std::integral... IDX, size_t... InputIndexes>
    KOKKOS_FORCEINLINE_FUNCTION auto get_impl(std::tuple<const IDX &...> allIdx,
                                              std::index_sequence<InputIndexes...>) const
    {
      auto constexpr lastIdx = sizeof...(IDX) - 1;
      return get_impl(std::get<lastIdx>(allIdx), std::get<InputIndexes>(allIdx)...);
    }

    template <std::integral IDX1, std::integral... IDX>
    KOKKOS_FORCEINLINE_FUNCTION auto get_impl(const IDX1 component, const IDX &...idx) const
    {
      Kokkos::Array<ptrdiff_t, NDim> result;
      mLayout.putSpatialLocationFromMemoryIndexInto(result, idx...);
      return result[component];
    }

    template <int N> auto operator()(Tag<N> t) { return getVectorComponent(*this, N - 1); }

    auto operator[](const ptrdiff_t &i) { return getVectorComponent(*this, i); }
    auto norm2() const { return dot(*this, *this); }
    auto norm() const { return sqrt(dot(*this, *this)); }

    static std::string toString(ptrdiff_t j) { return "k_" + std::to_string(j); }

    static std::string toString() { return "k"; }

    void confirmSpace(ptrdiff_t i, const LayoutStruct<NDim> &newLayout,
                      const SpaceStateInterface<NDim>::SpaceType &spaceType)
    {
      switch (spaceType) {
      case SpaceStateInterface<NDim>::SpaceType::Configuration:
        throw WaveNumberWrongSpaceConfirmation("WaveNumber explicitly only can be used in fourier space. Abort.");
        break;
      case SpaceStateInterface<NDim>::SpaceType::Fourier:
      default:
        break;
      }
    }

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

#endif
