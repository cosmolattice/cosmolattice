#ifndef KOKKOS_TEST_H
#define KOKKOS_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/complexalgebra/scalarcomplexmultiply.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include <cstdlib>

#ifndef NOKOKKOS
namespace TempLat
{
  template <typename NT> struct ctype {
    using value = NT;
  };

  template <typename T> struct ctype<complex<T>> {
    using value = T;
  };

  template <template <typename, typename> typename OP, typename NT = double>
  void test_binary_operator(TempLat::TDDAssertion &tdd)
  {
    using CT = typename ctype<NT>::value;
    const size_t big_number = 1000 * 1000;
    constexpr CT max_val = 5;
    auto transf = KOKKOS_LAMBDA(int i) { return 1 + i / (CT)big_number * max_val; };

    Kokkos::View<NT *, Kokkos::DefaultExecutionSpace> a("a", big_number);

    NT magic_number = 1. + (rand() % 5);
    if constexpr (std::is_same_v<NT, complex<CT>>) magic_number = complex<CT>(1. + (rand() % 5), 1. + (rand() % 5));

    Kokkos::parallel_for(
        Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>((size_t)0, big_number), KOKKOS_LAMBDA(size_t i) {
          auto op = OP(transf(i), magic_number);
          a(i) = op.get(0);
        });

    auto host_view = Kokkos::create_mirror_view(a);
    Kokkos::deep_copy(host_view, a);

    bool all_correct = true;
    for (size_t i = 0; i < big_number; ++i)
      all_correct &= TempLat::AlmostEqual(host_view[i], OP(transf(i), magic_number).get(0));
    tdd.verify(all_correct);
    if (!all_correct) {
      const auto nres = OP(transf(0), magic_number).get(0);
      say << "Failed with operation " << OP<NT, NT>::operatorString() << " and data type " << typeid(NT).name() << "\n"
          << "Relative error: " << ((host_view[0]) / nres - 1) << "\n";
    }
  }

  template <template <typename> typename OP, typename NT = double> void test_unary_operator(TempLat::TDDAssertion &tdd)
  {
    using CT = typename ctype<NT>::value;
    constexpr size_t big_number = 1000 * 1000;
    constexpr CT max_val = 2;
    auto transf = KOKKOS_LAMBDA(int i)->CT { return 1 + i / (CT)big_number * max_val; };

    Kokkos::View<NT *, Kokkos::DefaultExecutionSpace> a("a", big_number);

    NT magic_number = 0.; // 1. + (rand() % 5);
    if constexpr (std::is_same_v<NT, complex<CT>>) magic_number = complex<CT>(1. + (rand() % 5), 1. + (rand() % 5));

    Kokkos::parallel_for(
        Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>(0, big_number), KOKKOS_LAMBDA(int i) {
          auto op = Operators::Addition(OP(transf(i)), magic_number);
          a(i) = op.get(0);
        });

    auto host_view = Kokkos::create_mirror_view(a);
    Kokkos::deep_copy(host_view, a);

    bool all_correct = true;
    for (size_t i = 0; i < big_number; ++i)
      all_correct &= TempLat::AlmostEqual(host_view[i], Operators::Addition(OP(transf(i)), magic_number).get(0));
    tdd.verify(all_correct);
    if (!all_correct) {
      const auto nres = Operators::Addition(OP(transf(0.)), magic_number).get(0);
      say << "Failed with operation " << OP<NT>::operatorString() << " and data type " << typeid(NT).name() << "\n"
          << "Relative error: " << ((host_view[0]) / nres - 1) << "\n"
          << "Values: " << host_view[0] << " (GPU),  " << nres << " (CPU) \n";
    }
  }
} // namespace TempLat
#endif

