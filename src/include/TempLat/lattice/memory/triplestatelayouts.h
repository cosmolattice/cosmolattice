#ifndef TEMPLAT_LATTICE_MEMORY_TRIPLESTATELAYOUTS_H
#define TEMPLAT_LATTICE_MEMORY_TRIPLESTATELAYOUTS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2026

#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructlocal.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructglobal.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"

namespace TempLat
{
  /** @brief A class which keeps three layouts of the memory for one MPI task in a global exact hypercube.
   * - the pre-FFT configuration space layout (usually padded in the last dimension to hold 2 * (N / 2 + 1) entries.
   * - the Fourier space layout (some fraction of the global N x N x ... x (N / 2 + 1) entries)
   * - third layout: the configuration space layout *with* ghost cells, *without* FFT padding.
   *   That is: some fraction of the ( g + N + g ) x ( g + N + g ) x ( g + N + g ) x ...
   *
   * But the class itself doesn't do anything. Just computes the layouts.
   *
   * Unit test: ctest -R test-triplestatelayouts
   **/
  template <size_t NDim> class TripleStateLayouts
  {
  public:
    // Put public methods here. These should change very little over time.
    TripleStateLayouts(FFTLayoutStruct<NDim> fftLayout, device::Idx nGhostCells)
        : mFFTLayout(fftLayout), mConfigSpaceWithGhosts_layout(device::IdxArray<NDim>{}, nGhostCells),
          mNGridPoints(fftLayout.getNGridPoints()), mNGhostCells(nGhostCells)
    {
      // FFT padding for in-place c2r r2c (see https://fftw.org/fftw3_doc/Multi_002dDimensional-DFTs-of-Real-Data.html)
      // The padding is already included in the memory size of the FFTConfigurationSpace layout.

      // Config space memory layouts are given to us. We need to construct the padding due to ghost cells.
      // First, copy all the values, including those to be modified, then we fix the local sizes and padding.
      mConfigSpaceWithGhosts_layout = mFFTLayout.configurationSpace;
      device::array<device::IdxArray<2>, NDim> configPadding{};
      device::IdxArray<NDim> localConfigSize{};
      for (size_t i = 0; i < NDim; ++i) {
        configPadding[i][0] = mNGhostCells;
        configPadding[i][1] = mNGhostCells;
        localConfigSize[i] = mFFTLayout.configurationSpace.getLocalSizes()[i];
      }
      mConfigSpaceWithGhosts_layout.setLocalSizes(localConfigSize);
      mConfigSpaceWithGhosts_layout.setNGhosts(mNGhostCells);
      mConfigSpaceWithGhosts_layout.setPadding(configPadding);

      mNecessaryMemoryAllocation = computeMemSize();
    }

    device::Idx getNGhosts() const { return mNGhostCells; }

    friend std::ostream &operator<<(std::ostream &ostream, const TripleStateLayouts &tsl)
    {
      ostream << "TripleStateLayouts:\n  nDimensions: " << NDim << "\n  nGridPoints: " << tsl.mNGridPoints
              << "\n  nGhostCells: " << tsl.mNGhostCells
              << "\n  fftConfigSpacePadding: " << tsl.getFFTConfigSpaceLayout().getPadding()
              << "\n  necessaryMemoryAllocation: " << tsl.mNecessaryMemoryAllocation
              << "\n  mMemUsedFFTBothSpaces: " << tsl.mMemUsedFFTBothSpaces
              << "\n  memUsedConfigGhostSpace: " << tsl.mMemUsedConfigGhostSpace << "\n\n"
              << tsl.mFFTLayout << "\nconfigurationSpace:\n"
              << tsl.mConfigSpaceWithGhosts_layout;
      return ostream;
    }

    operator FFTLayoutStruct<NDim>() const { return mFFTLayout; }

    const auto &getConfigSpaceLayout() const { return mConfigSpaceWithGhosts_layout; }

    const auto &getConfigSpaceStarts() const { return mConfigSpaceWithGhosts_layout.getLocalStarts(); }

    const auto &getConfigSpaceSizes() const { return mConfigSpaceWithGhosts_layout.getLocalSizes(); }

    const auto &getFFTConfigSpaceLayout() const { return mFFTLayout.configurationSpace; }

    const auto &getFFTConfigSpaceStarts() const { return mFFTLayout.configurationSpace.getLocalStarts(); }

    const auto &getFFTConfigSpaceSizes() const { return mFFTLayout.configurationSpace.getLocalSizes(); }

    const auto &getFourierSpaceLayout() const { return mFFTLayout.fourierSpace; }

    const auto &getFourierSpaceStarts() const { return mFFTLayout.fourierSpace.getLocalStarts(); }

    const auto &getFourierSpaceSizes() const { return mFFTLayout.fourierSpace.getLocalSizes(); }

    const device::Idx &getNecessaryMemoryAllocation() const { return mNecessaryMemoryAllocation; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    const FFTLayoutStruct<NDim> mFFTLayout;
    LayoutStruct<NDim> mConfigSpaceWithGhosts_layout;

    // Using device::Idx's even though these numbers are positive definite. We want to be safe in subtractions.
    const device::IdxArray<NDim> mNGridPoints;
    const device::Idx mNGhostCells;
    device::Idx mMemUsedFFTBothSpaces;
    device::Idx mMemUsedConfigGhostSpace;
    device::Idx mNecessaryMemoryAllocation;

    device::Idx computeMemSize()
    {
      mMemUsedFFTBothSpaces = mFFTLayout.getMinimalMemorySize();

      mMemUsedConfigGhostSpace = 1;
      for (auto &&it : mConfigSpaceWithGhosts_layout.getLocalSizes())
        mMemUsedConfigGhostSpace *= it + 2 * mNGhostCells;

      return std::max(mMemUsedFFTBothSpaces, mMemUsedConfigGhostSpace);
    }
  };

} // namespace TempLat

#endif
