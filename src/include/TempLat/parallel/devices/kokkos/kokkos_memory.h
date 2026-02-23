#ifndef TEMPLAT_PARALLEL_KOKKOS_MEMORY_H
#define TEMPLAT_PARALLEL_KOKKOS_MEMORY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"

#include "TempLat/parallel/devices/kokkos/kokkos.h"
#include "TempLat/parallel/devices/kokkos/kokkos_internal.h"

#include <Kokkos_Core.hpp>
#include <sstream>

namespace TempLat::device_kokkos::memory
{
      template <size_t NDim, typename T, typename Exec = DefaultExecutionSpace, typename Layout = DefaultLayout>
      using NDView = Kokkos::View<typename GetKokkosNDStarType<NDim, T>::type, // Get the star syntax for
                                                                               // dimensionality recursively with
                                  Layout, // LayoutRight is most compatible for now, may change in future
                                  Exec    // Choice between GPU and CPU
                                  >;
      template <size_t NDim, typename T, typename Exec = DefaultExecutionSpace, typename Layout = DefaultLayout>
      using NDViewUnmanaged =
          Kokkos::View<typename GetKokkosNDStarType<NDim, T>::type, // Get the star syntax for dimensionality
                                                                    // recursively
                       Layout, // LayoutRight is most compatible for now, may change in future
                       Exec,   // Choice between GPU and CPU
                       Kokkos::MemoryTraits<Kokkos::Unmanaged> // No allocation: Attach to existing memory
                       >;

      template <size_t NDim, typename T>
      using NDViewUnmanagedHost =
          Kokkos::View<typename GetKokkosNDStarType<NDim, T>::type, // Get the star syntax for dimensionality
                                                                    // recursively
                       DefaultLayout,             // LayoutRight is most compatible for now, may change in future
                       DefaultHostExecutionSpace, // Choice between GPU and CPU
                       Kokkos::MemoryTraits<Kokkos::Unmanaged> // No allocation: Attach to existing memory
                       >;

      using Kokkos::subview;

      template <typename A> auto createMirrorView(const A &a) { return Kokkos::create_mirror_view(a); }

      template <typename OBJ, size_t NDim, typename T>
      void setAtOnePoint(OBJ &&obj, device_kokkos::IdxArray<NDim> pos, T val)
      {
        Kokkos::parallel_for(
            "Set a point", Kokkos::RangePolicy(0, 1), DEVICE_LAMBDA(const unsigned int) {
              device_kokkos::apply([&](const auto... idx) { obj.getSet(idx...) = val; }, pos);
            });
      }

      template <typename View, typename T> void fill(View &view, const T &value) { Kokkos::deep_copy(view, value); }

      template <typename OBJ, size_t NDim, typename I = ptrdiff_t>
      GetGetReturnType<OBJ>::type getAtOnePoint(OBJ &&obj, const device_kokkos::array<I, NDim> &pos)
      {
        using T = GetGetReturnType<OBJ>::type;
        T ret;
        Kokkos::parallel_reduce(
            "Get a point", Kokkos::RangePolicy(0, 1),
            DEVICE_LAMBDA(const unsigned int, T &update) {
              device_kokkos::apply([&](const auto... idx) { update = DoEval::eval(obj, idx...); }, pos);
            },
            ret);
        return ret;
      }

      template <typename View1, typename View2>
        requires(Kokkos::is_view<View1>::value && Kokkos::is_view<View2>::value)
      void copyDeviceToDevice(const View1 &src, View2 &dest)
      {
        static_assert(View1::rank == View2::rank, "Source and destination views must have the same rank.");
        static_assert(std::is_same_v<typename View1::value_type, typename View2::value_type>,
                      "Source and destination views must have the same value type.");
        static_assert(std::is_same_v<typename View1::execution_space, typename View2::execution_space>,
                      "Source and destination views must have the same execution space.");
        // static_assert(std::is_same_v<typename View1::array_layout, typename View2::array_layout>,
        //               "Source and destination views must have the same layout.");

        constexpr size_t dim = View1::rank;
        for (size_t i = 0; i < dim; ++i)
          if (src.extent(i) != dest.extent(i)) {
            std::stringstream ss;
            ss << "Source and destination views must have the same extents. Mismatch at dimension " << i << ": "
               << "src extent = " << src.extent(i) << ", dest extent = " << dest.extent(i);
            throw std::runtime_error(ss.str());
          }

        bool contiguous = src.span_is_contiguous() && dest.span_is_contiguous();
        if (contiguous) {
          Kokkos::deep_copy(dest, src);
        } else {
          // If not, we need to do a manual copy
          device::array<ptrdiff_t, dim> localSizes;
          for (size_t i = 0; i < dim; ++i)
            localSizes[i] = src.extent(i);

          auto functor = DEVICE_LAMBDA(const device_kokkos::IdxArray<dim> &idx)
          {
            device_kokkos::apply([&](const auto... i) { dest(i...) = src(i...); }, idx);
          };

          Kokkos::parallel_for("Copy non-contiguous", getLocalKokkosPolicy({}, localSizes),
                               KokkosNDLambdaWrapper<dim, decltype(functor)>(functor));
        }
      }

      template <typename View>
        requires(Kokkos::is_view<View>::value)
      void copyDeviceToHost(View &src, typename View::value_type *dest)
      {
        constexpr size_t dim = View::rank();
        using T = typename View::value_type;
        using Exec = typename View::execution_space;
        using Layout = typename View::array_layout;

        const bool contiguous = src.span_is_contiguous();

        // If the source view is contiguous, we can use a simple copy
        if (contiguous) {
          auto destView = NDViewUnmanaged<dim, T, Exec, Layout>(dest, src.layout());
          Kokkos::deep_copy(destView, src);
        } else {
          // If not, we first need a temporary contiguous copy on device
          device_kokkos::array<ptrdiff_t, dim> localSizes;
          for (size_t i = 0; i < dim; ++i)
            localSizes[i] = src.extent(i);
          auto device_temp = device_kokkos::apply(
              [&](const auto... sizes) { return NDView<dim, T, Exec, Layout>("temp", sizes...); }, localSizes);
          copyDeviceToDevice(src, device_temp);

          // now copy the contiguous temp to host
          auto destView = NDViewUnmanaged<dim, T, Exec, Layout>(dest, device_temp.layout());
          Kokkos::deep_copy(destView, device_temp);
        }
      }

      template <typename View>
        requires(Kokkos::is_view<View>::value)
      void copyHostToDevice(const typename View::value_type *src, View &dest)
      {
        constexpr size_t dim = View::rank();
        using T = typename View::value_type;
        using Exec = typename View::execution_space;
        using Layout = typename View::array_layout;
        const bool contiguous = dest.span_is_contiguous();

        // If the destination view is contiguous, we can use a simple copy
        if (contiguous) {
          auto srcView = NDViewUnmanaged<dim, T, Exec, Layout>(const_cast<T *>(src), dest.layout());
          Kokkos::deep_copy(dest, srcView);
        } else {
          // If not, we first need a temporary contiguous copy on device
          device_kokkos::array<ptrdiff_t, dim> localSizes;
          for (size_t i = 0; i < dim; ++i)
            localSizes[i] = dest.extent(i);
          auto device_temp = device_kokkos::apply(
              [&](const auto... sizes) { return NDView<dim, T, Exec, Layout>("temp", sizes...); }, localSizes);
          copyHostToDevice(src, device_temp);

          // now copy the contiguous temp to the original view
          copyDeviceToDevice(device_temp, dest);
        }
      }
} // namespace TempLat::device_kokkos::memory

#endif
