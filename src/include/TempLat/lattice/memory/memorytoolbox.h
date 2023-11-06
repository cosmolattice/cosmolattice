#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYTOOLBOX_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYTOOLBOX_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"

#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/fft/fftnormalization.h"
#include "TempLat/fft/fftmpidomainsplit.h"

#include "TempLat/lattice/memory/triplestatelayouts.h"
#include "TempLat/lattice/loopers/looper.h"
#include "TempLat/lattice/loopers/coordinates.h"

#include "TempLat/lattice/ghostcells/ghostbuster.h"
#include "TempLat/lattice/ghostcells/ghostupdater.h"

#include "TempLat/lattice/memory/verbositylevels.h"
#include "TempLat/lattice/algebra/operators/shiftedcoordinatesmanager.h"
#include <sstream>
#include "TempLat/lattice/loopers/npartiteloopers/npartitecoordinates.h"
#include "TempLat/lattice/loopers/npartiteloopers/npartitelooper.h"
//#include "TempLat/lattice/algebra/shift.h"

namespace TempLat {

    /** \brief A class which collects all the tool classes which deal with memory(-state) manipulation. Make one toolbox using the makeShared method, and instantiate as many memory managers as you want, with identical layouts!
     *
     * Unit test: make test-memorytoolbox
     **/

     MakeException(InconsistentDomainSplitting);

    class MemoryToolBox {
    public:
    /* Put public methods here. These should change very little over time. */

        /** \brief Constructor with default MPI layout and MPI_COMM_WORLD. */
        MemoryToolBox(ptrdiff_t nDimensions,
                      ptrdiff_t nGridPoints,
                      ptrdiff_t ghostDepth,
                      bool forbidTransposition = false
                      ) :
        MemoryToolBox(FFTMPIDomainSplit::makeMPIGroup(nDimensions), nDimensions, nGridPoints, ghostDepth,  forbidTransposition)
        {}
        /** \brief Shared-pointer constructor with default MPI layout and MPI_COMM_WORLD. */
        static inline std::shared_ptr<MemoryToolBox> makeShared(ptrdiff_t nDimensions,
                                                                ptrdiff_t nGridPoints,
                                                                ptrdiff_t ghostDepth,
                                                                bool forbidTransposition = false)
        {
            return std::make_shared<MemoryToolBox>(nDimensions, nGridPoints, ghostDepth,  forbidTransposition);
        }

        /** \brief Constructor with default MPI layout and MPI_COMM_WORLD. */
        MemoryToolBox(std::vector<ptrdiff_t> nGrid,
                      ptrdiff_t ghostDepth,
                      bool forbidTransposition = false
        ) :
                MemoryToolBox(FFTMPIDomainSplit::makeMPIGroup(nGrid.size()), nGrid, ghostDepth,  forbidTransposition)
        {}
        /** \brief Shared-pointer constructor with default MPI layout and MPI_COMM_WORLD. */
        static inline std::shared_ptr<MemoryToolBox> makeShared(std::vector<ptrdiff_t > nGrid,
                                                                ptrdiff_t ghostDepth,
                                                                bool forbidTransposition = false)
        {
            return std::make_shared<MemoryToolBox>(nGrid, ghostDepth,  forbidTransposition);
        }

        /** \brief Constructor with default MPI layout and MPI_COMM_WORLD but custom number of threads. */
        MemoryToolBox(ptrdiff_t nDimensions,
                      ptrdiff_t nGridPoints,
                      ptrdiff_t ghostDepth,
                      ptrdiff_t nThreads,
                      bool forbidTransposition
        ) :
                MemoryToolBox(FFTMPIDomainSplit::makeMPIGroup(nDimensions), nDimensions, nGridPoints, ghostDepth,  nThreads,forbidTransposition)
        {}
        /** \brief Shared-pointer constructor with default MPI layout and MPI_COMM_WORLD. */
        static inline std::shared_ptr<MemoryToolBox> makeShared(ptrdiff_t nDimensions,
                                                                ptrdiff_t nGridPoints,
                                                                ptrdiff_t ghostDepth,
                                                                ptrdiff_t nThreads,
                                                                bool forbidTransposition = false)
        {
            return std::make_shared<MemoryToolBox>(nDimensions, nGridPoints, ghostDepth, nThreads, forbidTransposition);
        }

