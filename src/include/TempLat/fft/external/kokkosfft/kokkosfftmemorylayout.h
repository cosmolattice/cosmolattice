#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTMEMORYLAYOUT_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTMEMORYLAYOUT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#include <cstddef>
#ifndef NOFFT
#ifdef HAVE_KOKKOSFFT
#include <KokkosFFT.hpp>
#endif
#endif

#include "TempLat/fft/external/kokkosfft/kokkosfftplanner.h"
#include "TempLat/fft/external/fftw/fftwhermitianpartners.h"

namespace TempLat
{
  MakeException(KokkosFFTMemoryLayoutException);

  /** @brief Implements part of FFTLibraryInterface, computes the local memory associated to the global problem, for
   *KokkosFFT. Build on this to replace KokkosFFT.
   *
   * Unit test: ctest -R test-kokkosfftmemorylayout
   **/
  template <size_t NDim> class KokkosFFTMemoryLayout : public KokkosFFTPlanner<NDim>
  {
  public:
    static constexpr bool KOKKOSFFTWITHTRANSPOSITION() { return false; }

    // Put public methods here. These should change very little over time.
    KokkosFFTMemoryLayout() = default;

    virtual FFTLayoutStruct<NDim> computeLocalSizes(MPICartesianGroup group, device::IdxArray<NDim> nGrid,
                                                    bool forbidTransposition = !KOKKOSFFTWITHTRANSPOSITION())
    {
      forbidTransposition = true || forbidTransposition;

      FFTLayoutStruct<NDim> result(nGrid, IntrinsicScales{1., 1.});

      device::IdxArray<NDim> confLocalSizes(nGrid);
      device::IdxArray<NDim> confLocalStarts{};
      device::IdxArray<NDim> fourLocalSizes(nGrid);
      device::IdxArray<NDim> fourLocalStarts{};
      device::IdxArray<NDim> fourTransposition{};
      device::array<device::IdxArray<2>, NDim> confPadding{};
      std::iota(fourTransposition.begin(), fourTransposition.end(), 0);

      fourLocalSizes[NDim - 1] = fourLocalSizes.back() / 2 + 1;
      // r2c/cr2 padding for the last dimension, KokkosFFT uses the same convention as FFTW
      confPadding[NDim - 1][1] = 2;

      result.configurationSpace.setLocalSizes(confLocalSizes);
      result.configurationSpace.setLocalStarts(confLocalStarts);
      result.configurationSpace.setPadding(confPadding);

      result.fourierSpace.setLocalSizes(fourLocalSizes);
      result.fourierSpace.setLocalStarts(fourLocalStarts);
      result.fourierSpace.setTranspositionMap_memoryToGlobalSpace(fourTransposition);

      result.fourierSpace.setHermitianPartners(
          FFTWHermitianPartners<NDim>::create(result.configurationSpace.getGlobalSizes()));

      return result;
    };
  };

} // namespace TempLat

#endif
