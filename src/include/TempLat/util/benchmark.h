#ifndef TEMPLAT_UTIL_TempLatBENCHMARK_H
#define TEMPLAT_UTIL_TempLatBENCHMARK_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include <functional>
#include <sstream>
#include <iomanip>
#include <list>

#include "TempLat/util/log/saycomplete.h"
#include "TempLat/util/timer.h"

namespace TempLat
{
  /**
   * @brief A class to benchmark your code.
   *
   */
  class Benchmark
  {
  public:
    class Measurer
    {
    public:
      template <typename F> void measure(const std::string tag, F &&f)
      {
        const Timer timer;
        f();
        const size_t elapsed = timer.nanoseconds();
        measurements.emplace_back(tag, elapsed);
      }

      size_t getAverage(const std::string &tag) const
      {
        size_t total = 0;
        size_t count = 0;
        for (const auto &[measurementTag, elapsed] : measurements) {
          if (measurementTag == tag) {
            total += elapsed;
            ++count;
          }
        }
        return count > 0 ? total / count : 0;
      }

      auto getMeasurement(const std::string tag) const
      {
        size_t average = getAverage(tag);
        size_t totalSquaredDiff = 0;
        size_t count = 0;
        for (const auto &[measurementTag, elapsed] : measurements) {
          if (measurementTag == tag) {
            size_t diff = elapsed > average ? elapsed - average : average - elapsed;
            totalSquaredDiff += diff * diff;
            ++count;
          }
        }
        size_t stdD = count > 0 ? std::sqrt(totalSquaredDiff / count) : 0;

        return std::make_tuple(tag, average, stdD, count);
      }

      std::vector<std::string> getTags() const
      {
        std::vector<std::string> tags;
        for (const auto &[tag, _] : measurements) {
          if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
            tags.push_back(tag);
          }
        }
        return tags;
      }

    private:
      std::vector<std::pair<std::string, size_t>> measurements; // tag, elapsed time in nanoseconds
      friend class Benchmark;
    };

    template <typename F> Benchmark(F &&function) : mFunction(std::forward<F>(function)) {}

    void run(size_t n = 0)
    {
      Measurer dead_measurer;
      if (n == 0) {
        say << "Estimating number of iterations to run for the benchmark.\n";
        // If n is 0, we run the function once and check how long it takes.
        Timer timer;
        mFunction(dead_measurer);
        size_t elapsed = timer.nanoseconds();
        // We wish to run the benchmark for no longer than 30s
        n = 30'000'000'000 / elapsed;
        n = std::max(n, size_t(1));
        n = std::min(n, size_t(1000));
      }

      // warmup
      if (n >= 10) {
        say << "Running warmup for " << std::max((size_t)10, n / 2) << " iterations.\n";
        for (uint i = 0; i < std::max((size_t)10, n / 2); ++i)
          mFunction(dead_measurer);
      }

      say << "Running benchmark for " << n << " iterations.\n";

      Measurer measurer;
      for (size_t i = 0; i < n; ++i)
        mFunction(measurer);

      for (const auto &tag : measurer.getTags())
        mMeasurements[tag] = measurer.getMeasurement(tag);
    }

    friend std::ostream &operator<<(std::ostream &os, const Benchmark &bench)
    {
      const size_t tagWidth = 28;
      std::list<std::string> outputs;
      for (const auto &el : bench.mMeasurements) {
        const auto &[measurementTag, measurementData] = el;
        const auto &[tag, average, stdDev, count] = measurementData;

        const auto [averageStr, averageLevel] = formatTime(average, 0);
        const auto [stdDevStr, stdDevLevel] = formatTime(stdDev, averageLevel);
        const std::string countStr = std::to_string(count);

        std::stringstream ss;
        ss << "\033[32mTag:\033[1;4;34m" << std::setw(tagWidth - 4) << measurementTag << "\033[0m" // tag
           << "\n    \033[1;34m|\033[0m Average : " << std::setw(tagWidth - 16 - averageStr.size()) << ""
           << averageStr // average
           << "\n    \033[1;34m|\033[0m Std Dev : " << std::setw(tagWidth - 16 - stdDevStr.size()) << ""
           << stdDevStr // std
           << "\n    \033[1;34m|\033[0m Count :   " << std::setw(tagWidth - 16 - countStr.size()) << ""
           << countStr; // count
        outputs.push_back(ss.str());
      }

      const auto terminal_width(WEXITSTATUS(std::system("exit `tput cols`")));
      const size_t nextTo = (terminal_width - 8) / (tagWidth + 8); // 8 for the spaces in between

      // Now glue the outputs together
      std::string output;
      std::vector<std::string> curLines;
      while (!outputs.empty()) {
        curLines.clear();
        // Take the first nextTo outputs, split them into lines, and glue them together.
        for (size_t i = 0; i < nextTo && !outputs.empty(); ++i) {

          const std::string curOutput = outputs.front();
          outputs.pop_front();

          std::istringstream iss(curOutput);
          std::string line;
          size_t lineCount = 0;
          while (std::getline(iss, line)) {
            if (curLines.size() <= lineCount)
              curLines.push_back("    " + line);
            else
              curLines[lineCount] += "        " + line;
            ++lineCount;
          }
        }

        for (size_t i = 0; i < curLines.size(); ++i)
          output += curLines[i] + "\n";

        output += "\n";
      }

      os << "\nBenchmark results:\n\n" << output;

      return os;
    }

  private:
    std::function<void(Measurer &)> mFunction;

    using Measurement = std::tuple<std::string, size_t, size_t, size_t>; // tag, average, std, count
    std::map<std::string, Measurement> mMeasurements;

    static std::pair<std::string, int> formatTime(size_t time_ns, int lv = 0)
    {
      size_t total = time_ns;
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

      if ((lv == 0 && hours > 0) || (lv == 1 && hours > 0))
        return make_pair(std::to_string(hours) + "h " + std::to_string(min) + "min", 1);
      else if ((lv == 0 && min > 0) || (lv == 2 && min > 0))
        return make_pair(std::to_string(min) + "min " + std::to_string(sec) + "s", 2);
      else if ((lv == 0 && sec > 0) || (lv == 3 && sec > 0))
        return make_pair(std::to_string(sec) + "s " + std::to_string(milli) + "ms", 3);
      else if ((lv == 0 && milli > 0) || (lv == 4 && milli > 0))
        return make_pair(std::to_string(milli) + "ms " + std::to_string(micro) + "us", 4);
      else if ((lv == 0 && micro > 0) || (lv == 5 && micro > 0))
        return make_pair(std::to_string(micro) + "us " + std::to_string(nanoseconds) + "ns", 5);
      else if (lv == 1)
        return make_pair(std::to_string(min) + "min", -2);
      else if (lv == 2)
        return make_pair(std::to_string(sec) + "s", -3);
      else if (lv == 3)
        return make_pair(std::to_string(milli) + "ms", -4);
      else if (lv == 4)
        return make_pair(std::to_string(micro) + "us", -5);
      else if (lv == 5 || lv == 0)
        return make_pair(std::to_string(nanoseconds) + "ns", 6);
      throw std::runtime_error("Invalid level for formatting time: " + std::to_string(lv));
    }
  };
} // namespace TempLat

#endif