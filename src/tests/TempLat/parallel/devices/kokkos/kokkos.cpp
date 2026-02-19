/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/devices/kokkos/kokkos.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/scalarcomplexmultiply.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

  struct KokkosTest {
    template <typename TDDA> static void Test(TDDA &tdd);
  };

  template <typename NT> struct ctype {
    using value = NT;
  };

  template <typename T> struct ctype<complex<T>> {
    using value = T;
  };

  template <template <typename, typename> typename OP, typename NT = double>
  void test_binary_operator(TDDAssertion &tdd, typename ctype<NT>::value add_epsilon = 1)
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
          a(i) = op.eval(0);
        });

    auto host_view = Kokkos::create_mirror_view(a);
    Kokkos::deep_copy(host_view, a);

    bool all_correct = true;
    for (size_t i = 0; i < big_number; ++i) {
      const NT expect = OP(transf(i), magic_number).eval(0);
      const bool i_correct =
          AlmostEqual(host_view[i], expect, std::sqrt(std::numeric_limits<CT>::epsilon()) * add_epsilon);
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
  void test_unary_operator(TDDAssertion &tdd, typename ctype<NT>::value add_epsilon = 1)
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
          a(i) = op.eval(0);
        });

    auto host_view = Kokkos::create_mirror_view(a);
    Kokkos::deep_copy(host_view, a);

    bool all_correct = true;
    for (size_t i = 0; i < big_number; ++i) {
      const NT expect = Operators::Addition(OP(transf(i)), magic_number).eval(0);
      const bool i_correct =
          AlmostEqual(host_view[i], expect, std::sqrt(std::numeric_limits<CT>::epsilon()) * add_epsilon);
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

  template <typename TDDA> void KokkosTest::Test(TDDA &tdd)
  {
    // ---- test singleset ----
    {
      auto toolBox = MemoryToolBox<2>::makeShared(8, 0);
      Field<2, double> rField("rField", toolBox);
      const auto layout = toolBox->mLayouts.getConfigSpaceLayout();
      for (size_t i = 0; i < layout.getLocalSizes()[0]; ++i) {
        for (size_t j = 0; j < layout.getLocalSizes()[1]; ++j) {
          device_kokkos::memory::setAtOnePoint(rField, device_kokkos::IdxArray<2>{(int64_t)i, (int64_t)j}, i + j);
        }
      }
      {
        auto host_view = rField.getLocalNDHostView();
        bool all_correct = true;
        for (size_t i = 0; i < layout.getLocalSizes()[0]; ++i) {
          for (size_t j = 0; j < layout.getLocalSizes()[1]; ++j) {
            all_correct &= host_view(i, j) == i + j;
          }
        }
        tdd.verify(all_correct);
      }
      {
        bool all_correct = true;
        for (size_t i = 0; i < layout.getLocalSizes()[0]; ++i) {
          for (size_t j = 0; j < layout.getLocalSizes()[1]; ++j) {
            all_correct &= device_kokkos::memory::getAtOnePoint(
                               rField, device_kokkos::IdxArray<2>{(int64_t)i, (int64_t)j}) == i + j;
          }
        }
        tdd.verify(all_correct);
      }
    }

    // ---- test double ----
    // binary operators
    test_binary_operator<Operators::Multiplication>(tdd);
    test_binary_operator<Operators::Addition>(tdd);
    test_binary_operator<Operators::Division>(tdd);
    test_binary_operator<Operators::Power>(tdd);
    test_binary_operator<Operators::Subtraction>(tdd);
    // unary operators
    test_unary_operator<Operators::AbsoluteValue>(tdd);
    test_unary_operator<Operators::Cosh>(tdd);
    test_unary_operator<Operators::ASinh>(tdd);
    test_unary_operator<Operators::Cosine>(tdd);
    test_unary_operator<Operators::DiracDeltaFunction>(tdd);
    test_unary_operator<Operators::Exponential>(tdd);
    test_unary_operator<HeavisideStepFunction>(tdd);
    test_unary_operator<Operators::Log>(tdd);
    test_unary_operator<Operators::Sine>(tdd);
    test_unary_operator<Operators::Sinh>(tdd);
    test_unary_operator<Operators::SafeSqrt>(tdd);
    test_unary_operator<Operators::Tanh>(tdd);
    test_unary_operator<Operators::UnaryMinus>(tdd);

    // ---- test complex ----
    test_unary_operator<Operators::ComplexConjugate, complex<double>>(tdd);
    test_unary_operator<Operators::AbsoluteValue, complex<double>>(tdd);
    test_binary_operator<Operators::Multiplication, complex<double>>(tdd);
    test_binary_operator<Operators::Addition, complex<double>>(tdd);
    test_binary_operator<Operators::Division, complex<double>>(tdd);
    test_binary_operator<Operators::Power, complex<double>>(tdd);
    test_binary_operator<Operators::Subtraction, complex<double>>(tdd);
    test_unary_operator<Operators::AbsoluteValue, complex<double>>(tdd);
    test_unary_operator<Operators::ASinh, complex<double>>(tdd);
    test_unary_operator<Operators::Cosh, complex<double>>(tdd);
    test_unary_operator<Operators::Cosine, complex<double>>(tdd);
    test_unary_operator<Operators::DiracDeltaFunction, complex<double>>(tdd);
    test_unary_operator<Operators::Exponential, complex<double>>(tdd);
    test_unary_operator<Operators::Log, complex<double>>(tdd);
    test_unary_operator<Operators::Sine, complex<double>>(tdd);
    test_unary_operator<Operators::Sinh, complex<double>>(tdd);
    test_unary_operator<Operators::SafeSqrt, complex<double>>(tdd);
    test_unary_operator<Operators::Tanh, complex<double>>(tdd);
    test_unary_operator<Operators::UnaryMinus, complex<double>>(tdd);

    // ---- test float ----
    // binary operators
    test_binary_operator<Operators::Multiplication, float>(tdd, 1e+1);
    test_binary_operator<Operators::Addition, float>(tdd, 1e+1);
    test_binary_operator<Operators::Division, float>(tdd, 1e+1);
    test_binary_operator<Operators::Power, float>(tdd, 1e+1);
    test_binary_operator<Operators::Subtraction, float>(tdd, 5e+1);
    // unary operators
    test_unary_operator<Operators::AbsoluteValue, float>(tdd, 1e+1);
    test_unary_operator<Operators::ASinh, float>(tdd, 1e+1);
    test_unary_operator<Operators::Cosh, float>(tdd, 1e+1);
    test_unary_operator<Operators::Cosine, float>(tdd, 1e+3);
    test_unary_operator<Operators::DiracDeltaFunction, float>(tdd, 1e+1);
    test_unary_operator<Operators::Exponential, float>(tdd, 1e+1);
    test_unary_operator<HeavisideStepFunction, float>(tdd, 1e+1);
    test_unary_operator<Operators::Log, float>(tdd, 1e+3);
    test_unary_operator<Operators::Sine, float>(tdd, 1e+1);
    test_unary_operator<Operators::Sinh, float>(tdd, 1e+1);
    test_unary_operator<Operators::SafeSqrt, float>(tdd, 1e+1);
    test_unary_operator<Operators::Tanh, float>(tdd, 1e+1);
    test_unary_operator<Operators::UnaryMinus, float>(tdd, 1e+1);

    // ---- test complex ----
    test_unary_operator<Operators::ComplexConjugate, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::AbsoluteValue, complex<float>>(tdd, 1e+2);
    test_binary_operator<Operators::Multiplication, complex<float>>(tdd, 1e+2);
    test_binary_operator<Operators::Addition, complex<float>>(tdd, 1e+2);
    test_binary_operator<Operators::Division, complex<float>>(tdd, 1e+2);
    test_binary_operator<Operators::Power, complex<float>>(tdd, 1e+3);
    test_binary_operator<Operators::Subtraction, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::AbsoluteValue, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::ASinh, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::Cosh, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::Cosine, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::DiracDeltaFunction, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::Exponential, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::Log, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::Sine, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::Sinh, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::SafeSqrt, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::Tanh, complex<float>>(tdd, 1e+2);
    test_unary_operator<Operators::UnaryMinus, complex<float>>(tdd, 1e+2);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::KokkosTest> test;
}
