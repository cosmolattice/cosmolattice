#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cmath>
#include <cstring>
#include <vector>

#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/util/exception.h"
#include "TempLat/util/log/log.h"

#include "TempLat/fft/types/fftmallocfree.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  MakeException(MemoryBlockOutOfBoundsException);

#ifndef NOKOKKOS

  /** \brief A class which holds a block of memory.
   * Feel free to pass it around and copy: the pointer
   * is itself kept inside a shared_ptr. Only when the
   * last owner is destructed, the memory is freed.
   *
   * Templated for double / float.
   *
   * Unit test: make test-memoryblock
   **/
  template <typename T> class MemoryBlock
  {
  public:
    /* Put public methods here. These should change very little over time. */

    /** \brief Default constructor: empty. */
    MemoryBlock() : mSize(0u) {}

    /** \brief Constructor with a size to allocate. */
    MemoryBlock(size_t size) : mSize(size), mData("FieldMemoryBlock", size) { zero(); }

    /** getter */
    size_t size() const { return mSize; }

    /** \brief state modify: zero out */
    void zero()
    {
      std::cout << "zero=" << mSize << std::endl;
      if (size()) Kokkos::parallel_for(size(), KOKKOS_CLASS_LAMBDA(size_t i) { mData(i) = 0; });
      std::cout << "zero=" << mSize << std::endl;
    }

    /** \brief access */
    KOKKOS_FORCEINLINE_FUNCTION
    T &operator[](ptrdiff_t i) const
    {
      checkBounds(i);
      return mData(i);
    }

    auto getView() const { return mData; }

    auto getHostView() const
    {
      if (!mHostMirror.is_allocated()) mHostMirror = Kokkos::create_mirror_view(mData);
      Kokkos::deep_copy(mHostMirror, mData);
      return mHostMirror;
    }

    KOKKOS_FORCEINLINE_FUNCTION
    T *ptr() const { return mData.data(); }

    KOKKOS_FORCEINLINE_FUNCTION
    operator T *() const { return mData.data(); }

    KOKKOS_FORCEINLINE_FUNCTION
    T *data() const { return mData.data(); }

    MemoryBlock duplicate()
    {
      MemoryBlock newblock(mSize);
      Kokkos::deep_copy(newblock.getView(), mData);
      return newblock;
    }

    friend std::ostream &operator<<(std::ostream &ostream, const MemoryBlock &mb)
    {
      ostream << "Memory Block. Size: " << mb.mSize << ", allocated: " << mb.mData.is_allocated() << "\n";
      return ostream;
    }

    friend bool operator==(const MemoryBlock &a, const MemoryBlock &b) { return a.getView() == b.getView(); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    size_t mSize;
    Kokkos::View<T *, Kokkos::DefaultExecutionSpace> mData;
    mutable typename Kokkos::View<T *, Kokkos::DefaultExecutionSpace>::host_mirror_type mHostMirror;
    static constexpr size_t TSIZE = sizeof(T);

    KOKKOS_FORCEINLINE_FUNCTION
    void checkBounds(ptrdiff_t i) const
    {
#ifdef CHECKBOUNDS
#ifdef NOKOKKOS
      if (i < 0 || i >= (ptrdiff_t)mSize)
        throw MemoryBlockOutOfBoundsException("Accessing memory block out of bounds:", i, "not in 0 -- ", mSize);
#endif
#endif
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

#else

  template <typename T> class MemoryBlock
  {
  public:
    /* Put public methods here. These should change very little over time. */
    /** \brief Default constructor: empty. */
    MemoryBlock() : mSize(0u), thePointer() /* defaults to thePointer.get() == NULL */ {}

    /** \brief Constructor with a size to allocate. */
    MemoryBlock(size_t size)
        : mSize(size), thePointer(std::make_shared<T *>((T *)FFTMallocFree<T>::malloc(size * TSIZE)))
    {
      zero();
    }

    /** destructor */
    ~MemoryBlock()
    {
      if (thePointer.use_count() < 2 && thePointer.get() != NULL) {
        FFTMallocFree<T>::free(*thePointer);
      }
    }

    /** getter */
    size_t size() const { return mSize; }

    /** \brief state modify: zero out */
    void zero()
    {
      if (size()) std::memset(*thePointer, 0, size() * TSIZE);
    }

    /** \brief access: avoid using these for manual iteration */
    inline operator T *() { return *thePointer; }

    /** \brief access: avoid using these for manual iteration */
    inline operator const T *() const { return *thePointer; }

    /** \brief access: avoid using these for iteration */
    inline T *ptr() { return *thePointer; }

    /** \brief access: avoid using these for iteration */
    inline T *data() { return *thePointer; }

    /** \brief access: avoid using these for iteration */
    inline const T *data() const { return *thePointer; }

    /** \brief access */
    inline const T &operator[](ptrdiff_t i) const
    {
      checkBounds(i);
      return (*thePointer)[i];
    }

    /** \brief access */
    inline T &operator[](ptrdiff_t i)
    {
      checkBounds(i);
      return (*thePointer)[i];
    }

    inline MemoryBlock duplicate()
    {
      MemoryBlock newblock(mSize);
      std::memmove(newblock.ptr(), ptr(), mSize * TSIZE);
    }

    friend std::ostream &operator<<(std::ostream &ostream, const MemoryBlock &mb)
    {
      ostream << "Memory Block. Size: " << mb.mSize << ", allocated: " << *(mb.thePointer) << "\n";
      return ostream;
    }

    friend bool operator==(const MemoryBlock &a, const MemoryBlock &b) { return a.ptr() == b.ptr(); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    size_t mSize;
    std::shared_ptr<T *> thePointer;
    static constexpr size_t TSIZE = sizeof(T);

    inline void checkBounds(ptrdiff_t i) const
    {
#ifdef CHECKBOUNDS
      if (i < 0 || i >= (ptrdiff_t)mSize)
        throw MemoryBlockOutOfBoundsException("Accessing memory block out of bounds:", i, "not in 0 -- ", mSize);
#endif
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
#endif
} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memoryblock_test.h"
#endif

#endif
