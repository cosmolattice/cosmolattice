#ifndef TEMPLAT_UTIL_DEBUG_MPIDEBUGGERHANGER_H
#define TEMPLAT_UTIL_DEBUG_MPIDEBUGGERHANGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include <cstdio>
#include <unistd.h>

namespace TempLat
{
  /** @brief A class which can be used to hang a serial debugger like gdb to individual threads.
   *
   *
   * Unit test: ctest -R test-mpidebuggerhanger
   **/
  class MPIDebuggerHanger
  {
  public:
    // Put public methods here. These should change very little over time.
    MPIDebuggerHanger() = delete;

    static void hang()
    {
      volatile int i = 0;
      char hostname[256];
      gethostname(hostname, sizeof(hostname));
      printf("PID %d on %s ready for attach\n", getpid(), hostname);
      fflush(stdout);
      while (0 == i)
        sleep(5);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };
} // namespace TempLat

#endif
