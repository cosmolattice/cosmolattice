#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYTOOLBOX_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYTOOLBOX_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"
#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/fft/fftmpidomainsplit.h"
#include "TempLat/fft/fftnormalization.h"

#include "TempLat/lattice/memory/triplestatelayouts.h"

#include "TempLat/lattice/ghostcells/ghostbuster.h"
#include "TempLat/lattice/ghostcells/ghostupdater.h"

#include "TempLat/lattice/memory/verbositylevels.h"
#include "TempLat/util/makeuniformarray.h"

#include "TempLat/parallel/device_memory.h"

#include <cstddef>
#include <sstream>
#include <stdexcept>
// #include "TempLat/lattice/algebra/shift.h"

namespace TempLat
{
  MakeException(InconsistentDomainSplitting);
  MakeException(InconsistentDimensions);

  /** @brief A class which collects all the tool classes which deal with memory(-state) manipulation.
   * Make one toolbox using the makeShared method, and instantiate as many memory managers as you want, with identical
   *layouts!
   *
   * Unit test: ctest -R test-memorytoolbox
   **/
  template <size_t _NDim> class MemoryToolBox
  {
  public:
    // Put public methods here. These should change very little over time.

    static constexpr size_t NDim = _NDim;

    /** @brief Constructor with default MPI layout and MPI_COMM_WORLD. */
    MemoryToolBox(device::Idx nGridPoints, device::Idx ghostDepth, bool forbidTransposition = false)
        : MemoryToolBox(FFTMPIDomainSplit<NDim>::makeMPIGroup(NDim), nGridPoints, ghostDepth, forbidTransposition)
    {
    }
    /** @brief Shared-pointer constructor with default MPI layout and MPI_COMM_WORLD. */
    static inline device::memory::host_ptr<MemoryToolBox<NDim>>
    makeShared(device::Idx nGridPoints, device::Idx ghostDepth, bool forbidTransposition = false)
    {
      return device::memory::host_ptr<MemoryToolBox<NDim>>(nGridPoints, ghostDepth, forbidTransposition);
    }

    /** @brief Constructor with default MPI layout and MPI_COMM_WORLD. */
    MemoryToolBox(device::IdxArray<NDim> nGrid, device::Idx ghostDepth, bool forbidTransposition = false)
        : MemoryToolBox(FFTMPIDomainSplit<NDim>::makeMPIGroup(NDim), nGrid, ghostDepth, forbidTransposition)
    {
    }
    /** @brief Shared-pointer constructor with default MPI layout and MPI_COMM_WORLD. */
    static inline device::memory::host_ptr<MemoryToolBox<NDim>>
    makeShared(device::IdxArray<NDim> nGrid, device::Idx ghostDepth, bool forbidTransposition = false)
    {
      return device::memory::host_ptr<MemoryToolBox<NDim>>(nGrid, ghostDepth, forbidTransposition);
    }

    /** @brief Constructor with default MPI layout and MPI_COMM_WORLD but custom number of threads. */
    MemoryToolBox(device::Idx nGridPoints, device::Idx ghostDepth, device::Idx nThreads, bool forbidTransposition)
        : MemoryToolBox(FFTMPIDomainSplit<NDim>::makeMPIGroup(NDim), nGridPoints, ghostDepth, nThreads,
                        forbidTransposition)
    {
    }
    /** @brief Shared-pointer constructor with default MPI layout and MPI_COMM_WORLD. */
    static inline device::memory::host_ptr<MemoryToolBox<NDim>>
    makeShared(device::Idx nGridPoints, device::Idx ghostDepth, device::Idx nThreads, bool forbidTransposition = false)
    {
      return device::memory::host_ptr<MemoryToolBox<NDim>>(nGridPoints, ghostDepth, nThreads, forbidTransposition);
    }

    /** @brief Constructor with default MPI layout and MPI_COMM_WORLD but custom number of threads. */
    MemoryToolBox(device::IdxArray<NDim> nGrid, device::Idx ghostDepth, device::Idx nThreads, bool forbidTransposition)
        : MemoryToolBox(FFTMPIDomainSplit<NDim>::makeMPIGroup(NDim), nGrid, ghostDepth, nThreads, forbidTransposition)
    {
    }
    /** @brief Shared-pointer constructor with default MPI layout and MPI_COMM_WORLD. */
    static inline device::memory::host_ptr<MemoryToolBox<NDim>> makeShared(device::IdxArray<NDim> nGrid,
                                                                           device::Idx ghostDepth, device::Idx nThreads,
                                                                           bool forbidTransposition = false)
    {
      return device::memory::host_ptr<MemoryToolBox<NDim>>(nGrid, ghostDepth, nThreads, forbidTransposition);
    }

    /** @brief Constructor with default MPI layout and but custom MPI_Comm. */
    MemoryToolBox(MPICommReference comm, device::Idx nGridPoints, device::Idx ghostDepth,
                  bool forbidTransposition = false)
        : MemoryToolBox(FFTMPIDomainSplit<NDim>::makeMPIGroup(comm, NDim), nGridPoints, ghostDepth, forbidTransposition)
    {
    }
    /** @brief Shared-pointer constructor with default MPI layout and but custom MPI_Comm. */
    static inline device::memory::host_ptr<MemoryToolBox<NDim>>
    makeShared(MPICommReference comm, device::Idx nGridPoints, device::Idx ghostDepth, bool forbidTransposition = false)
    {
      return device::memory::host_ptr<MemoryToolBox<NDim>>(comm, nGridPoints, ghostDepth, forbidTransposition);
    }

    /** @brief Constructor with default MPI layout and but custom MPI_Comm. */
    MemoryToolBox(MPICommReference comm, device::IdxArray<NDim> nGrid, device::Idx ghostDepth,
                  bool forbidTransposition = false)
        : MemoryToolBox(FFTMPIDomainSplit<NDim>::makeMPIGroup(comm, NDim), nGrid, ghostDepth, forbidTransposition)
    {
    }
    /** @brief Shared-pointer constructor with default MPI layout and but custom MPI_Comm. */
    static inline device::memory::host_ptr<MemoryToolBox<NDim>> makeShared(MPICommReference comm,
                                                                           device::IdxArray<NDim> nGrid,
                                                                           device::Idx ghostDepth,
                                                                           bool forbidTransposition = false)
    {
      return device::memory::host_ptr<MemoryToolBox<NDim>>(comm, nGrid, ghostDepth, forbidTransposition);
    }

    /** @brief Constructor with user chosen MPI layout. */
    MemoryToolBox(MPICartesianGroup group, device::Idx nGridPoints, device::Idx ghostDepth,
                  bool forbidTransposition = false)
        : MemoryToolBox(group, nGridPoints, ghostDepth, 1, forbidTransposition)
    {
    }
    /** @brief Shared-pointer constructor with user chosen MPI layout. */
    static inline device::memory::host_ptr<MemoryToolBox<NDim>> makeShared(MPICartesianGroup group,
                                                                           device::Idx nGridPoints,
                                                                           device::Idx ghostDepth,
                                                                           bool forbidTransposition = false)
    {
      return device::memory::host_ptr<MemoryToolBox<NDim>>(group, nGridPoints, ghostDepth, 1, forbidTransposition);
    }

    /** @brief Constructor with user chosen MPI layout. */
    MemoryToolBox(MPICartesianGroup group, device::IdxArray<NDim> nGrid, device::Idx ghostDepth,
                  bool forbidTransposition = false)
        : MemoryToolBox(group, nGrid, ghostDepth, 1, forbidTransposition)
    {
    }
    /** @brief Shared-pointer constructor with user chosen MPI layout. */
    static inline device::memory::host_ptr<MemoryToolBox<NDim>> makeShared(MPICartesianGroup group,
                                                                           device::IdxArray<NDim> nGrid,
                                                                           device::Idx ghostDepth,
                                                                           bool forbidTransposition = false)
    {
      return device::memory::host_ptr<MemoryToolBox<NDim>>(group, nGrid, ghostDepth, 1, forbidTransposition);
    }

    /** @brief Constructor with user chosen MPI layout and number of threads. */
    MemoryToolBox(MPICartesianGroup group, device::Idx nGridPoints, device::Idx ghostDepth, device::Idx nThreads,
                  bool forbidTransposition = false)
        : MemoryToolBox(group, makeUniformArray<device::Idx, NDim>(nGridPoints), ghostDepth, nThreads,
                        forbidTransposition)
    {
    }

    MemoryToolBox(MPICartesianGroup group, device::IdxArray<NDim> nGrid, device::Idx ghostDepth, device::Idx nThreads,
                  bool forbidTransposition = false)
        : mGroup(group),
          // mNGridPoints(nGridPoints),
          mNGridPointsVec(nGrid), mGhostDepth(ghostDepth), mFFTLibrary(group, mNGridPointsVec, forbidTransposition),
          mLayouts(mFFTLibrary.getLayout(), mGhostDepth), mFFTNormalization(mLayouts),
          mGhostBuster_toFFTConfig(mLayouts.getConfigSpaceLayout(), mLayouts.getFFTConfigSpaceLayout()),
          mGhostBuster_toConfig(mLayouts.getFFTConfigSpaceLayout(), mLayouts.getConfigSpaceLayout()),
          mGhostUpdater(group, mLayouts.getConfigSpaceLayout())
    {
      checkParallelConsistency();
#ifndef CHECKBOUNDS
      unsetVerbose();
#endif
    }

    void checkParallelConsistency()
    {
      auto dec = mGroup.getDecomposition();
      bool check = true;
      std::stringstream tmp1, tmp2;

      for (size_t i = 0; i < dec.size(); ++i) {
        check = check && (mNGridPointsVec[i] % dec[i] == 0);
        tmp1 << mNGridPointsVec[i] << " ";
        tmp2 << dec[i] << " ";
      }
      if (!check) {

        throw(InconsistentDomainSplitting("Abort: The domain decomposition (" + tmp2.str() +
                                          ") is not consistent with the lattice (" + tmp1.str() +
                                          "). Every lattice dimension should be dividable by the corresponding number "
                                          "of processes specified in the decomposition. Two solutions: Either adjust "
                                          "the number of gridpoints to match the decomposition or vice versa."));
      } else {
        // sayMPI << "Using the following domain decomposition:(" + tmp2.str() +"), with nThreads = " <<
        // mCSpaceItThreaded.size();
        if (amIRoot()) sayMPI << "Using the following domain decomposition:(" + tmp2.str() + ").";
      }
    }

    void setVerbose() { verbosity.setAllOn(); }

    void unsetVerbose() { verbosity.setAllOff(); }

    void resetVerbose() { verbosity.setToDefault(); }

    int getMPIRank() { return mGroup.getBaseComm().rank(); }

    int getNProcesses() const
    {
      int res = 1;
      auto dec = mGroup.getDecomposition();
      for (size_t i = 0; i < dec.size(); ++i) {
        res *= dec[i];
      }
      return res;
    }

    auto getDecomposition() const { return mGroup.getDecomposition(); }

    bool amIRoot() { return mGroup.getBaseComm().rank() == 0; }

    template <typename T> bool initializeFFT() //(JBB, Sep 2023)
    {
      if (std::is_same<T, float>::value)
        mFFTLibrary.getPlans_float();
      else if (std::is_same<T, double>::value)
        mFFTLibrary.getPlans_double();
      return true;
    }

    device::Idx getNGhosts() const { return mGhostDepth; }

  public:
    MPICartesianGroup mGroup;
    device::IdxArray<NDim> mNGridPointsVec; // Lattice size in each dimension
    device::Idx mGhostDepth;
    FFTLibrarySelector<NDim> mFFTLibrary;
    TripleStateLayouts<NDim> mLayouts;
    FFTNormalization<NDim> mFFTNormalization;
    GhostBuster<NDim> mGhostBuster_toFFTConfig;
    GhostBuster<NDim> mGhostBuster_toConfig;
    GhostUpdater<NDim> mGhostUpdater;

    VerbosityLevels verbosity;
  };

} // namespace TempLat

#include "TempLat/lattice/memory/memorymanager.h"

#endif
