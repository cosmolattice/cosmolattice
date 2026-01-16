#ifndef TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_H
#define TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/coordinates/dimensioncountrecorder.h"
#include "TempLat/util/constexpr_for.h"
#include "TempLat/util/random/randomgaussian.h"
#include "TempLat/util/tdd/tdd.h"

#include "TempLat/parallel/device.h"

#include <tuple>

namespace TempLat
{
  MakeException(RandomGaussianFieldNegativeFrequencyException);

  /** \brief A class which initializes your complex random gaussian field. ONLY WORKS FOR FFTW R2C complex layouts. It
   * has a state, only call for monotonically increasing last dimension, and walk an entire rod in the last dimension
   *  before moving sideways. Otherwise this will be freaking slow.
   *  This field is aware of wavenumbers: same layout is repeated
   *  for different resolutions. Increasing the resolution with the
   *  same random seed, means that you keep the same structure that
   *  you had at the lower resolution, with additional high frequency
   *  randomness.
   *
   *  And yet, the current implementation is slow: on each call to get
   *  we allocate a new std::vector, because we now take into account
   *  that the gaussian random field must have hermitian symmetry.
   *  If we didn't do this, FFTW would have no problem and still
   *  produce a real field with C2R and a hermitian symmetric
   *  field with a consequential R2C, but this field then would not
   *  be exactly equal to the input. Just for consistency, we chose to
   *  sacrifice the speed and provide an input which has exactly
   *  the hermitian symmetry which FFTW would otherwise impose.
   *
   * Unit test: make test-randomgaussianfield
   **/
  template <size_t NDim, typename T, bool Real, bool Unitary>
  class RandomGaussianFieldHelper : public DimensionCountRecorder<NDim>
  {
    using RNGInteger = typename RandomGaussian::IntegerType;

  public:
    // Put public methods here. These should change very little over time.
    RandomGaussianFieldHelper(std::string baseSeed, device::memory::host_ptr<MemoryToolBox<NDim>> pToolBox)
        : DimensionCountRecorder<NDim>(SpaceStateType::undefined), mBaseSeed(baseSeed), prng(baseSeed),
          mToolBox(pToolBox), mLayout(mToolBox->mLayouts.getFourierSpaceLayout()), generation(0),
          mGlobalSizes(mLayout.getGlobalSizes())
    {
      DimensionCountRecorder<NDim>::confirmSpace(mLayout, SpaceStateType::Fourier);
    }

    void reset() { generation = 0; }

    void postGet() const
    {
      // This is called after the get, so we can increase the generation.
      generation++;
    }

    DEVICE_FORCEINLINE_FUNCTION std::tuple<RNGInteger, RNGInteger>
    gidx_to_idx2(const device::array<ptrdiff_t, NDim> &gidx) const
    {
      constexpr size_t nd1 = NDim / 2;

      std::tuple<RNGInteger, RNGInteger> result;
      auto &r = std::get<0>(result);
      auto &c = std::get<1>(result);

      RNGInteger dim_length = 1;
      constexpr_for<0, NDim, 1>([&](const auto _i) {
        // We go from the last dimension to the first, so we need to reverse the index.
        constexpr size_t i = NDim - 1 - decltype(_i)::value;
        // If we are in the second half of the dimensions, we sum the index to c
        if constexpr (i > nd1) c += gidx[i] * dim_length;
        // As soon, as we go into the first half, reset the dim_length to 1
        if constexpr (i == nd1) dim_length = 1;
        // If we are in the first half of the dimensions, we sum the index to r
        if constexpr (i <= nd1) r += gidx[i] * dim_length;
        dim_length *= mGlobalSizes[i];
      });

      return result;
    }

    DEVICE_FORCEINLINE_FUNCTION
    complex<T> to_complex(const device::array<double, 2> &pair) const { return complex<T>(pair[0], pair[1]); }

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION complex<T> get(const IDX &...idx) const
    {
      device::array<ptrdiff_t, NDim> global_coord;
      mLayout.putSpatialLocationFromMemoryIndexInto(global_coord, idx...);

      device::array<ptrdiff_t, NDim> hermitianPartner;
      auto hermitianType = DimensionCountRecorder<NDim>::getCurrentLayout().getHermitianPartners().putHermitianPartner(
          global_coord, hermitianPartner);

      // We do not need coordinates actually, but rather (positive!) global indices.
      for (uint d = 0; d < NDim; ++d) {
        if (global_coord[d] < 0) global_coord[d] += mGlobalSizes[d];
        if (hermitianPartner[d] < 0) hermitianPartner[d] += mGlobalSizes[d];
      }

      if (hermitianType == HermitianRedundancy::none) {
        const auto [r, c] = gidx_to_idx2(global_coord);
        const auto val = to_complex(prng.getPair(r, c, generation, Real, Unitary));
        return val;
      } else {
        const auto [r, c] = gidx_to_idx2(hermitianPartner);
        const auto val = to_complex(prng.getPair(r, c, generation, Real, Unitary));
        return (hermitianType == HermitianRedundancy::positivePartner)   ? val
               : (hermitianType == HermitianRedundancy::negativePartner) ? device::conj(val)
               : (hermitianType == HermitianRedundancy::realValued)      ? complex<T>(device::real(val))
                                                                         : complex<T>(0.0, 0.0);
      }
    }

    std::string toString() const { return "Random gaussian field with seed: \"" + mBaseSeed + "\""; }

    const auto getCurrentSeed() const { return prng.getSeed(); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::string mBaseSeed;
    mutable RandomGaussian prng;
    device::memory::host_ptr<MemoryToolBox<NDim>> mToolBox;
    LayoutStruct<NDim> mLayout;
    mutable RNGInteger generation;
    device::array<ptrdiff_t, NDim> mGlobalSizes;
  };

  class RandomGaussianFieldTester
  {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <size_t NDim, typename T> using RandomGaussianField = RandomGaussianFieldHelper<NDim, T, false, false>;

  template <size_t NDim, typename T> using RandomRayleighField = RandomGaussianFieldHelper<NDim, T, true, false>;

  template <size_t NDim, typename T> using RandomUniformUnitaryField = RandomGaussianFieldHelper<NDim, T, false, true>;

} // namespace TempLat

#endif