        /** \brief Constructor with default MPI layout and MPI_COMM_WORLD but custom number of threads. */
        MemoryToolBox(std::vector<ptrdiff_t> nGrid,
                      ptrdiff_t ghostDepth,
                      ptrdiff_t nThreads,
                      bool forbidTransposition
        ) :
                MemoryToolBox(FFTMPIDomainSplit::makeMPIGroup(nGrid.size()), nGrid, ghostDepth,  nThreads,forbidTransposition)
        {}
        /** \brief Shared-pointer constructor with default MPI layout and MPI_COMM_WORLD. */
        static inline std::shared_ptr<MemoryToolBox> makeShared(std::vector<ptrdiff_t> nGrid,
                                                                ptrdiff_t ghostDepth,
                                                                ptrdiff_t nThreads,
                                                                bool forbidTransposition = false)
        {
            return std::make_shared<MemoryToolBox>(nGrid, ghostDepth, nThreads, forbidTransposition);
        }


        /** \brief Constructor with default MPI layout and but custom MPI_Comm. */
        MemoryToolBox(
                      MPICommReference comm,
                      ptrdiff_t nDimensions,
                      ptrdiff_t nGridPoints,
                      ptrdiff_t ghostDepth,
                      bool forbidTransposition = false
                      ) :
        MemoryToolBox(FFTMPIDomainSplit::makeMPIGroup(comm, nDimensions), nDimensions, nGridPoints, ghostDepth, forbidTransposition)
        {}
        /** \brief Shared-pointer constructor with default MPI layout and but custom MPI_Comm. */
        static inline std::shared_ptr<MemoryToolBox> makeShared(
                                                                MPICommReference comm,
                                                                ptrdiff_t nDimensions,
                                                                ptrdiff_t nGridPoints,
                                                                ptrdiff_t ghostDepth,
                                                                bool forbidTransposition = false)
        {
            return std::make_shared<MemoryToolBox>(comm, nDimensions, nGridPoints, ghostDepth, forbidTransposition);
        }


        /** \brief Constructor with default MPI layout and but custom MPI_Comm. */
        MemoryToolBox(
                MPICommReference comm,
                std::vector<ptrdiff_t> nGrid,
                ptrdiff_t ghostDepth,
                bool forbidTransposition = false
        ) :
                MemoryToolBox(FFTMPIDomainSplit::makeMPIGroup(comm, nGrid.size()), nGrid, ghostDepth, forbidTransposition)
        {}
        /** \brief Shared-pointer constructor with default MPI layout and but custom MPI_Comm. */
        static inline std::shared_ptr<MemoryToolBox> makeShared(
                MPICommReference comm,
                std::vector<ptrdiff_t> nGrid,
                ptrdiff_t ghostDepth,
                bool forbidTransposition = false)
        {
            return std::make_shared<MemoryToolBox>(comm, nGrid, ghostDepth, forbidTransposition);
        }





        /** \brief Constructor with user chosen MPI layout. */
        MemoryToolBox(
                      MPICartesianGroup group,
                      ptrdiff_t nDimensions,
                      ptrdiff_t nGridPoints,
                      ptrdiff_t ghostDepth,
                      bool forbidTransposition = false
                      ) : MemoryToolBox(group,nDimensions,nGridPoints,ghostDepth, 1, forbidTransposition)
        {

        }
        /** \brief Shared-pointer constructor with user chosen MPI layout. */
        static inline std::shared_ptr<MemoryToolBox> makeShared(MPICartesianGroup group,
                                                                ptrdiff_t nDimensions,
                                                                ptrdiff_t nGridPoints,
                                                                ptrdiff_t ghostDepth,
                                                                bool forbidTransposition = false
                                                                )
        {
            return std::make_shared<MemoryToolBox>(group, nDimensions, nGridPoints, ghostDepth, 1, forbidTransposition);
        }


        /** \brief Constructor with user chosen MPI layout. */
        MemoryToolBox(
                MPICartesianGroup group,
                std::vector<ptrdiff_t> nGrid,
                ptrdiff_t ghostDepth,
                bool forbidTransposition = false
        ) : MemoryToolBox(group,nGrid,ghostDepth, 1, forbidTransposition)
        {

        }
        /** \brief Shared-pointer constructor with user chosen MPI layout. */
        static inline std::shared_ptr<MemoryToolBox> makeShared(MPICartesianGroup group,
                                                                std::vector<ptrdiff_t> nGrid,
                                                                ptrdiff_t ghostDepth,
                                                                bool forbidTransposition = false
        )
        {
            return std::make_shared<MemoryToolBox>(group, nGrid, ghostDepth,  1, forbidTransposition);
        }


