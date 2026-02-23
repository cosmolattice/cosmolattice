#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_FFTWMEMORYLAYOUT_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_FFTWMEMORYLAYOUT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <algorithm>

#ifndef NOFFT
#ifdef HAVE_MPI
#include "fftw3-mpi.h"
#endif
#endif

#ifndef NOFFT
#include "fftw3.h"
#endif

#include "TempLat/fft/external/fftw/fftwplanner.h"
#include "TempLat/fft/external/fftw/fftwhermitianpartners.h"

#include <numeric>

namespace TempLat
{
  /** @brief Implements part of FFTLibraryInterface, computes the local memory associated to the global problem, for
   *FFTW.
   *
   *
   * Unit test: ctest -R test-fftwmemorylayout
   **/
  template <size_t NDim> class FFTWMemoryLayout : public FFTWPlanner<NDim>
  {
  public:
    // Put public methods here. These should change very little over time.
    FFTWMemoryLayout() = default;

    virtual FFTLayoutStruct<NDim> computeLocalSizes(MPICartesianGroup group, device::IdxArray<NDim> nGridPoints,
                                                    bool forbidTransposition = false)
    {
      FFTLayoutStruct<NDim> result(nGridPoints);

      device::IdxArray<NDim> confLocalSizes(nGridPoints);
      device::IdxArray<NDim> confLocalStarts{};
      device::IdxArray<NDim> fourLocalSizes(nGridPoints);
      device::IdxArray<NDim> fourLocalStarts{};
      device::IdxArray<NDim> fourTransposition{};
      device::array<device::IdxArray<2>, NDim> confPadding{};
      std::iota(fourTransposition.begin(), fourTransposition.end(), 0);

      fourLocalSizes[NDim - 1] = fourLocalSizes.back() / 2 + 1;
      // That's the padding for r2c/cr2, making place for that additional complex value
      confPadding[NDim - 1][1] = 2;

      device::Idx fftwRequiredMemory = 0;

#ifdef HAVE_MPI
      if (NDim > 1) {
        std::vector<ptrdiff_t> globalLayout(NDim);
        for (size_t i = 0; i < NDim; ++i)
          globalLayout[i] = result.fourierSpace.getLocalSizes()[i];

        bool doTranspose = NDim > 2 && group.size() > 1;

        doTranspose = doTranspose && !forbidTransposition;

        if (doTranspose) {
          ptrdiff_t tmp_ln0, tmp_ls0, tmp_ln1, tmp_ls1;
          fftwRequiredMemory = fftw_mpi_local_size_transposed((int)NDim, globalLayout.data(), group.getComm(), &tmp_ln0,
                                                              &tmp_ls0, &tmp_ln1, &tmp_ls1);
          confLocalSizes[0] = tmp_ln0;
          confLocalStarts[0] = tmp_ls0;
          fourLocalSizes[1] = tmp_ln1;
          fourLocalStarts[1] = tmp_ls1;
          std::swap(fourTransposition[0], fourTransposition[1]);
        } else {
          ptrdiff_t tmp_ln0, tmp_ls0;
          fftwRequiredMemory = fftw_mpi_local_size((int)NDim, globalLayout.data(), group.getComm(), &tmp_ln0, &tmp_ls0);
          fourLocalSizes[0] = tmp_ln0;
          fourLocalStarts[0] = tmp_ls0;
          std::copy(fourLocalSizes.begin(), fourLocalSizes.end(), confLocalSizes.begin());
          std::copy(fourLocalStarts.begin(), fourLocalStarts.end(), confLocalStarts.begin());
          confLocalSizes.back() *= 2;
        }
      }
#endif

      confLocalSizes.back() = nGridPoints.back();

      result.configurationSpace.setLocalSizes(confLocalSizes);
      result.configurationSpace.setLocalStarts(confLocalStarts);
      result.configurationSpace.setPadding(confPadding);

      result.fourierSpace.setLocalSizes(fourLocalSizes);
      result.fourierSpace.setLocalStarts(fourLocalStarts);
      result.fourierSpace.setTranspositionMap_memoryToGlobalSpace(fourTransposition);

      // fftwRequiredMemory is in unit of complex numbers, addExternalMemoryRequest expects units of real numbers.
      result.addExternalMemoryRequest(2 * fftwRequiredMemory);

      result.fourierSpace.setHermitianPartners(
          FFTWHermitianPartners<NDim>::create(result.configurationSpace.getGlobalSizes()));

      return result;
    };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

} // namespace TempLat

#endif
