#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTMEMORYLAYOUT_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTMEMORYLAYOUT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#include <cstddef>
#ifndef NOFFT
#ifdef KOKKOS_FFT
#include <KokkosFFT.hpp>
#endif
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/external/kokkosfft/kokkosfftplanner.h"
#include "TempLat/fft/external/fftw/fftwhermitianpartners.h"

namespace TempLat
{
  MakeException(KokkosFFTMemoryLayoutException);

  /** \brief Implements part of FFTLibraryInterface, computes the local memory associated to the global problem, for
   *KokkosFFT. Build on this to replace KokkosFFT.
   *
   * Unit test: make test-kokkosfftmemorylayout
   **/
  template <size_t NDim> class KokkosFFTMemoryLayout : public KokkosFFTPlanner<NDim>
  {
  public:
    static constexpr bool KOKKOSFFTWITHTRANSPOSITION() { return false; }

    /* Put public methods here. These should change very little over time. */
    KokkosFFTMemoryLayout() {}

    virtual FFTLayoutStruct<NDim> computeLocalSizes(MPICartesianGroup group, std::array<ptrdiff_t, NDim> nGrid,
                                                    bool forbidTransposition = !KOKKOSFFTWITHTRANSPOSITION())
    {
      forbidTransposition = true;

      FFTLayoutStruct<NDim> result(nGrid, false, false, true, IntrinsicScales{1., 1.});
      /* default: everything is local. */

      std::array<ptrdiff_t, NDim> confLocalSizes(nGrid);
      std::array<ptrdiff_t, NDim> confLocalStarts{};
      std::array<ptrdiff_t, NDim> fourLocalSizes(nGrid);
      std::array<ptrdiff_t, NDim> fourLocalStarts{};
      std::array<ptrdiff_t, NDim> fourTransposition{};
      std::iota(fourTransposition.begin(), fourTransposition.end(), 0);

      fourLocalSizes.back() = fourLocalSizes.back() / 2 + 1;
      confLocalSizes.back() = 2 * fourLocalSizes.back();

      ptrdiff_t fftwRequiredMemory = 0;

#ifndef NOMPI
      if (NDim > 1) {
        std::vector<ptrdiff_t> globalLayout(NDim);
        for (size_t i = 0; i < NDim; ++i)
          globalLayout[i] = result.fourierSpace.getLocalSizes()[i];

        bool doTranspose = NDim > 2 && group.size() > 1;

        doTranspose = doTranspose && !forbidTransposition;

        if (doTranspose) {
          fftwRequiredMemory = fftw_mpi_local_size_transposed((int)NDim, globalLayout.data(), group.getComm(),
                                                              confLocalSizes.data(), confLocalStarts.data(),
                                                              fourLocalSizes.data() + 1, fourLocalStarts.data() + 1);
          std::swap(fourTransposition[0], fourTransposition[1]);
        } else {
          fftwRequiredMemory = fftw_mpi_local_size((int)NDim, globalLayout.data(), group.getComm(),
                                                   fourLocalSizes.data(), fourLocalStarts.data());
          std::copy(fourLocalSizes.begin(), fourLocalSizes.end(), confLocalSizes.begin());
          std::copy(fourLocalStarts.begin(), fourLocalStarts.end(), confLocalStarts.begin());
          confLocalSizes.back() *= 2;
        }
      }
#endif

      result.configurationSpace.setLocalSizes(confLocalSizes);
      result.configurationSpace.setLocalStarts(confLocalStarts);
      result.fourierSpace.setLocalSizes(fourLocalSizes);
      result.fourierSpace.setLocalStarts(fourLocalStarts);
      result.fourierSpace.setTranspositionMap_memoryToGlobalSpace(fourTransposition);

      result.addExternalMemoryRequest(2 *
                                      fftwRequiredMemory); /* fftwRequiredMemory is in unit of complex numbers,
                                                              addExternalMemoryRequest expects units of real numbers. */

      result.fourierSpace.setHermitianPartners(
          FFTWHermitianPartners<NDim>::create(result.configurationSpace.getGlobalSizes()));

      return result;
    };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
