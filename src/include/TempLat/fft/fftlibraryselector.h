#ifndef TEMPLAT_FFT_FFTLIBRARYSELECTOR_H
#define TEMPLAT_FFT_FFTLIBRARYSELECTOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/fft/external/fftw/fftwinterface.h"

#ifndef NOFFT

#ifdef HAVE_MPI
#ifdef HAVE_PARAFAFT
#include "TempLat/fft/external/parafaft/parafaftinterface.h"
#endif
#endif

#ifdef HAVE_KOKKOSFFT
#include "TempLat/fft/external/kokkosfft/kokkosfftinterface.h"
#endif

#endif

namespace TempLat
{
  MakeException(FFTLibraryDoubleInitializationException);

  /**
   * @brief I wrapped this in a struct for a very specific case: If we have multiple translation units (cpp files)
   * which include this header, and each calls getFFTSessionGuards, then we will have multiple static variables, one
   * per translation unit, and the guards will not work as intended. By wrapping it in a struct, we ensure that
   * there is only one instance of the static variable, no matter how many translation units include this header.
   *
   */
  struct holdStaticGuard {
    /** @brief An inline function for storing a static global variable in a header. A lock that
        verifies that we do not accidentally call the FFT initi/fin-alizations twice. */
    static bool getSessionGuardsWasCalledOnce()
    {
      static bool wasOnce = false;
      bool result = wasOnce;
      wasOnce = true;
      return result;
    }
  };

  static inline std::vector<std::shared_ptr<FFTSessionGuard>> getFFTSessionGuards(bool pVerbose = true)
  {
    if (holdStaticGuard::getSessionGuardsWasCalledOnce())
      throw FFTLibraryDoubleInitializationException("You can only call getSessionGuards once.");

    std::vector<std::shared_ptr<FFTSessionGuard>> result;

    /* add your guards here. */
    /* Note: the standard guarantees that the destructors are called in the
     inverse order of appearance in the vector. So if you must construct after FFTW and
     destruct before FFTW, you should be safe if you add your thing after FFTW.
     */
#ifndef NOFFT
    result.push_back(getFFTWSessionGuard(pVerbose));
#ifdef HAVE_MPI

#ifdef HAVE_KOKKOSFFT
    result.push_back(getKokkosFFTSessionGuard(pVerbose));
#endif

#endif
#endif

    return result;
  }

  /** @brief A class which sets up the interface with the appropriate FFT library.
   * Once you have implemented the FFTLibraryInterface for your library, add it to the logic here.
   *
   * Unit test: ctest -R test-fftlibraryselector
   **/
  template <size_t NDim> class FFTLibrarySelector
  {
  public:
    // Put public methods here. These should change very little over time.
    FFTLibrarySelector(MPICartesianGroup group, const device::IdxArray<NDim> &nGridPoints,
                       bool forbidTransposition = false)
        : mGroup(group), mNGridPoints(nGridPoints), mLayout(mNGridPoints), madePlansFloat(false),
          madePlansDouble(false), verbose(false)
    {
      /* here we take the decisions, although the decision to split the group has been made already. */
      [[maybe_unused]] const ptrdiff_t nDimSplit = group.getNumberOfDividedDimensions();

#ifdef HAVE_PARAFAFT
      [[maybe_unused]] constexpr bool haveParafaft = true;
#else
      [[maybe_unused]] constexpr bool haveParafaft = false;
#endif

#ifdef HAVE_KOKKOSFFT
      [[maybe_unused]] constexpr bool haveKOKKOSFFT = true;
#else
      [[maybe_unused]] constexpr bool haveKOKKOSFFT = false;
#endif

      // Priority: Parafaft > KokkosFFT > FFTW (when multi-dimensional split needed)
// MPI case
#ifdef HAVE_MPI
#ifdef HAVE_PARAFAFT
      if constexpr (haveParafaft && (NDim >= 2)) {
        theLibrary = std::make_shared<ParafaftInterface<NDim>>();
        backend = "Parafaft";
      } else
#endif
#endif // HAVE_MPI
      {
#ifdef HAVE_KOKKOSFFT
        if constexpr (haveKOKKOSFFT && (NDim <= 3)) {
          if (group.size() == 1) {
            theLibrary = std::make_shared<KokkosFFTInterface<NDim>>();
            backend = "KokkosFFT";
          } else {
            theLibrary = std::make_shared<FFTWInterface<NDim>>();
            backend = "FFTW";
          }
        } else
#endif // HAVE_KOKKOSFFT
        {
          theLibrary = std::make_shared<FFTWInterface<NDim>>();
          backend = "FFTW";
        }
      }
      if (mGroup.getRank() == 0) sayShort << "Using " << backend << " backend for FFTs.\n";
      mLayout = theLibrary->computeLocalSizes(mGroup, mNGridPoints, forbidTransposition);
    }

    const auto &getLayout() { return mLayout; }

    const std::string &getBackend() const { return backend; }

    void setVerbose() { verbose = true; }

    /* forward all the library calls */
    static ptrdiff_t getMaximumNumberOfDimensionsToDivide(ptrdiff_t nDimensions)
    {
      ptrdiff_t result = FFTWInterface<NDim>().getMaximumNumberOfDimensionsToDivide(nDimensions);
#ifdef HAVE_MPI
#ifdef HAVE_PARAFAFT
      result = ParafaftInterface<NDim>().getMaximumNumberOfDimensionsToDivide(nDimensions);
#endif
#endif
      return result;
    };

    void r2c(MemoryBlock<NDim, double> &mBlock)
    {
      getPlans_double();
      if (verbose) sayMPI << "Going to perform double r2c.\n";
      mPlansDouble->r2c(mBlock);
      mBlock.flagHostMirrorOutdated();
    }

    void r2c(MemoryBlock<NDim, float> &mBlock)
    {
      getPlans_float();
      if (verbose) sayMPI << "Going to perform float r2c.\n";
      mPlansFloat->r2c(mBlock);
      mBlock.flagHostMirrorOutdated();
    }

    void c2r(MemoryBlock<NDim, double> &mBlock)
    {
      getPlans_double();
      if (verbose) sayMPI << "Going to perform double c2r.\n";
      mPlansDouble->c2r(mBlock);
      mBlock.flagHostMirrorOutdated();
    }

    void c2r(MemoryBlock<NDim, float> &mBlock)
    {
      getPlans_float();
      if (verbose) sayMPI << "Going to perform float c2r.\n";
      mPlansFloat->c2r(mBlock);
      mBlock.flagHostMirrorOutdated();
    }

    void getPlans_float()
    {
      if (!madePlansFloat) {
        if (verbose) sayMPI << "Going to prepare float FFT plans.\n";
        madePlansFloat = true;
        mPlansFloat = theLibrary->getPlans_float(mGroup, mLayout);
      }
    }
    void getPlans_double()
    {
      if (!madePlansDouble) {
        if (verbose) sayMPI << "Going to prepare double FFT plans.\n";
        madePlansDouble = true;
        mPlansDouble = theLibrary->getPlans_double(mGroup, mLayout);
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    MPICartesianGroup mGroup;
    device::IdxArray<NDim> mNGridPoints;
    std::shared_ptr<FFTLibraryInterface<NDim>> theLibrary;
    FFTLayoutStruct<NDim> mLayout;

    bool madePlansFloat;
    bool madePlansDouble;
    std::shared_ptr<FFTPlanInterface<NDim, float>> mPlansFloat;
    std::shared_ptr<FFTPlanInterface<NDim, double>> mPlansDouble;

    bool verbose;

    std::string backend;
  };
} // namespace TempLat

#endif
