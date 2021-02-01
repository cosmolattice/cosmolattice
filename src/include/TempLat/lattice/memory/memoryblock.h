#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cmath>
#include <cstring>
#include <vector>

#include "TempLat/util/log/log.h"
#include "TempLat/util/exception.h"


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/types/fftmallocfree.h"
namespace TempLat {

    MakeException(MemoryBlockOutOfBoundsException);

    /** \brief A class which holds a block of memory.
     * Feel free to pass it around and copy: the pointer
     * is itself kept inside a shared_ptr. Only when the
     * last owner is destructed, the memory is freed.
     *
     * Templated for double / float.
     *
     * Unit test: make test-memoryblock
     **/
    template <typename T>
    class MemoryBlock {
    public:
    /* Put public methods here. These should change very little over time. */
        /** \brief Default constructor: empty. */
        MemoryBlock() :
        mSize(0u),
        thePointer() /* defaults to thePointer.get() == NULL */
        {
        }

        /** \brief Constructor with a size to allocate. */
        MemoryBlock(size_t size) :
        mSize(size),
        thePointer(
                   std::make_shared<T*>(
                                        (T*) FFTMallocFree<T>::malloc(size  * TSIZE)
                                        )
                   )
        {
            zero();
        }

        /** destructor */
        ~MemoryBlock() {
            if ( thePointer.use_count() < 2 && thePointer.get() != NULL ) {
                FFTMallocFree<T>::free(*thePointer);
            }
        }

        /** getter */
        size_t size() const {
            return mSize;
        }

        /** \brief state modify: zero out */
        void zero() {
            if ( size() ) std::memset(*thePointer, 0, size() * TSIZE);
        }

        /** \brief access: avoid using these for manual iteration */
        inline operator T*() { return *thePointer; }

        /** \brief access: avoid using these for manual iteration */
        inline operator const T*() const { return *thePointer; }

        /** \brief access: avoid using these for iteration */
        inline T* ptr() { return *thePointer; }

        /** \brief access: avoid using these for iteration */
        inline T* data() { return *thePointer; }

        /** \brief access: avoid using these for iteration */
        inline const T* data() const { return *thePointer; }

        /** \brief access */
        inline const T& operator[](ptrdiff_t i) const { checkBounds(i); return (*thePointer)[i]; }

        /** \brief access */
        inline T& operator[](ptrdiff_t i) {  checkBounds(i); return (*thePointer)[i]; }

        inline MemoryBlock duplicate() {
            MemoryBlock newblock(mSize);
            std::memmove(newblock.ptr(), ptr(), mSize * TSIZE);
        }

        friend
        std::ostream& operator<< (std::ostream& ostream, const MemoryBlock& mb) {
            ostream << "Memory Block. Size: " << mb.mSize << ", allocated: " << *(mb.thePointer) << "\n";
            return ostream;
        }

        friend
        bool operator==( const MemoryBlock &a, const MemoryBlock &b) {
            return a.ptr() == b.ptr();
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        size_t mSize;
        std::shared_ptr<T*> thePointer;
        static constexpr size_t TSIZE = sizeof(T);

        inline void checkBounds(ptrdiff_t i ) {
#ifdef CHECKBOUNDS
            if( i < 0 || i >= (ptrdiff_t) mSize ) throw MemoryBlockOutOfBoundsException("Accessing memory block out of bounds:"
            , i, "not in 0 -- ", mSize);
#endif
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memoryblock_test.h"
#endif


#endif
