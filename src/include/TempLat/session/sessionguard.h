#ifndef TEMPLAT_SESSION_SESSIONGUARD_H
#define TEMPLAT_SESSION_SESSIONGUARD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/parallel/device_guard.h"
#include "TempLat/parallel/mpi/session/mpiguard.h"
#include "TempLat/util/exception.h"

namespace TempLat
{
  MakeException(SessionGuardInstantiationException);

  /** @brief A class which holds all the guards: fftw, other fft libraries and mpi.
   * Only one instance per process is allowed. Throws an exception if that condition is violated.
   *
   * Unit test: ctest -R test-sessionguard
   **/
  class SessionGuard
  {
  public:
    // Put public methods here. These should change very little over time.
    SessionGuard(int argc, char *argv[], bool verbose = false)
        : instanceProtectionKey(InstanceCounter(1)), mMPIGuard(argc, argv, verbose), mDeviceGuard(argc, argv, verbose),
          mFFTSessionGuards(getFFTSessionGuards(verbose))
    {
    }

    // Getter for testing purposes only. Allows tests to check the current instance counter.
    static inline int GetInstanceCounter() { return InstanceCounter(); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    int instanceProtectionKey;
    MPIGuard mMPIGuard;
    DeviceGuard mDeviceGuard;
    std::vector<std::shared_ptr<FFTSessionGuard>> mFFTSessionGuards;

    static inline int InstanceCounter(int delta = 0)
    {
      static int counter = 0;
      counter += delta;
      if (counter > 1)
        throw SessionGuardInstantiationException("Per process, the MPIGuard can be instantiated only once. This should "
                                                 "be done in `int main()`. This is wrong. Instances:",
                                                 counter);
      return counter;
    }
  };
} // namespace TempLat

#endif
