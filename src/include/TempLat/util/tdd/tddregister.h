#ifndef TEMPLAT_UTIL_TDD_TDDREGISTER_H
#define TEMPLAT_UTIL_TDD_TDDREGISTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cstddef>
#include <vector>
#include <tuple>
#include <iostream>

#include "TempLat/util/demangle.h"
#include "TempLat/util/tdd/tddcontainerbase.h"
#include "TempLat/util/tdd/tddmacros.h"

#include "TempLat/util/log/log.h"
#include "TempLat/util/log/saycomplete.h"

#ifdef HAVE_MPI
#include <mpi.h>
#endif

namespace TempLat
{

  /** @brief A class which registers all unit tests in the currently linked binaries,
   *  and runs all the unit tests when called accordingly.
   *
   *
   * Unit test: ctest -R test-tddregister
   */
  class TDDRegister
  {
  public:
    // Put public methods here. These should change very little over time.
    static void add(TDDContainerBase *ptr, const char *idName)
    {
      if (ptr != NULL && idName != NULL) {
        theList().emplace_back(ptr, Demangle::demangle(idName), 0);
      }
    }

    static ptrdiff_t run()
    {
      /* it's run time! */
      //                say << "Will perform " << theList().size() << " test" << (theList().size() > 1u ? "s" : "") <<
      //                ".\n";
      for (auto &&it : theList()) {

#ifdef HAVE_MPI
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        const bool verbose = rank == 0;
#else
        const bool verbose = true;
#endif
        if (verbose) sayShort << "Starting tests for [" << std::get<1>(it) << "]\n";

#ifdef HAVE_MPI
        MPI_Barrier(MPI_COMM_WORLD);
#endif

        std::get<2>(it) = std::get<0>(it)->Test();

#ifdef HAVE_MPI
        MPI_Barrier(MPI_COMM_WORLD);
#endif

        if (verbose) sayShort << "Finished tests for [" << std::get<1>(it) << "]\n\n\n";

#ifdef HAVE_MPI
        MPI_Barrier(MPI_COMM_WORLD);
#endif
      }
      return summarize();
    }

    static bool isSingleUnitTest() { return theList().size() == 1; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    /** @brief A trick to instantiate the static theList in a header-only file: make it a static variable inside an
     * inline function / method. */
    inline static std::vector<std::tuple<TDDContainerBase *, std::string, ptrdiff_t>> &theList()
    {
      static std::vector<std::tuple<TDDContainerBase *, std::string, ptrdiff_t>> theList;

      return theList;
    }

    static constexpr size_t lineSize = 64;
    static constexpr size_t nameSize = 48;

#ifdef HAVE_MPI
    static ptrdiff_t summarize_MPI()
    {
      int rank, size;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      MPI_Comm_size(MPI_COMM_WORLD, &size);

      ptrdiff_t result = 0;
      if (theList().size() > 0u) {
        std::string line;
        line.resize(lineSize, '-');

        if (rank == 0) {
          std::cerr << "Summary:\n";
          std::cerr << line << "\n";
        }
        /* dumb, let's just do it twice, first only print succes, then show all failure. */
        for (ptrdiff_t sucfail = 0; sucfail < 2; ++sucfail) {
          for (auto &&it : theList()) {
            ptrdiff_t mfailCount = std::get<2>(it);

            // Gather the respective failCounts into an array so we can report on each rank's failures.
            std::vector<int> allFailCounts(size);
            MPI_Gather(&mfailCount, 1, MPI_INT, allFailCounts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

            // Sum up the total fail count across all ranks.
            ptrdiff_t totalFailCount = 0;
            for (int i = 0; i < size; ++i) {
              totalFailCount += allFailCounts[i];
            }

            // Report only on success or only on failure, depending on the value of sucfail.
            if ((totalFailCount && !sucfail) || (!totalFailCount && sucfail)) continue;
            result += totalFailCount;

            // Only rank 0 reports.
            if (rank != 0) continue;

            std::stringstream sstream;
            sstream << "[" + std::get<1>(it) + "]\n";
            for (int r = 0; r < size; ++r) {
              auto str = "    Rank " + std::to_string(r);
              str.resize(nameSize, ' ');
              sstream << str << " - ";
              if (allFailCounts[r] > 0) {
                std::string plurals = allFailCounts[r] == 1 ? "" : "s";
                sstream << allFailCounts[r] << " fail" << plurals << ".\n";
              } else {
                sstream << "all passed.\n";
              }
            }

            if (totalFailCount > 0)
              std::cerr << KRED << sstream.str() << KRESET;
            else
              std::cerr << sstream.str();
          }
        }
        if (rank == 0) {
          std::string tot(" Total:");
          tot.resize(32, ' ');
          std::cerr << line << "\n";
          std::cerr << tot << " - ";
          if (result > 0) {
            std::cerr << result << " fails.\n";
          } else {
            std::cerr << "all passed.\n";
          }
        }
      }
      MPI_Barrier(MPI_COMM_WORLD);
      return result;
    }
#endif

    static ptrdiff_t summarize()
    {
#ifdef HAVE_MPI
      return summarize_MPI();
#endif

      ptrdiff_t result = 0;
      if (theList().size() > 0u) {
        std::string line;
        line.resize(lineSize, '-');

        std::string emptyName;
        emptyName.resize(nameSize, ' ');
        emptyName = "\n" + emptyName;

        std::cerr << "Summary:\n";
        std::cerr << line << "\n";
        /* dumb, let's just do it twice, first only print succes, then show all failure. */
        for (ptrdiff_t sucfail = 0; sucfail < 2; ++sucfail) {
          for (auto &&it : theList()) {

            ptrdiff_t failCount = std::get<2>(it);
            if ((failCount && !sucfail) || (!failCount && sucfail)) continue;
            std::stringstream sstream;

            auto str = "[" + std::get<1>(it) + "]";
            if (str.size() <= nameSize)
              str.resize(nameSize, ' ');
            else
              str += emptyName;
            sstream << str << " - ";
            if (failCount > 0) {
              result += failCount;
              std::string plurals = failCount == 1 ? "" : "s";
              sstream << failCount << " fail" << plurals << ".\n";
            } else {
              sstream << "all passed.\n";
            }

            if (failCount)
              std::cerr << KRED << sstream.str() << KRESET;
            else
              std::cerr << sstream.str();
          }
        }
        std::string tot(" Total:");
        tot.resize(32, ' ');
        std::cerr << line << "\n";
        std::cerr << tot << " - ";
        if (result > 0) {
          std::cerr << result << " fails.\n";
        } else {
          std::cerr << "all passed.\n";
        }
      }
      return result;
    }

  public:
    template <typename TestObjectUnknownHere> static inline void Test(TestObjectUnknownHere &tdd);
  };
} // namespace TempLat

#endif
