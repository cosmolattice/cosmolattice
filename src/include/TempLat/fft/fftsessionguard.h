#ifndef TEMPLAT_FFT_FFTSESSIONGUARD_H
#define TEMPLAT_FFT_FFTSESSIONGUARD_H

namespace TempLat
{
  /** @brief Yes, another nested class interface: for the session guard. Your constructor should take care of
   * your_library_init(), your destructor should take care of your_library_cleanup(). These are the session-wide
   * initialization / finalization calls.
   *
   */
  class FFTSessionGuard
  {
  public:
    virtual ~FFTSessionGuard() {};
  };
} // namespace TempLat

#endif