#ifndef TEMPLAT_PARALLEL_DEVICE_MEMORY_H
#define TEMPLAT_PARALLEL_DEVICE_MEMORY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#include "TempLat/parallel/device.h"

#ifdef DEVICE_KOKKOS

#include "TempLat/parallel/devices/kokkos/kokkos_memory.h"

#elif DEVICE_STD

#include "TempLat/parallel/devices/std/std_memory.h"

#else

static_assert(false, "No device memory backend selected.");

#endif

#ifndef DEVICE_FUNCTION
static_assert(false, "No DEVICE_FUNCTION defined.");
#endif
#ifndef DEVICE_FORCEINLINE_FUNCTION
static_assert(false, "No DEVICE_FORCEINLINE_FUNCTION defined.");
#endif
#ifndef DEVICE_INLINE_FUNCTION
static_assert(false, "No DEVICE_INLINE_FUNCTION defined.");
#endif
#ifndef DEVICE_LAMBDA
static_assert(false, "No DEVICE_LAMBDA defined.");
#endif
#ifndef DEVICE_CLASS_LAMBDA
static_assert(false, "No DEVICE_CLASS_LAMBDA defined.");
#endif

#include <atomic>

namespace TempLat::device::memory
{
      using export_device_namespace::memory::NDView;
      using export_device_namespace::memory::NDViewUnmanaged;
      using export_device_namespace::memory::NDViewUnmanagedHost;

      using export_device_namespace::memory::createMirrorView;
      using export_device_namespace::memory::subview;

      using export_device_namespace::memory::copyDeviceToDevice;
      using export_device_namespace::memory::copyDeviceToHost;
      using export_device_namespace::memory::copyHostToDevice;
      using export_device_namespace::memory::fill;
      using export_device_namespace::memory::getAtOnePoint;
      using export_device_namespace::memory::setAtOnePoint;

      template <typename T> class host_ptr
      {
      public:
        // delete the default constructor
        DEVICE_FUNCTION
        host_ptr() : mPtr(nullptr), mRefCount(nullptr) {}

        template <typename... ARGS>
          requires requires(ARGS... args) { T(args...); }
        DEVICE_FUNCTION host_ptr(const ARGS &...args)
        {
#ifndef DEVICE_REGION
          mPtr = new T(args...);
          mRefCount = new std::atomic<size_t>(1);
#else
          mPtr = nullptr;
          mRefCount = nullptr;
#endif
        }

        DEVICE_FUNCTION host_ptr(T *ptr)
        {
#ifndef DEVICE_REGION
          if (ptr == nullptr) {
            mPtr = nullptr;
            mRefCount = nullptr;
          } else {
            mPtr = ptr;
            mRefCount = new std::atomic<size_t>(1);
          }
#else
          mPtr = nullptr;
          mRefCount = nullptr;
#endif
        }

        DEVICE_FUNCTION
        host_ptr<T> &operator=(const host_ptr<T> &other)
        {
#ifndef DEVICE_REGION
          if (this->mRefCount != nullptr) {
            --(*this->mRefCount);
            update_ref_count();
          }
          this->mPtr = other.mPtr;
          this->mRefCount = other.mRefCount;
          if (this->mRefCount != nullptr) ++(*this->mRefCount);
#else
          mPtr = nullptr;
          mRefCount = nullptr;
#endif
          return *this;
        }

        DEVICE_FUNCTION
        host_ptr<T> &operator=(T *ptr)
        {
#ifndef DEVICE_REGION
          if (mRefCount != nullptr) {
            --(*mRefCount);
            update_ref_count();
          }
          if (ptr == nullptr) {
            mPtr = nullptr;
            mRefCount = nullptr;
          } else {
            mPtr = ptr;
            mRefCount = new std::atomic<size_t>(1);
          }
#else
          mPtr = nullptr;
          mRefCount = nullptr;
#endif
          return *this;
        }

        DEVICE_FUNCTION
        host_ptr(const host_ptr &other)
        {
#ifndef DEVICE_REGION
          if (mRefCount != nullptr) {
            --(*mRefCount);
            update_ref_count();
          }
          mPtr = other.mPtr;
          mRefCount = other.mRefCount;
          if (mRefCount != nullptr) ++(*mRefCount);
#else
          mPtr = nullptr;
          mRefCount = nullptr;
#endif
        }

        DEVICE_FUNCTION
        ~host_ptr()
        {
#ifndef DEVICE_REGION
          if (mRefCount == nullptr) return;
          --(*mRefCount);
          update_ref_count();
#endif
        }

        DEVICE_FUNCTION
        T *operator->() const { return this->get(); }
        DEVICE_FUNCTION
        T &operator*() const { return *(this->get()); }
        DEVICE_FUNCTION
        T *get() const { return mPtr; }

        DEVICE_FUNCTION
        bool operator==(const host_ptr &other) const { return mPtr == other.mPtr; }

        size_t use_count() const
        {
#ifndef DEVICE_REGION
          if (mRefCount != nullptr) return *mRefCount;
#endif
          return 0;
        }

      private:
        T *mPtr = nullptr;
        std::atomic<size_t> *mRefCount = nullptr;

        void update_ref_count()
        {
          if (mRefCount != nullptr && *mRefCount == 0) {
            delete mPtr;
            delete mRefCount;
            mPtr = nullptr;
            mRefCount = nullptr;
          }
        }
      };

      using host_string = host_ptr<std::string>;
} // namespace TempLat::device::memory

#endif
