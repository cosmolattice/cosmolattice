#ifndef TEMPLAT_PARALLEL_KOKKOS_KOKKOS_TEST_H
#define TEMPLAT_PARALLEL_KOKKOS_KOKKOS_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/complexalgebra/scalarcomplexmultiply.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{
  template <typename NT> struct ctype {
    using value = NT;
  };

  template <typename T> struct ctype<complex<T>> {
    using value = T;
  };

  template <template <typename, typename> typename OP, typename NT = double>
  void test_binary_operator(TempLat::TDDAssertion &tdd, typename ctype<NT>::value add_epsilon = 1)
  {
    using CT = typename ctype<NT>::value;
    const size_t big_number = 1000 * 1000;
    constexpr CT max_val = 5;
    auto transf = DEVICE_LAMBDA(int i)->CT { return 1 + i / (CT)big_number * max_val; };

    Kokkos::View<NT *, Kokkos::DefaultExecutionSpace> a("a", big_number);

    NT magic_number = 1. + (rand() % 5);
    if constexpr (std::is_same_v<NT, complex<CT>>) magic_number = complex<CT>(1. + (rand() % 5), 1. + (rand() % 5));

    Kokkos::parallel_for(
        Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>((size_t)0, big_number), DEVICE_LAMBDA(size_t i) {
          auto op = OP(transf(i), magic_number);
          a(i) = op.get(0);
        });

    auto host_view = Kokkos::create_mirror_view(a);
    Kokkos::deep_copy(host_view, a);

    bool all_correct = true;
    for (size_t i = 0; i < big_number; ++i) {
      const NT expect = OP(transf(i), magic_number).get(0);
      const bool i_correct =
          TempLat::AlmostEqual(host_view[i], expect, std::sqrt(std::numeric_limits<CT>::epsilon()) * add_epsilon);
      all_correct &= i_correct;
      if (!i_correct) {
        say << "Failed at index " << i << " with operation " << OP(transf(i), magic_number).operatorString()
            << " and data type " << typeid(NT).name() << "\n"
            << "Relative error: " << (device_kokkos::abs(host_view[i] / expect) - 1.) << "\n"
            << "Values: " << host_view[i] << " (GPU),  " << expect << " (CPU) \n";
      }
    }
    tdd.verify(all_correct);
  }

  template <template <typename> typename OP, typename NT = double>
  void test_unary_operator(TempLat::TDDAssertion &tdd, typename ctype<NT>::value add_epsilon = 1)
  {
    using CT = typename ctype<NT>::value;
    constexpr size_t big_number = 1000 * 1000;
    constexpr CT max_val = 2;
    auto transf = DEVICE_LAMBDA(int i)->CT { return 1 + i / (CT)big_number * max_val; };

    Kokkos::View<NT *, Kokkos::DefaultExecutionSpace> a("a", big_number);

    NT magic_number = 0.; // 1. + (rand() % 5);
    if constexpr (std::is_same_v<NT, complex<CT>>) magic_number = complex<CT>(1. + (rand() % 5), 1. + (rand() % 5));

    Kokkos::parallel_for(
        Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>(0, big_number), DEVICE_LAMBDA(int i) {
          auto op = Operators::Addition(OP(transf(i)), magic_number);
          a(i) = op.get(0);
        });

    auto host_view = Kokkos::create_mirror_view(a);
    Kokkos::deep_copy(host_view, a);

    bool all_correct = true;
    for (size_t i = 0; i < big_number; ++i) {
      const NT expect = Operators::Addition(OP(transf(i)), magic_number).get(0);
      const bool i_correct =
          TempLat::AlmostEqual(host_view[i], expect, std::sqrt(std::numeric_limits<CT>::epsilon()) * add_epsilon);
      all_correct &= i_correct;
      if (!i_correct) {
        say << "Failed Unary operation at index " << i << " with operation " << OP(transf(i)).operatorString()
            << " and data type " << typeid(NT).name() << "\n"
            << "Relative error: " << (device_kokkos::abs(host_view[i] / expect) - 1.) << " > "
            << std::sqrt(std::numeric_limits<CT>::epsilon()) * add_epsilon << "\n"
            << "Values: " << host_view[i] << " (GPU),  " << expect << " (CPU) \n";
      }
    }
    tdd.verify(all_correct);
  }
} // namespace TempLat