inline void TempLat::KokkosTest::Test(TempLat::TDDAssertion &tdd)
{
#ifndef NOKOKKOS
  // NOTE: Some tests are commented out because they have missing functionality.
  // We should fix them eventually.

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

  /*
  // ---- test complex ----
  test_unary_operator<TempLat::Operators::ComplexConjugate, complex<double>
  >(tdd); test_unary_operator<TempLat::Operators::AbsoluteValue, complex<double>
  >(tdd); test_binary_operator<TempLat::Operators::Multiplication,
  complex<double> >(tdd); test_binary_operator<TempLat::Operators::Addition,
  complex<double> >(tdd); test_binary_operator<TempLat::Operators::Division,
  complex<double> >(tdd); test_binary_operator<TempLat::Operators::Power,
  complex<double> >(tdd); test_binary_operator<TempLat::Operators::Subtraction,
  complex<double> >(tdd); test_unary_operator<TempLat::Operators::AbsoluteValue,
  complex<double> >(tdd); test_unary_operator<TempLat::Operators::ASinh,
  complex<double> >(tdd); test_unary_operator<TempLat::Operators::Cosh,
  complex<double> >(tdd); test_unary_operator<TempLat::Operators::Cosine,
  complex<double> >(tdd);
  test_unary_operator<TempLat::Operators::DiracDeltaFunction, complex<double>
  >(tdd); test_unary_operator<TempLat::Operators::Exponential, complex<double>
  >(tdd); test_unary_operator<TempLat::Operators::Log, complex<double> >(tdd);
  test_unary_operator<TempLat::Operators::Sine, complex<double> >(tdd);
  test_unary_operator<TempLat::Operators::Sinh, complex<double> >(tdd);
  //test_unary_operator<TempLat::Operators::SafeSqrt, complex<double> >(tdd);
  test_unary_operator<TempLat::Operators::Tanh, complex<double> >(tdd);
  test_unary_operator<TempLat::Operators::UnaryMinus, complex<double> >(tdd);

  // ---- test float ----
  test_binary_operator<TempLat::Operators::Multiplication, float>(tdd);
  test_binary_operator<TempLat::Operators::Addition, float>(tdd);
  test_binary_operator<TempLat::Operators::Division, float>(tdd);
  //test_binary_operator<TempLat::Operators::Power, float>(tdd);
  //test_binary_operator<TempLat::Operators::Subtraction, float>(tdd);
  test_unary_operator<TempLat::Operators::AbsoluteValue, float>(tdd);
  test_unary_operator<TempLat::Operators::ASinh, float>(tdd);
  test_unary_operator<TempLat::Operators::Cosh, float>(tdd);
  test_unary_operator<TempLat::Operators::Cosine, float>(tdd);
  test_unary_operator<TempLat::Operators::DiracDeltaFunction, float>(tdd);
  test_unary_operator<TempLat::Operators::Exponential, float>(tdd);
  test_unary_operator<TempLat::HeavisideStepFunction, float>(tdd);
  test_unary_operator<TempLat::Operators::Log, float>(tdd);
  test_unary_operator<TempLat::Operators::Sine, float>(tdd);
  test_unary_operator<TempLat::Operators::Sinh, float>(tdd);
  test_unary_operator<TempLat::Operators::SafeSqrt, float>(tdd);
  test_unary_operator<TempLat::Operators::Tanh, float>(tdd);
  test_unary_operator<TempLat::Operators::UnaryMinus, float>(tdd);

  // ---- test complex ----
  test_unary_operator<TempLat::Operators::ComplexConjugate, complex<float>
  >(tdd); test_unary_operator<TempLat::Operators::AbsoluteValue, complex<float>
  >(tdd); test_binary_operator<TempLat::Operators::Multiplication,
  complex<float> >(tdd); test_binary_operator<TempLat::Operators::Addition,
  complex<float> >(tdd); test_binary_operator<TempLat::Operators::Division,
  complex<float> >(tdd);
  //test_binary_operator<TempLat::Operators::Power, complex<float> >(tdd);
  //test_binary_operator<TempLat::Operators::Subtraction, complex<float> >(tdd);
  test_unary_operator<TempLat::Operators::AbsoluteValue, complex<float> >(tdd);
  test_unary_operator<TempLat::Operators::ASinh, complex<float> >(tdd);
  test_unary_operator<TempLat::Operators::Cosh, complex<float> >(tdd);
  test_unary_operator<TempLat::Operators::Cosine, complex<float> >(tdd);
  test_unary_operator<TempLat::Operators::DiracDeltaFunction, complex<float>
  >(tdd); test_unary_operator<TempLat::Operators::Exponential, complex<float>
  >(tdd); test_unary_operator<TempLat::Operators::Log, complex<float> >(tdd);
  test_unary_operator<TempLat::Operators::Sine, complex<float> >(tdd);
  test_unary_operator<TempLat::Operators::Sinh, complex<float> >(tdd);
  //test_unary_operator<TempLat::Operators::SafeSqrt, complex<float> >(tdd);
  test_unary_operator<TempLat::Operators::Tanh, complex<float> >(tdd);
  test_unary_operator<TempLat::Operators::UnaryMinus, complex<float> >(tdd);

*/
  /*
  // test algebra on GPU
  // Make some operators first
  ptrdiff_t nGrid = 16, nGhost = 1;

  auto toolBox = MemoryToolBox::makeShared(3, nGrid, nGhost);

  toolBox->setVerbose();

  Field<double> psi("psi", toolBox);

  WaveNumber k(toolBox);

  psi.inFourierSpace() = k.norm2() * RandomGaussianField<double>("Hoi",toolBox);

  psi = 1;

  say << "psi(0) = " << psi.get(2) << "\n";

  const auto orig = psi.get(2);

  Kokkos::View<double *, Kokkos::DefaultExecutionSpace> a("a", 10);

  Kokkos::parallel_for(Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>((size_t)
  0, 10), KOKKOS_LAMBDA(size_t i) { Kokkos::printf("hello world from thread
  %d\n", i); Kokkos::printf("hello world from thread %f\n", psi.get(2));
                       });

  auto host_view = Kokkos::create_mirror_view(a);
  Kokkos::deep_copy(host_view, a);
  const auto new_val = host_view(0);
  say << "host_view(0) = " << host_view(0) << "\n";

  tdd.verify(AlmostEqual(new_val, orig));
*/
  /*
  WaveNumber k(toolBox);

  phi.inFourierSpace() = k.norm2() * RandomGaussianField<T>("Hoi",toolBox);

  //just manipulated phi(k), so it must still be in Fourier space, and ghosts
are stale. tdd.verify(phi.mManager->isFourierSpace());
  tdd.verify(phi.mManager->areGhostsStale());

  // alternatively, put the result of getNorm in a variable.
  //SpatialCoordinate x;
  //auto r = x.getNorm();

  chi = 3;//pow(r, 3);

  // just manipulated chi(x), so it must still be in configuration space, and
ghosts are stale. tdd.verify(!chi.mManager->isFourierSpace());
  tdd.verify(chi.mManager->isConfigSpace());
  tdd.verify(chi.mManager->areGhostsStale());

  // neat consequence of the implementation: an expression actually evaluates to
a specific type. Keeping that instance,
//     without passing it to an assignment operator, is simply the compiled
expression. So we can do stuff with it. auto potential = 0.5 * phi * phi + 42 *
chi * chi * phi * phi - chi + (-chi);

  // Stuff we can do:
  say << "Potential2: " << potential.toString() << "\n";

  // and awesomer:
  auto dVdPhi = potential.d(phi);

  say << "dPotential/dphi: " << dVdPhi.toString() << "\n";

  // didn't touch ghost cells of phi, so ghosts must still be stale. Also,
didn't touch phi at all, so must be in fourier space.
  tdd.verify(phi.mManager->isFourierSpace());
  tdd.verify(phi.mManager->areGhostsStale());

  psi = psi + chi;
  */
#endif
}

#endif // KOKKOS_TEST_H
