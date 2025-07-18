#ifndef TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_H
#define TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/coordinates/dimensioncountrecorder.h"
#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/util/constexpr_for.h"
#include "TempLat/util/random/randomgaussian.h"
#include "TempLat/util/tdd/tdd.h"
#include <tuple>

namespace TempLat
{
  template <size_t NDim, std::integral... IDX>
  KOKKOS_FORCEINLINE_FUNCTION Kokkos::Array<ptrdiff_t, NDim> ndIdxToCoordinate(const LayoutStruct<NDim> &layout,
                                                                               const IDX &...idx)
  {
    Kokkos::Array<ptrdiff_t, NDim> res{(ptrdiff_t)(idx)...};
    for (size_t i = 0; i < NDim; ++i) {
      const ptrdiff_t &tSize = layout.getGlobalSizes()[i] / 2;
      res[i] += layout.getLocalStarts()[i];
      res[i] = res[i] > tSize ? res[i] - layout.getGlobalSizes()[i] : res[i];
    }
    return res;
  }

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
  public:
    /* Put public methods here. These should change very little over time. */
    RandomGaussianFieldHelper(std::string baseSeed, std::shared_ptr<MemoryToolBox<NDim>> pToolBox)
        : DimensionCountRecorder<NDim>(SpaceStateInterface<NDim>::SpaceType::undefined), mBaseSeed(baseSeed),
          prng(baseSeed), prng_hermitian(baseSeed), mToolBox(pToolBox),
          mLayout(mToolBox->mLayouts.getFourierSpaceLayout())
    {
      DimensionCountRecorder<NDim>::confirmSpace(mLayout, SpaceStateInterface<NDim>::SpaceType::Fourier);
      mRodSize = 1;

      for (size_t i = 0; i < NDim; ++i) {
        mLocalStarts[i] = mLayout.getLocalStarts()[i];
        mLocalSizes[i] = mLayout.getLocalSizes()[i];
        mGlobalSizes[i] = mLayout.getGlobalSizes()[i];
      }

      size_t hermitian_size = 1;
      for (size_t i = 0; i < NDim - 1; ++i)
        hermitian_size *= mLayout.getGlobalSizes()[i];
      prng_hermitian = Util::RandomGaussian(mBaseSeed + "_Hermitian", hermitian_size);
      precomputed_hermitian = Kokkos::View<complex<double> *, Kokkos::DefaultExecutionSpace>(
          "PrecomputedHermitianRandomValues", hermitian_size);

      precomputeHermitian();
    }

    void reset()
    {
      Kokkos::fence();
      prng = Util::RandomGaussian(mBaseSeed);
    }

    void precomputeHermitian()
    {
      // Fill the precomputed hermitian values.
      Kokkos::parallel_for(
          Kokkos::RangePolicy(0, precomputed_hermitian.size()), KOKKOS_CLASS_LAMBDA(const size_t idx) {
            const auto pair = prng_hermitian.getNextPair(idx, Real, Unitary);
            precomputed_hermitian(idx).imag() = pair[0];
            precomputed_hermitian(idx).real() = pair[1];
          });
    }

    Kokkos::View<complex<double> *, Kokkos::DefaultExecutionSpace> precomputed_hermitian;

