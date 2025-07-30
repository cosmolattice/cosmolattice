#ifndef TEMPLAT_FFT_FFTLIBRARYSELECTOR_H
#define TEMPLAT_FFT_FFTLIBRARYSELECTOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <functional>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/external/fftw/fftwinterface.h"

#ifndef NOFFT
#ifndef NOMPI
#ifndef NOPFFT
#include "TempLat/fft/external/pfft/pfftinterface.h"
#endif
#endif
#endif

namespace TempLat
{
  MakeException(FFTLibraryDoubleInitializationException);

  /** \brief An inline function for storing a static global variable in a header. A lock that
      verifies that we do not accidentally call the FFT initi/fin-alizations twice. */
  static inline bool getSessionGuardsWasCalledOnce()
  {
    static bool wasOnce = false;
    bool result = wasOnce;
    wasOnce = true;
    return result;
  }

  static inline std::vector<std::shared_ptr<FFTSessionGuard>> getFFTSessionGuards(bool pVerbose = true)
  {
    if (getSessionGuardsWasCalledOnce())
      throw FFTLibraryDoubleInitializationException("You can only call getSessionGuards once.");

    std::vector<std::shared_ptr<FFTSessionGuard>> result;

    /* add your guards here. */
    /* Note: the standard guarantees that the destructors are called in the
     inverse order of appearance in the vector. So if you must construct after FFTW and
     destruct before FFTW, you should be safe if you add your thing after FFTW.
     */
#ifndef NOFFT
    result.push_back(getFFTWSessionGuard(pVerbose));
#ifndef NOMPI
#ifndef NOPFFT
    result.push_back(PFFTInterface().getSessionGuard(pVerbose));
#endif
#endif
#endif

    return result;
  }

  /** \brief A class which sets up the interface with the appropriate FFT library.
   * Once you have implemented the FFTLibraryInterface for your library, add it to the logic here.
   *
   * Unit test: make test-fftlibraryselector
   **/
  template <size_t NDim> class FFTLibrarySelector
  {
  public:
    /* Put public methods here. These should change very little over time. */
    FFTLibrarySelector(MPICartesianGroup group, const std::array<ptrdiff_t, NDim> &nGridPoints,
                       bool forbidTransposition = false)
        : mGroup(group), mNGridPoints(nGridPoints), mLayout(mNGridPoints, true, false), madePlansFloat(false),
          madePlansDouble(false), verbose(false)
    {
      /* here we take the decisions, although the decision to split the group has been made already. */
      ptrdiff_t nDimSplit = group.getNumberOfDividedDimensions();
      bool havePFFT = false;
#ifndef NOMPI
#ifndef NOPFFT
      havePFFT = true;
#endif
#endif
      if (havePFFT && nDimSplit > 1) {
#ifndef NOMPI
#ifndef NOPFFT
        theLibrary = std::make_shared<PFFTInterface>();
#endif
#endif
      } else {
        theLibrary = std::make_shared<FFTWInterface<NDim>>();
      }
      mLayout = theLibrary->computeLocalSizes(mGroup, mNGridPoints, forbidTransposition);
    }

    const auto &getLayout() { return mLayout; }

    void setVerbose() { verbose = true; }

    /* forward all the library calls */
    static ptrdiff_t getMaximumNumberOfDimensionsToDivide(ptrdiff_t nDimensions)
    {
      ptrdiff_t result = FFTWInterface<NDim>().getMaximumNumberOfDimensionsToDivide(nDimensions);
#ifndef NOMPI
#ifndef NOPFFT
      result = PFFTInterface().getMaximumNumberOfDimensionsToDivide(nDimensions);
#endif
#endif
      return result;
    };

    void r2c(MemoryBlock<NDim, double> &mBlock)
    {
      getPlans_double();
      if (verbose) sayMPI << "Going to perform double r2c.\n";
      mPlansDouble->r2c(mBlock);
    }

    void r2c(MemoryBlock<NDim, float> &mBlock)
    {
      getPlans_float();
      if (verbose) sayMPI << "Going to perform float r2c.\n";
      mPlansFloat->r2c(mBlock);
    }

    void c2r(MemoryBlock<NDim, double> &mBlock)
    {
      getPlans_double();
      if (verbose) sayMPI << "Going to perform double c2r.\n";
      mPlansDouble->c2r(mBlock);
    }

    void c2r(MemoryBlock<NDim, float> &mBlock)
    {
      getPlans_float();
      if (verbose) sayMPI << "Going to perform float c2r.\n";
      mPlansFloat->c2r(mBlock);
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
    std::array<ptrdiff_t, NDim> mNGridPoints;
    std::shared_ptr<FFTLibraryInterface<NDim>> theLibrary;
    FFTLayoutStruct<NDim> mLayout;

    bool madePlansFloat;
    bool madePlansDouble;
    std::shared_ptr<FFTPlanInterface<NDim, float>> mPlansFloat;
    std::shared_ptr<FFTPlanInterface<NDim, double>> mPlansDouble;

    bool verbose;

  public:
    template <typename T> static inline void TestBody(TDDAssertion &tdd);

#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