template <typename TDDA> inline void TempLat::KokkosTest::Test(TDDA &tdd)
{
  // ---- test singleset ----
  {
    auto toolBox = MemoryToolBox<2>::makeShared(8, 0);
    Field<2, double> rField("rField", toolBox);
    for (uint i = 0; i < 8; ++i) {
      for (uint j = 0; j < 8; ++j) {
        device_kokkos::memory::setAtOnePoint(rField, device_kokkos::array<ptrdiff_t, 2>{i, j}, i + j);
      }
    }
    {
      auto host_view = rField.getLocalNDHostView();
      bool all_correct = true;
      for (uint i = 0; i < 8; ++i) {
        for (uint j = 0; j < 8; ++j) {
          all_correct &= host_view(i, j) == i + j;
        }
      }
      tdd.verify(all_correct);
    }
    {
      bool all_correct = true;
      for (uint i = 0; i < 8; ++i) {
        for (uint j = 0; j < 8; ++j) {
          all_correct &=
              device_kokkos::memory::getAtOnePoint(rField, device_kokkos::array<ptrdiff_t, 2>{i, j}) == i + j;
        }
      }
      tdd.verify(all_correct);
    }
  }

  // ---- test double ----
  // binary operators
  test_binary_operator<TempLat::Operators::Multiplication>(tdd);
  test_binary_operator<TempLat::Operators::Addition>(tdd);
  test_binary_operator<TempLat::Operators::Division>(tdd);
  test_binary_operator<TempLat::Operators::Power>(tdd);
  test_binary_operator<TempLat::Operators::Subtraction>(tdd);
  // unary operators
  test_unary_operator<TempLat::Operators::AbsoluteValue>(tdd);
  test_unary_operator<TempLat::Operators::Cosh>(tdd);
  test_unary_operator<TempLat::Operators::ASinh>(tdd);
  test_unary_operator<TempLat::Operators::Cosine>(tdd);
  test_unary_operator<TempLat::Operators::DiracDeltaFunction>(tdd);
  test_unary_operator<TempLat::Operators::Exponential>(tdd);
  test_unary_operator<TempLat::HeavisideStepFunction>(tdd);
  test_unary_operator<TempLat::Operators::Log>(tdd);
  test_unary_operator<TempLat::Operators::Sine>(tdd);
  test_unary_operator<TempLat::Operators::Sinh>(tdd);
  test_unary_operator<TempLat::Operators::SafeSqrt>(tdd);
  test_unary_operator<TempLat::Operators::Tanh>(tdd);
  test_unary_operator<TempLat::Operators::UnaryMinus>(tdd);

  // ---- test complex ----
  test_unary_operator<TempLat::Operators::ComplexConjugate, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::AbsoluteValue, complex<double>>(tdd);
  test_binary_operator<TempLat::Operators::Multiplication, complex<double>>(tdd);
  test_binary_operator<TempLat::Operators::Addition, complex<double>>(tdd);
  test_binary_operator<TempLat::Operators::Division, complex<double>>(tdd);
  test_binary_operator<TempLat::Operators::Power, complex<double>>(tdd);
  test_binary_operator<TempLat::Operators::Subtraction, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::AbsoluteValue, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::ASinh, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::Cosh, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::Cosine, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::DiracDeltaFunction, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::Exponential, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::Log, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::Sine, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::Sinh, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::SafeSqrt, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::Tanh, complex<double>>(tdd);
  test_unary_operator<TempLat::Operators::UnaryMinus, complex<double>>(tdd);

  // ---- test float ----
  // binary operators
  test_binary_operator<TempLat::Operators::Multiplication, float>(tdd, 1e+1);
  test_binary_operator<TempLat::Operators::Addition, float>(tdd, 1e+1);
  test_binary_operator<TempLat::Operators::Division, float>(tdd, 1e+1);
  test_binary_operator<TempLat::Operators::Power, float>(tdd, 1e+1);
  test_binary_operator<TempLat::Operators::Subtraction, float>(tdd, 1e+1);
  // unary operators
  test_unary_operator<TempLat::Operators::AbsoluteValue, float>(tdd, 1e+1);
  test_unary_operator<TempLat::Operators::ASinh, float>(tdd, 1e+1);
  test_unary_operator<TempLat::Operators::Cosh, float>(tdd, 1e+1);
  test_unary_operator<TempLat::Operators::Cosine, float>(tdd, 1e+3);
  test_unary_operator<TempLat::Operators::DiracDeltaFunction, float>(tdd, 1e+1);
  test_unary_operator<TempLat::Operators::Exponential, float>(tdd, 1e+1);
  test_unary_operator<TempLat::HeavisideStepFunction, float>(tdd, 1e+1);
  test_unary_operator<TempLat::Operators::Log, float>(tdd, 1e+3);
  test_unary_operator<TempLat::Operators::Sine, float>(tdd, 1e+1);
  test_unary_operator<TempLat::Operators::Sinh, float>(tdd, 1e+1);
  test_unary_operator<TempLat::Operators::SafeSqrt, float>(tdd, 1e+1);
  test_unary_operator<TempLat::Operators::Tanh, float>(tdd, 1e+1);
  test_unary_operator<TempLat::Operators::UnaryMinus, float>(tdd, 1e+1);

  // ---- test complex ----
  test_unary_operator<TempLat::Operators::ComplexConjugate, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::AbsoluteValue, complex<float>>(tdd, 1e+2);
  test_binary_operator<TempLat::Operators::Multiplication, complex<float>>(tdd, 1e+2);
  test_binary_operator<TempLat::Operators::Addition, complex<float>>(tdd, 1e+2);
  test_binary_operator<TempLat::Operators::Division, complex<float>>(tdd, 1e+2);
  test_binary_operator<TempLat::Operators::Power, complex<float>>(tdd, 1e+3);
  test_binary_operator<TempLat::Operators::Subtraction, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::AbsoluteValue, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::ASinh, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::Cosh, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::Cosine, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::DiracDeltaFunction, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::Exponential, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::Log, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::Sine, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::Sinh, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::SafeSqrt, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::Tanh, complex<float>>(tdd, 1e+2);
  test_unary_operator<TempLat::Operators::UnaryMinus, complex<float>>(tdd, 1e+2);
}

#endif // KOKKOS_TEST_H