        /** \brief Constructor with user chosen MPI layout and number of threads. */
        MemoryToolBox(
                MPICartesianGroup group,
                ptrdiff_t nDimensions,
                ptrdiff_t nGridPoints,
                ptrdiff_t ghostDepth,
                ptrdiff_t nThreads,
                bool forbidTransposition = false) : MemoryToolBox(group,std::vector<ptrdiff_t >(nDimensions,nGridPoints),ghostDepth,nThreads,forbidTransposition)
        {

        }


        MemoryToolBox(
                MPICartesianGroup group,
                std::vector<ptrdiff_t > nGrid,
                ptrdiff_t ghostDepth,
                ptrdiff_t nThreads,
                bool forbidTransposition = false) :
                mGroup(group),
                mNDimensions(nGrid.size()),
                // mNGridPoints(nGridPoints),
                mNGridPointsVec(nGrid),
                mGhostDepth(ghostDepth),
                mFFTLibrary(group, mNGridPointsVec, forbidTransposition),
                mLayouts( mFFTLibrary.getLayout(), mGhostDepth),
                mFFTNormalization(mLayouts),
                mGhostBuster_toFFTConfig(mLayouts.getConfigSpaceJumps(), mLayouts.getFFTConfigSpaceJumps()),
                mGhostBuster_toConfig(mLayouts.getFFTConfigSpaceJumps(), mLayouts.getConfigSpaceJumps()),
                mGhostUpdater(group, mLayouts.getConfigSpaceJumps()),
                mConfCoord(std::make_shared<Coordinates>(mLayouts.getConfigSpaceJumps(), mLayouts.getConfigSpaceLayout())),
                mFourierCoord(std::make_shared<Coordinates>( mLayouts.getFourierSpaceJumps(), mLayouts.getFourierSpaceLayout())),
                mCSpaceIt(mConfCoord),
                mFSpaceIt(mFourierCoord)//,
        //   mCSpaceItThreaded(mConfCoord,nThreads)
        {
            checkParallelConsistency();
            initNNNeighbours();
#ifndef TEMPLATTEST
            unsetVerbose();
#endif
        }


      	~MemoryToolBox()
      	{
      	}

        void checkParallelConsistency()
        {
          auto dec = mGroup.getDecomposition();
          bool check = true;
          std::stringstream tmp1, tmp2;

          for(size_t i = 0; i < dec.size(); ++i){
            check = check && (mNGridPointsVec[i]%dec[i] == 0);
            tmp1 << mNGridPointsVec[i] << " ";
            tmp2 << dec[i] << " ";
          }
          if(!check){

            throw(InconsistentDomainSplitting( "Abort: The domain decomposition (" + tmp2.str() +") is not consistent with the lattice ("+ tmp1.str() +"). Every lattice dimension should be dividable by the corresponding number of processes specified in the decomposition. Two solutions: Either adjust the number of gridpoints to match the decomposition or vice versa."));
          }
          else {
             // sayMPI << "Using the following domain decomposition:(" + tmp2.str() +"), with nThreads = " << mCSpaceItThreaded.size();
              if(amIRoot()) sayMPI << "Using the following domain decomposition:(" + tmp2.str() +").";
          }
        }

        void setVerbose() {
            verbosity.setAllOn();
        }

        void unsetVerbose() {
            verbosity.setAllOff();
        }

        void resetVerbose() {
            verbosity.setToDefault();
        }

        int getMPIRank() {
            return mGroup.getBaseComm().rank();
        }

        int getNProcesses() const {
            int res = 1;
            auto dec = mGroup.getDecomposition();
            for(size_t i = 0; i < dec.size(); ++i){
                res*= dec[i];
            }
            return res;
        }

        auto getDecomposition() const {
            return mGroup.getDecomposition();
        }

        bool amIRoot()
        {
          return mGroup.getBaseComm().rank() == 0;
        }

        /* exceptionally, let everyone access private members of MemoryToolBox. */


      Looper& itX()
      {
        return mCSpaceIt;
      }

      Looper& itP()
      {
        return mFSpaceIt;
      }

        NPartiteLooper& itXNPart()
        {
            return mCSpaceItNPartite;
        }

        NPartiteLooper& itPNPart()
        {
            return mFSpaceItNPartite;
        }

