#ifndef TEMPLAT_UTIL_CSTYLETIME_H
#define TEMPLAT_UTIL_CSTYLETIME_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/util/tdd/tdd.h"
#include <chrono>

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
    CStyleTime()
    { // available in c++ since c++20
    }

    void now()
    {
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

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
