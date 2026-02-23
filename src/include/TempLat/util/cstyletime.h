#ifndef TEMPLAT_UTIL_CSTYLETIME_H
#define TEMPLAT_UTIL_CSTYLETIME_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include <chrono>
#if !defined(__cpp_lib_chrono) || __cpp_lib_chrono < 201907L
#include <ctime>
#endif

namespace TempLat
{
  /** @brief A class which returns date and time, pre C++17.
   *
   *
   * Unit test: ctest -R test-cstyletime
   **/
  class CStyleTime
  {
  public:
    // Put public methods here. These should change very little over time.
    CStyleTime() = default;

    void now()
    {
// Apple Clang's libc++ does not yet support C++20 chrono timezone features
// (zoned_time, current_zone), so we fall back to C-style localtime.
#if defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
      using namespace std::chrono;
      const auto now = zoned_time{current_zone(), system_clock::now()}.get_local_time();
      const auto td = floor<days>(now);
      const auto ts = floor<seconds>(now - td);
      const year_month_day ymd{td};
      const hh_mm_ss hms{ts};

      year = static_cast<int>(ymd.year());
      month = static_cast<unsigned>(ymd.month());
      day = static_cast<unsigned>(ymd.day());
      hour = hms.hours().count();
      minute = hms.minutes().count();
      second = hms.seconds().count();
#else
      time_t t = std::time(0);
      tm *ltm = localtime(&t);
      year = 1900 + ltm->tm_year;
      month = 1 + ltm->tm_mon;
      day = ltm->tm_mday;
      hour = ltm->tm_hour;
      minute = ltm->tm_min;
      second = ltm->tm_sec;
#endif
    }

    std::string date(std::string sep = "_")
    {
      return "d" + std::to_string(day) + sep + "m" + std::to_string(month) + sep + "y" + std::to_string(year);
    }

    std::string time(std::string sep = "_")
    {
      return "h" + std::to_string(hour) + sep + "m" + std::to_string(minute) + sep + "s" + std::to_string(second);
    }

    std::string approxTime(std::string sep = "_")
    {
      return "h" + std::to_string(hour) + sep + "m" + std::to_string(minute);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    int day, month, year, second, minute, hour;
  };

} // namespace TempLat

#endif
