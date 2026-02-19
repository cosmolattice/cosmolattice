#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/util/exception.h"
#include "TempLat/util/log/log.h"

#include "TempLat/parallel/device_memory.h"

namespace TempLat
{
  MakeException(MemoryBlockOutOfBoundsException);

  /** @brief A class which holds a block of memory.
   * Feel free to pass it around and copy: the pointer
   * is itself kept inside a shared pointer. Only when the
   * last owner is destructed, the memory is freed.
   *
   * Templated for double / float.
   *
   * Unit test: ctest -R test-memoryblock
   **/
  template <size_t NDim, typename T> class MemoryBlock
  {
  public:
    // Put public methods here. These should change very little over time.

    /** @brief Default constructor: empty. */
    MemoryBlock() : mSize(0u), mHostMirrorOutdated(true) {}

    /** @brief Constructor with a size to allocate. */
    MemoryBlock(size_t size) : mSize(size), mHostMirrorOutdated(true)
    {
      mData = device::memory::NDView<1, T>("MemoryBlock", mSize);
      zero();
    }

    /** getter */
    size_t size() const { return mSize; }

    /** @brief state modify: zero out */
    void zero() { fill(T{0}); }

    void fill(const T &value)
    {
      device::memory::fill(mData, value);
      mHostMirrorOutdated = true; // mark host mirror as outdated
    }

    /** @brief access */
    DEVICE_FORCEINLINE_FUNCTION
    T &operator[](ptrdiff_t i) const
    {
      checkBounds(i);
      return mData(i);
    }

    template <typename R = T> auto getNDView(const device::IdxArray<NDim> &localSizes) const
    {
#ifdef CHECKBOUNDS
      size_t total_size = 1;
      for (size_t i = 0; i < NDim; ++i)
        total_size *= localSizes[i];
      if (total_size > mSize)
        throw MemoryBlockOutOfBoundsException("Accessing memory block out of bounds: total size ", total_size,
                                              " is larger than allocated size ", mSize);
#endif

      return device::apply(
          [&](auto &&...args) {
            return device::memory::NDViewUnmanaged<NDim, R>(reinterpret_cast<R *>(mData.data()), args...);
          },
          localSizes);
    }

    template <typename R = T> auto getNDHostView(const device::IdxArray<NDim> &localSizes) const
    {
#ifdef CHECKBOUNDS
      size_t total_size = 1;
      for (size_t i = 0; i < NDim; ++i)
        total_size *= localSizes[i];
      if (total_size > mSize)
        throw MemoryBlockOutOfBoundsException("Accessing memory block out of bounds: total size ", total_size,
                                              " is larger than allocated size ", mSize);
#endif
      pullHostView(); // ensure host mirror is up to date

      return device::apply(
          [&](auto &&...args) {
            return device::memory::NDViewUnmanagedHost<NDim, R>(reinterpret_cast<R *>(mHostMirror.data()), args...);
          },
          localSizes);
    }

    void flagHostMirrorOutdated() const { mHostMirrorOutdated = true; }

    void pullHostView() const
    {
      if (mHostMirrorOutdated) {
        if (!mHostMirror.is_allocated()) mHostMirror = device::memory::createMirrorView(mData);
        device::memory::copyDeviceToHost(mData, mHostMirror.data());
      }
      mHostMirrorOutdated = false;
    }

    void pushHostView()
    {
      if (!mHostMirror.is_allocated())
        throw MemoryBlockOutOfBoundsException(
            "Cannot push host view: host mirror is not allocated. Call getRawHostView() or getNDHostView() first.");
      device::memory::copyHostToDevice(mHostMirror.data(), mData);
      mHostMirrorOutdated = false;
    }

    void deallocateHostView()
    {
      mHostMirror = HostView();
      mHostMirrorOutdated = true; // mark as outdated
    }

    bool isHostViewAllocated() const { return mHostMirror.is_allocated(); }

    template <typename R = T> auto getRawView() const
    {
      if constexpr (std::is_same_v<R, T>)
        return mData;
      else {
        const size_t size = mSize * sizeof(T) / sizeof(R);
        return device::memory::NDViewUnmanaged<1, R>(reinterpret_cast<R *>(mData.data()), size);
      }
    }

    template <typename R = T> auto getRawHostView() const
    {
      pullHostView(); // ensure host mirror is up to date
      if constexpr (std::is_same_v<R, T>)
        return mHostMirror;
      else {
        const size_t size = mSize * sizeof(T) / sizeof(R);
        return device::memory::NDViewUnmanagedHost<1, R>(reinterpret_cast<R *>(mHostMirror.data()), size);
      }
    }

    DEVICE_FORCEINLINE_FUNCTION
    operator T *() const { return mData.data(); }

    DEVICE_FORCEINLINE_FUNCTION
    T *data() const { return mData.data(); }

    MemoryBlock duplicate()
    {
      MemoryBlock newblock(mSize);
      device::memory::copyDeviceToDevice(mData, newblock.getRawView());
      return newblock;
    }

    friend std::ostream &operator<<(std::ostream &ostream, const MemoryBlock &mb)
    {
      ostream << "Memory Block. Size: " << mb.mSize << ", allocated: " << mb.mData.is_allocated() << "\n";
      if (mb.mSize < 64)
        ostream << "Data: \n";
      else
        ostream << "Data (first 64 elements): \n";
      auto data = mb.getRawHostView();
      mb.flagHostMirrorOutdated();
      for (size_t i = 0; i < std::min((size_t)64, mb.mSize); ++i) {
        if (i > 0) ostream << ", ";
        ostream << data(i);
      }
      ostream << "\n";
      return ostream;
    }

    friend bool operator==(const MemoryBlock &a, const MemoryBlock &b) { return a.getRawView() == b.getRawView(); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    size_t mSize;
    using DeviceView = device::memory::NDView<1, T>;
    using HostView = typename DeviceView::host_mirror_type;
    DeviceView mData;
    mutable HostView mHostMirror;
    static constexpr size_t TSIZE = sizeof(T);
    mutable bool mHostMirrorOutdated = true;

    DEVICE_FORCEINLINE_FUNCTION
    void checkBounds(ptrdiff_t i) const
    {
#ifdef CHECKBOUNDS
#ifdef DEVICE_HAS_EXCEPTIONS
      if (i < 0 || i >= (ptrdiff_t)mSize)
        throw MemoryBlockOutOfBoundsException("Accessing memory block out of bounds:", i, "not in 0 -- ", mSize);
#endif
#endif
    }
  };

} // namespace TempLat

#endif
