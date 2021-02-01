#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYMANAGER_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYMANAGER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <complex>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/memory/memoryblock.h"
#include "TempLat/lattice/memory/memorylayoutstate.h"
#include "TempLat/lattice/ghostcells/ghoststatekeeper.h"

namespace TempLat {

    MakeException(MemoryManagerAccessOutOfBounds);

    /** \brief A class which holds a single lattice in memory, and tracks and moves between various ghost states. Templated for the memory type, typically float or double.
     *  All the confirm___Space() functions return a ptrdiff_t, which counts the amount of work done. Ignore that, but it is important for testing purposes.
     *  NOTE that the memory access operators (T* and operator[]) do NOT verify if memory was allocated.
     *
     * Unit test: make test-memorymanager
     **/
    template <typename T>
    class MemoryManager {
    public:
    /* Put public methods here. These should change very little over time. */
        MemoryManager(std::shared_ptr<MemoryToolBox> toolBox, std::string name = "") :
        mToolBox(toolBox),
        mName(name),
        mAllocated(false)
        {}

        ptrdiff_t allocate() {
            if ( mAllocated ) return 0;
            if ( mToolBox->verbosity.allocation ) sayMPI << "Allocating memory.\n";
            mAllocated = true;
            mBlock = MemoryBlock<T>(mToolBox->mLayouts.getNecessaryMemoryAllocation());
            return 1;
        }

        // disabled these: only access through operator[], which optionally compiles with bounds checking.
//        inline
//        operator T*() { return mBlock; }
//
//        inline
//        operator const T*() const { return mBlock; }

        inline
        T& operator [](ptrdiff_t i) {
#ifdef CHECKBOUNDS
            checkRealBounds(i);
#endif
            return mBlock[i];
        }


        inline
        T operator [](ptrdiff_t i) const {
#ifdef CHECKBOUNDS
            checkRealBounds(i);
#endif
            return mBlock[i];
        }

  /*      inline
        const T& operator [](ptrdiff_t i) const {
#ifdef CHECKBOUNDS
            checkRealBounds(i);
#endif
            return reinterpret_cast<std::complex<T>*>((T*) mBlock)[i];
        }
        */

        inline
        std::complex<T>& complex(ptrdiff_t i) {
#ifdef CHECKBOUNDS
            checkComplexBounds(i);
#endif
            /* https://stackoverflow.com/a/41654651/2295722 : cast double to complex is part of the standard since C++11. */
            return reinterpret_cast<std::complex<T>*>((T*) mBlock)[i];
        }

        inline
        const std::complex<T>& complex(ptrdiff_t i) const {
#ifdef CHECKBOUNDS
            checkComplexBounds(i);
#endif
            return reinterpret_cast<std::complex<T>*>((T*) mBlock)[i];;
        }

        ptrdiff_t confirmConfigSpace() {
            ptrdiff_t result = allocate();
            if ( mToolBox->verbosity.spaceConfirmation ) sayMPI << "Confirming that we are in configuration space. " << getName() << "\n";
            if ( ! mLayoutState.isConfigSpace() ) {
                if ( mLayoutState.isFourierSpace() ) {
                    if ( mToolBox->verbosity.spaceConfirmation ) sayMPI << "Need FFT C2R.\n";
//say << "Setting fft library verbose.\n";
//mToolBox->mFFTLibrary.setVerbose();
                    /* do an fft */
                    ++result;
                    if ( mToolBox->verbosity.fftPerformance ) say << "FFT: " << mName << "(k) -> " << mName << "(x)\n";
                    mToolBox->mFFTLibrary.c2r(mBlock);
                    /* normalize after FFT */
                    ++result;
                    mToolBox->mFFTNormalization.c2r(mBlock, (T) 1.);
                    mLayoutState.setToFFTConfigSpace();
                    if ( mToolBox->verbosity.spaceConfirmation ) sayMPI << "Performed FFT C2R.\n";
                }
                if ( mLayoutState.isFFTConfigSpace() ) {
                    if ( mToolBox->verbosity.spaceConfirmation ) sayMPI << "Need ghost buster from fft config to plain config space.\n";
                    ++result;
                    mToolBox->mGhostBuster_toConfig(mBlock);
                }
                if ( mToolBox->verbosity.spaceConfirmation ) sayMPI << "Setting ghost state to stale, because of the FFT we performed. " << getName() << "\n";
                mGhostStateKeeper.setStale();
            }
            mLayoutState.setToConfigSpace();
            if ( mToolBox->verbosity.spaceConfirmation ) sayMPI << "We are in configuration space.\n";
            return result;
        }