    template <std::integral... IDX>
      requires(sizeof...(IDX) == NDim)
    KOKKOS_FORCEINLINE_FUNCTION complex<T> get(const IDX &...idx) const
    {
      Kokkos::Array<ptrdiff_t, NDim> global_coord = ndIdxToCoordinate(mLayout, idx...);
      Kokkos::Array<ptrdiff_t, NDim> hermitianPartner;

      auto hermitianType = DimensionCountRecorder<NDim>::getCurrentLayout().getHermitianPartners().putHermitianPartner(
          global_coord, hermitianPartner);

#ifdef NOKOKKOS
      // TODO implement this also for the kokkos version...
      updatePRNG(hermitianPartner);
      auto pair = prng.getNextPair(Real, Unitary);
      /* ordered the if-statement by most-occurring case first. */
      return complex<T>(pair[0], hermitianType == HermitianRedundancy::none ||
                                         hermitianType == HermitianRedundancy::positivePartner
                                     ? pair[1]
                                 : hermitianType == HermitianRedundancy::negativePartner ? -pair[1]
                                                                                         : T(0));
#else
      if (hermitianType == HermitianRedundancy::none) {
        size_t local_idx = 1;
        size_t dim_length = 1;
        constexpr_for<0, NDim, 1>([&](const auto _i) {
          constexpr size_t i = NDim - 1 - decltype(_i)::value;
          local_idx += std::get<i>(std::tie(idx...)) * dim_length;
          dim_length *= mLocalSizes[i];
        });
        return prng.getNextPair(local_idx % prng_hermitian.getPoolSize(), Real, Unitary)[0];
      } else {
        size_t hermitian_idx = 1;
        size_t dim_length = 1;
        constexpr_for<0, NDim - 1, 1>([&](const auto _i) {
          constexpr size_t i = NDim - 2 - decltype(_i)::value;
          hermitian_idx += std::get<i>(std::tie(idx...)) * dim_length;
          dim_length *= mGlobalSizes[i];
        });
        return (hermitianType == HermitianRedundancy::positivePartner) ? precomputed_hermitian(hermitian_idx)
               : (hermitianType == HermitianRedundancy::negativePartner)
                   ? Kokkos::conj(precomputed_hermitian(hermitian_idx))
               : (hermitianType == HermitianRedundancy::realValued)
                   ? complex<T>(Kokkos::real(precomputed_hermitian(hermitian_idx)))
                   : complex<T>(0.0, 0.0);
      }
#endif
    }

    std::string toString() const { return "Random gaussian field with seed: \"" + mBaseSeed + "\""; }

    const auto getCurrentSeed() const { return prng.getSeed(); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::string mBaseSeed;
    size_t mRodSize;
    mutable std::vector<ptrdiff_t> rodPosition;
    mutable Util::RandomGaussian prng;
    mutable Util::RandomGaussian prng_hermitian;
    std::shared_ptr<MemoryToolBox<NDim>> mToolBox;

    LayoutStruct<NDim> mLayout;

    Kokkos::Array<ptrdiff_t, NDim> mLocalStarts;
    Kokkos::Array<ptrdiff_t, NDim> mLocalSizes;
    Kokkos::Array<ptrdiff_t, NDim> mGlobalSizes;

    /** \brief Verifies that the coordinates asked for are
     *  on the same rod that we are in, and that the last dimension
     *  is monotonically growing.
     *  If not, rebuild the prng.
     */
    void updatePRNG(const std::array<ptrdiff_t, NDim> &coordinates) const
    {
      bool needUpdate = rodPosition.size() < NDim;

      /* can only monotonically grow. Are we going down? Restart from zero. */
      needUpdate = needUpdate || rodPosition.back() > coordinates[NDim - 1];
      if (needUpdate) rodPosition.resize(NDim);
      for (size_t i = 0; i < NDim - 1; ++i) {
        needUpdate = needUpdate || rodPosition[i] != coordinates[i];
        rodPosition[i] = coordinates[i];
      }
      if (needUpdate) {
        std::string rodPositionString = "";
        for (size_t i = 0; i < NDim - 1; ++i) {
          rodPositionString += std::to_string(rodPosition[i]) + ", ";
        }
        prng = Util::RandomGaussian(mBaseSeed + " " + rodPositionString);
        rodPosition.back() = 0;
      }

      /* next, update the prng to be at one step before coordinates[NDim - 1] */
      ptrdiff_t goal = coordinates[NDim - 1];

      if (goal < 0)
        throw RandomGaussianFieldNegativeFrequencyException(
            "Random gaussian field assumes a positive wavenumber-only layout of the rods, exactly as in FFTW's R2C "
            "transforms. Sorry, you asked for a negative frequency.");

      while (((ptrdiff_t)prng.getState() / 2) < goal)
        prng.getNextPair();

      rodPosition.back() = coordinates[NDim - 1];
    }
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

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/random/randomgaussianfield_test.h"
#endif

#endif
