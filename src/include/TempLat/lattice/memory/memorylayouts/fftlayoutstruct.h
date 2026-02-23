#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_FFTLAYOUTSTRUCT_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_FFTLAYOUTSTRUCT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"

namespace TempLat
{
  /** @brief The result holder for getIntrinsicRescaleToGetUnnormalizedFFT:
   *  two labeled double values.
   */
  struct IntrinsicScales {
    IntrinsicScales() : c2r(1), r2c(1) {}
    IntrinsicScales(double _c2r, double _r2c) : c2r(_c2r), r2c(_r2c) {}
    /** @brief Value to rescale with after a c2r FFT */
    double c2r;
    /** @brief Value to rescale with after a r2c FFT */
    double r2c;
    /** @brief Whether the scales are default (1,1), i.e. as in FFTW */
    bool isDefault() const { return c2r == 1 && r2c == 1; }
  };

  /** @brief A struct which holds two memory layouts, which are inseparable:
   * - the pre-FFT layout in configuration space, with padding but no ghosting.
   * - the Fourier space (complex values!) layout.
   *
   *
   * Unit test: ctest -R test-fftlayoutstruct
   **/
  template <size_t NDim> class FFTLayoutStruct
  {
  public:
    FFTLayoutStruct(const device::IdxArray<NDim> &nGridPoints, IntrinsicScales scales = IntrinsicScales())
        : configurationSpace(nGridPoints, 0), fourierSpace(LayoutStruct<NDim>::createGlobalFFTLayout(nGridPoints)),
          mNGridPoints(nGridPoints), mExternalMemoryRequirement(0), mScales(scales)
    {
    }

    // yes, public members! That's why its name is "struct".
    LayoutStruct<NDim> configurationSpace;
    LayoutStruct<NDim> fourierSpace;

    // no, these aren't public members. Just getter methods.
    static constexpr size_t getNDimensions() { return NDim; }
    const device::IdxArray<NDim> &getNGridPoints() const { return mNGridPoints; }

    /** @brief Compute on the fly, as our members may be modified by others. That's why OOP... */
    device::Idx getMinimalMemorySize() const
    {
      device::Idx resultC = 2; // complex
      for (auto &&it : fourierSpace.getLocalSizes())
        resultC *= it;

      device::Idx resultR = 1; // real
      for (auto &&it : configurationSpace.getLocalSizes())
        resultR *= it;

      return std::max(mExternalMemoryRequirement, std::max(resultR, resultC));
    }

    /** @brief Your library may require more memory than the simple final layout of the result.
     *  Pass that number here.
     *  This is specifically the case for non-transposed FFTW in/outputs:
     *  needs some extra space for the final transposition into normal layout.
     */
    void addExternalMemoryRequest(device::Idx requirement)
    {
      mExternalMemoryRequirement = std::max(mExternalMemoryRequirement, requirement);
    }

    friend bool operator==(const FFTLayoutStruct &a, const FFTLayoutStruct &b)
    {
      return a.configurationSpace == b.configurationSpace && a.fourierSpace == b.fourierSpace;
    }

    friend std::ostream &operator<<(std::ostream &ostream, const FFTLayoutStruct &ls)
    {
      ostream << "fftConfigurationSpace:\n"
              << ls.configurationSpace << "\n"
              << "fourierSpace:\n"
              << ls.fourierSpace << "\n"
              << "\n  Resulting minimal space needed in memory: " << ls.getMinimalMemorySize() << "\n\n";
      return ostream;
    }

    IntrinsicScales getIntrinsicScales() const { return mScales; }

  private:
    device::IdxArray<NDim> mNGridPoints;
    device::Idx mExternalMemoryRequirement;

    IntrinsicScales mScales;
  };
} // namespace TempLat

#endif
