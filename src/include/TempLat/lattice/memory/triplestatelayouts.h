#ifndef TEMPLAT_LATTICE_MEMORY_TRIPLESTATELAYOUTS_H
#define TEMPLAT_LATTICE_MEMORY_TRIPLESTATELAYOUTS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/lattice/memory/jumpsholder.h"

namespace TempLat
{

  /** @brief A class which understands three layouts of the memory for one MPI task in a global exact hypercube.
   * - receives the pre-FFT configuration space layout (usually padded in the last dimension to hold 2 * (N / 2 + 1 )
   *entries.
   * - receives the Fourier space layout (some fraction of the global N x N x ... x (N / 2 + 1) )
   * - third layout: the configuration space layout *with* ghost cells, *without* FFT padding.
   *   That is: some fraction of the ( g + N + g ) x ( g + N + g ) x ( g + N + g ) x ...
   *
   * But the class doesn't do anything. Just compute the layouts.
   *
   * Unit test: make test-triplestatelayouts
   **/
  template <size_t NDim> class TripleStateLayouts
  {
  public:
    // Put public methods here. These should change very little over time.
    TripleStateLayouts(FFTLayoutStruct<NDim> fftLayout, ptrdiff_t nGhostCells)
        : mFFTLayout(fftLayout), mConfigSpaceWithGhosts_layout(device::IdxArray<NDim>{}, nGhostCells),
          mNGridPoints(fftLayout.getNGridPoints()), mNGhostCells(nGhostCells)
    {
      measureFFTPadding();

      /** FFT's memory layouts are given to us. Construct jumps without padding. */
      mJumps_fftFourierSpace =
          JumpsHolder<NDim>(mFFTLayout.fourierSpace, {{}}, 1
                            /*2 atom size: complex! Ah, no, actually moved to pointers to std::complex, which is the
                               atom now. So back to atom size 1 std::complex, as opposed to 2 double/float. */
          );

      device::array<device::array<ptrdiff_t, 2u>, NDim> tmpPadding{};
      // FFT padding for in-place c2r r2c (see https://fftw.org/fftw3_doc/Multi_002dDimensional-DFTs-of-Real-Data.html)
      // The padding is already included in the memory size of the FFTConfigurationSpace layout.

      /** FFT's memory layouts are given to us. Construct jumps without padding. */
      mJumps_fftConfigSpace = JumpsHolder<NDim>(mFFTLayout.configurationSpace, tmpPadding);

      /** Config space memory layouts are given to us. Construct jumps without FFT padding, with ghost cells. */
      /* first copy all the values, including those to be modified */
      mConfigSpaceWithGhosts_layout = mFFTLayout.configurationSpace;
      device::IdxArray<NDim> localConfigSize{};
      for (size_t i = 0; i < NDim; ++i) {
        tmpPadding[i][0] = mNGhostCells;
        tmpPadding[i][1] = mNGhostCells;
        localConfigSize[i] = mFFTLayout.configurationSpace.getLocalSizes()[i] - mFFTConfigSpacePadding[i];
      }
      mConfigSpaceWithGhosts_layout.setLocalSizes(localConfigSize);
      mConfigSpaceWithGhosts_layout.setNGhosts(mNGhostCells);

      /* hard-coded exact hypercube: same size in all dimensions. */
      mJumps_configSpace = JumpsHolder<NDim>(mConfigSpaceWithGhosts_layout, tmpPadding);

      mNecessaryMemoryAllocation = computeMemSize();
    }

    ptrdiff_t getNGhosts() const { return mNGhostCells; }

    friend std::ostream &operator<<(std::ostream &ostream, const TripleStateLayouts &tsl)
    {
      ostream << "TripleStateLayouts:\n  nDimensions: " << NDim << "\n  nGridPoints: " << tsl.mNGridPoints
              << "\n  nGhostCells: " << tsl.mNGhostCells << "\n  fftConfigSpacePadding: " << tsl.mFFTConfigSpacePadding
              << "\n  necessaryMemoryAllocation: " << tsl.mNecessaryMemoryAllocation
              << "\n  mMemUsedFFTBothSpaces: " << tsl.mMemUsedFFTBothSpaces
              << "\n  memUsedConfigGhostSpace: " << tsl.mMemUsedConfigGhostSpace << "\n\n"
              << tsl.mFFTLayout << "\nconfigurationSpace:\n"
              << tsl.mConfigSpaceWithGhosts_layout << "\njumps:"
              << "\n  fftConfigSpace:\n"
              << tsl.mJumps_fftConfigSpace << "\n  fftFourierSpace:\n"
              << tsl.mJumps_fftFourierSpace << "\n  configSpace:\n"
              << tsl.mJumps_configSpace;
      return ostream;
    }

    operator FFTLayoutStruct<NDim>() const { return mFFTLayout; }

    const auto &getConfigSpaceLayout() const { return mConfigSpaceWithGhosts_layout; }

    const auto &getConfigSpaceStarts() const { return mConfigSpaceWithGhosts_layout.getLocalStarts(); }

    const auto &getConfigSpaceSizes() const { return mConfigSpaceWithGhosts_layout.getLocalSizes(); }

    const JumpsHolder<NDim> &getConfigSpaceJumps() const { return mJumps_configSpace; }

    const auto &getFourierSpaceLayout() const { return mFFTLayout.fourierSpace; }

    const auto &getFourierSpaceStarts() const { return mFFTLayout.fourierSpace.getLocalStarts(); }

    const auto &getFourierSpaceSizes() const { return mFFTLayout.fourierSpace.getLocalSizes(); }

    const JumpsHolder<NDim> &getFourierSpaceJumps() const { return mJumps_fftFourierSpace; }

    const JumpsHolder<NDim> &getFFTConfigSpaceJumps() const { return mJumps_fftConfigSpace; }

    const ptrdiff_t &getNecessaryMemoryAllocation() const { return mNecessaryMemoryAllocation; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    FFTLayoutStruct<NDim> mFFTLayout;
    LayoutStruct<NDim> mConfigSpaceWithGhosts_layout;
    /** @brief Using ptrdiff_t's even though these numbers are positive definite. We want to be safe in subtractions. */
    device::IdxArray<NDim> mNGridPoints;
    ptrdiff_t mNGhostCells;
    ptrdiff_t mMemUsedFFTBothSpaces;
    ptrdiff_t mMemUsedConfigGhostSpace;
    ptrdiff_t mNecessaryMemoryAllocation;

    /** @brief The padding needed in configuration space when being passed to perform an FFT to Fourier space. */
    device::IdxArray<NDim> mFFTConfigSpacePadding;

    JumpsHolder<NDim> mJumps_fftConfigSpace;
    JumpsHolder<NDim> mJumps_fftFourierSpace;
    JumpsHolder<NDim> mJumps_configSpace;

    ptrdiff_t computeMemSize()
    {
      mMemUsedFFTBothSpaces = mFFTLayout.getMinimalMemorySize();

      mMemUsedConfigGhostSpace = 1;
      for (auto &&it : mConfigSpaceWithGhosts_layout.getLocalSizes())
        mMemUsedConfigGhostSpace *= it + 2 * mNGhostCells;

      return std::max(mMemUsedFFTBothSpaces, mMemUsedConfigGhostSpace);
    }

    void measureFFTPadding()
    {
      for (size_t i = 0; i < NDim; ++i)
        mFFTConfigSpacePadding[i] =
            std::max((device::Idx)0, mFFTLayout.configurationSpace.getLocalSizes()[i] - mNGridPoints[i]);
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
