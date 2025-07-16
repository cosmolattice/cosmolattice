#ifndef TEMPLAT_UTIL_TempLatTimer_H
#define TEMPLAT_UTIL_TempLatTimer_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include <chrono>

namespace TempLat
{
  /**
   * @brief A simple timer class to measure elapsed time.
   *
   */
  class Timer
  {
  public:
    Timer() : mStart(std::chrono::high_resolution_clock::now()) {}

    size_t seconds() const
    {
      auto end = std::chrono::high_resolution_clock::now();
      return std::chrono::duration_cast<std::chrono::seconds>(end - mStart).count();
    }

    size_t milliseconds() const
    {
      auto end = std::chrono::high_resolution_clock::now();
      return std::chrono::duration_cast<std::chrono::milliseconds>(end - mStart).count();
    }

    size_t microseconds() const
    {
      auto end = std::chrono::high_resolution_clock::now();
      return std::chrono::duration_cast<std::chrono::microseconds>(end - mStart).count();
    }

    size_t nanoseconds() const
    {
      auto end = std::chrono::high_resolution_clock::now();
      return std::chrono::duration_cast<std::chrono::nanoseconds>(end - mStart).count();
    }

    size_t minutes() const
    {
      auto end = std::chrono::high_resolution_clock::now();
      return std::chrono::duration_cast<std::chrono::minutes>(end - mStart).count();
    }

    friend std::ostream &operator<<(std::ostream &os, const Timer &timer)
    {
      size_t total = timer.nanoseconds();

      const size_t nanoseconds = total % 1000;
      total /= 1000;

      const size_t micro = total % 1000;
      total /= 1000;

      const size_t milli = total % 1000;
      total /= 1000;

      const size_t sec = total % 60;
      total /= 60;

      const size_t min = total % 60;
      total /= 60;

      const size_t hours = total;

      os << hours << "h " << min << "m " << sec << "s " << milli << "ms " << micro << "us" << nanoseconds << "ns";

      return os;
    }

  private:
    const std::chrono::high_resolution_clock::time_point mStart;
  };
} // namespace TempLat

#endif