        ptrdiff_t confirmFourierSpace() {
            ptrdiff_t result = allocate();
            if ( mToolBox->verbosity.spaceConfirmation ) sayMPI << "Confirming that we are in Fourier space. " << getName() << "\n";
            if ( ! mLayoutState.isFourierSpace() ) {
                if ( mLayoutState.isConfigSpace() ) {
                    if ( mToolBox->verbosity.spaceConfirmation ) sayMPI << "Need ghost buster from plain config to fft config space.\n";
                    ++result;
                    mToolBox->mGhostBuster_toFFTConfig(mBlock);
                    mLayoutState.setToFFTConfigSpace();
                }
                if ( mLayoutState.isFFTConfigSpace() ) {
                    if ( mToolBox->verbosity.spaceConfirmation ) sayMPI << "Need FFT R2C.\n";
                    ++result;
                    if ( mToolBox->verbosity.fftPerformance ) say << "FFT: " <<  mName << "(x) -> " << mName << "(k)\n";
                    /* do an fft */
                    mToolBox->mFFTLibrary.r2c(mBlock);
                    /* normalize after FFT */
                    ++result;
                    mToolBox->mFFTNormalization.r2c(mBlock, (T) 1.);
                    if ( mToolBox->verbosity.spaceConfirmation ) sayMPI << "Performed FFT R2C.\n";
                }
            }
            mLayoutState.setToFourierSpace();
            if ( mToolBox->verbosity.spaceConfirmation ) sayMPI << "We are in Fourier space.\n";
            return result;
        }

        ptrdiff_t confirmGhostsUpToDate() {
            ptrdiff_t result = confirmConfigSpace();
            if ( mToolBox->verbosity.ghostConfirmationSteps ) sayMPI << "Confirming that ghost cells are up to date. " << getName() << "\n" << mGhostStateKeeper << "\n";
            if ( mGhostStateKeeper.isStale() ) {
                if ( mToolBox->verbosity.ghostConfirmationSteps ) sayMPI << "Need to update ghost cells.\n";
                ++result;
                mToolBox->mGhostUpdater.update(mBlock.ptr());
                mGhostStateKeeper.setUpToDate();
            }
            if ( mToolBox->verbosity.ghostConfirmationSteps ) sayMPI << "Ghost cells are up to date.\n" << mGhostStateKeeper << "\n";
            return result;
        }

        void updateGhosts(){
            mToolBox->mGhostUpdater.update(mBlock.ptr());
        }

        /** \brief this is the only state the one may need to set from the outside: if a field is updated in the integrator. */
        void setGhostsAreStale() {
            if ( mToolBox->verbosity.ghostConfirmationSteps ) sayMPI << "Noting that ghost cells are no longer up to date. " << getName() << "\n";
            mGhostStateKeeper.setStale();
        }

        bool areGhostsStale() const {
            return mGhostStateKeeper.isStale();
        }

        /** \brief Check the current state. */
        bool isConfigSpace() const {
            return mLayoutState.isConfigSpace();
        }

        /** \brief Check the current state. */
        bool isFourierSpace() const {
            return mLayoutState.isFourierSpace();
        }

        std::string getName() const {
            return mName;
        }

        void setName(std::string newName) {
            mName = newName;
        }

        friend
        std::ostream& operator<< ( std::ostream& ostream, const MemoryManager & mMan ) {
            ostream << "Memory manager -\n " << mMan.mToolBox->mGroup << "\n" << mMan.mToolBox->mLayouts << "\n\n" << mMan.mLayoutState << "\n\n" << mMan.mGhostStateKeeper;
            return ostream;
        }

        friend
        bool operator==(const MemoryManager& a, const MemoryManager& b) {
            return a.mBlock == b.mBlock;
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        std::shared_ptr<MemoryToolBox> mToolBox;
        std::string mName;
        bool mAllocated ;
        MemoryBlock<T> mBlock;

        MemoryLayoutState mLayoutState;
        GhostStateKeeper mGhostStateKeeper;

        void checkRealBounds(ptrdiff_t i) {
            if ( i < 0 || i >= (ptrdiff_t) mBlock.size() ) {throw MemoryManagerAccessOutOfBounds("Accessing memory out of bounds ", getName(), ", mBlock.size(): ", mBlock.size(), "index:", i);}
        }

        void checkComplexBounds(ptrdiff_t i) {
            if ( i < 0 || 2 * i >= (ptrdiff_t) mBlock.size() ) {throw MemoryManagerAccessOutOfBounds("Accessing memory out of bounds ", getName(), ", mBlock.size(): ", mBlock.size(), "index:", i);}
            /* also check that the casting works out correctly */
            void *ptr1 = &(reinterpret_cast<std::complex<T>*>((T*) mBlock)[i]);
            void *ptr2 = &(mBlock[2 * i]);
            if ( ptr1 != ptr2 ) throw MemoryManagerAccessOutOfBounds("pointer casting from double to complex failed:", ptr1, "!=", ptr2);
        }


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorymanager_test.h"
#endif
#ifdef TEMPLATTEST
//#include "TempLat/lattice/loopers/genericlooper_test.h"
#endif


#endif