      std::vector<ptrdiff_t> getCoordConfiguration(ptrdiff_t i)
      {
        return mConfCoord->getVec(i);
      }
      std::vector<ptrdiff_t> getCoordFourier(ptrdiff_t i)
      {
        return mFourierCoord->getVec(i);
      }

      std::vector<size_t> getCoordConfiguration0N(ptrdiff_t i) //Brings back the coordinates between 0 and N-1. Useful for saving and loading for example
      {
        auto tmp = getCoordConfiguration(i);
        std::vector<size_t> res;

        for(size_t j = 0; j < tmp.size(); ++j)
          res.push_back(tmp[j] + mNGridPointsVec[j] / 2 - 1);
        return res;
      }
      std::vector<size_t> getCoordFourier0N(ptrdiff_t i)
      {
        auto tmp = getCoordFourier(i);
        std::vector<size_t> res;
        for(size_t j = 0; j < tmp.size(); ++j)
          res.push_back(tmp[j] + mNGridPointsVec[j] / 2 - 1);
        return res;
      }



      ptrdiff_t getCoordinatesShiftByOne(ptrdiff_t i)
      {
          return i > 0 ? (*mCoordinatesNNNeighbourShifts)[i-1] : (*mCoordinatesNNNeighbourShifts)[-i-1+mNDimensions];
      }


    ptrdiff_t computeCoordinatesShifts(const std::vector<ptrdiff_t>& shifts)
    {
        ShiftedCoordinatesManager mShifts(shifts);

        mShifts.setJumps(mLayouts.getConfigSpaceJumps());
        return mShifts.memoryJump();

    }

    void addNPartiteIteration(int parity)
    {
        if(!mConfCoordNPartite)
        {
            mConfCoordNPartite =  std::make_shared<NPartiteCoordinates>(parity, mLayouts.getConfigSpaceJumps(), mLayouts.getConfigSpaceLayout());
            mCSpaceItNPartite.init(mConfCoordNPartite);
        }
        if(!mFourierCoordNPartite)
        {
            mFourierCoordNPartite =  std::make_shared<NPartiteCoordinates>(parity, mLayouts.getFourierSpaceJumps(), mLayouts.getFourierSpaceLayout());
            mFSpaceItNPartite.init(mConfCoordNPartite);
        }
    }

    template<typename T>
    bool initializeFFT() //(JBB, Sep 2023)
    {
        if (std::is_same<T, float>::value) mFFTLibrary.getPlans_float();
        else if (std::is_same<T, double>::value) mFFTLibrary.getPlans_double();
        return true;
    }
//    private:
    public:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        MPICartesianGroup mGroup;
        ptrdiff_t mNDimensions;
       // ptrdiff_t mNGridPoints;
        std::vector<ptrdiff_t > mNGridPointsVec;
        ptrdiff_t mGhostDepth;
        FFTLibrarySelector mFFTLibrary;
        TripleStateLayouts mLayouts;
        FFTNormalization mFFTNormalization;
        GhostBuster mGhostBuster_toFFTConfig;
        GhostBuster mGhostBuster_toConfig;
        GhostUpdater mGhostUpdater;


        std::shared_ptr<Coordinates> mConfCoord;
        std::shared_ptr<Coordinates> mFourierCoord;

        std::shared_ptr<NPartiteCoordinates> mConfCoordNPartite;
        std::shared_ptr<NPartiteCoordinates> mFourierCoordNPartite;

        std::shared_ptr<std::vector<ptrdiff_t >> mCoordinatesNNNeighbourShifts;

        Looper mCSpaceIt;
        Looper mFSpaceIt;

        NPartiteLooper mCSpaceItNPartite;
        NPartiteLooper mFSpaceItNPartite;
      //  ThreadedLooper mCSpaceItThreaded;

        VerbosityLevels verbosity;

    private:
        void initNNNeighbours()
        {
            mCoordinatesNNNeighbourShifts = std::make_shared<std::vector<ptrdiff_t > >(2 * mNDimensions);
            for(ptrdiff_t i = 0; i < mNDimensions; ++i)
            {
                std::vector<ptrdiff_t> shifts(mNDimensions, (ptrdiff_t) 0);
                shifts[i] = 1;
                (*mCoordinatesNNNeighbourShifts)[i] = computeCoordinatesShifts(shifts);
                shifts[i] = -1;
                (*mCoordinatesNNNeighbourShifts)[i+mNDimensions] = computeCoordinatesShifts(shifts);
            }
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorytoolbox_test.h"
#endif
#include "TempLat/lattice/memory/memorymanager.h"

#endif
