//
// Created by franz on 6/4/25.
//

#ifndef KOKKOS_TEST_H
#define KOKKOS_TEST_H

#include "TempLat/lattice/algebra/operators/operators.h"
#include <cstdlib>

#ifndef NOKOKKOS
template<template<typename, typename> typename OP>
void test_binary_operator(TempLat::TDDAssertion &tdd) {
    const size_t big_number = 1000 * 1000;
    Kokkos::View<double *, Kokkos::DefaultExecutionSpace> a("a", big_number);
    const double magic_number = 5. + rand();

    Kokkos::parallel_for(Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>((size_t)0, big_number),
                         KOKKOS_LAMBDA(size_t i) {
                             auto op = OP((double)i, magic_number);
                             a(i) = op.get(0);
                         });

    auto host_view = Kokkos::create_mirror_view(a);
    Kokkos::deep_copy(host_view, a);

    bool all_correct = true;
    for (size_t i = 0; i < big_number; ++i)
        all_correct &= TempLat::AlmostEqual(host_view[i], OP((double)i, magic_number).get(0));
    tdd.verify(all_correct);
    if (!all_correct)
        say << "Failed with operation " << OP<double, double>::operatorString() << "\n";
}

template<template<typename> typename OP>
void test_unary_operator(TempLat::TDDAssertion &tdd) {
    const size_t big_number = 1000 * 1000;
    Kokkos::View<double *, Kokkos::DefaultExecutionSpace> a("a", big_number);

    Kokkos::parallel_for(Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>(0, big_number),
                         KOKKOS_LAMBDA(int i) {
                             auto op = OP((double) i);
                             a(i) = op.get(0);
                         });

    auto host_view = Kokkos::create_mirror_view(a);
    Kokkos::deep_copy(host_view, a);

    bool all_correct = true;
    for (size_t i = 0; i < big_number; ++i)
        all_correct &= TempLat::AlmostEqual(host_view[i], OP((double)i).get(0));
    tdd.verify(all_correct);
    if (!all_correct)
        say << "Failed with operation " << OP<double>::operatorString() << "\n";
}
#endif

inline void TempLat::KokkosTest::Test(TempLat::TDDAssertion &tdd) {
#ifndef NOKOKKOS
    test_binary_operator<TempLat::Operators::Multiplication>(tdd);
    test_binary_operator<TempLat::Operators::Addition>(tdd);
    test_binary_operator<TempLat::Operators::Division>(tdd);
    test_binary_operator<TempLat::Operators::Power>(tdd);
    test_binary_operator<TempLat::Operators::Substraction>(tdd);

    test_unary_operator<TempLat::Operators::AbsoluteValue>(tdd);
    test_unary_operator<TempLat::Operators::ASinh>(tdd);
    test_unary_operator<TempLat::Operators::Cosh>(tdd);
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

    // test algebra on GPU
    /*
    // Make some operators first
    ptrdiff_t nGrid = 256, nGhost = 1;

    auto toolBox = MemoryToolBox::makeShared(3, nGrid, nGhost);

    toolBox->setVerbose();

    Field<T> phi("phi", toolBox);
    Field<T> chi("chi", toolBox);
    Field<T> psi("psi", toolBox);

    WaveNumber k(toolBox);

    phi.inFourierSpace() = k.norm2() * RandomGaussianField<T>("Hoi",toolBox);

    //just manipulated phi(k), so it must still be in Fourier space, and ghosts are stale.
    tdd.verify(phi.mManager->isFourierSpace());
    tdd.verify(phi.mManager->areGhostsStale());

    // alternatively, put the result of getNorm in a variable.
    //SpatialCoordinate x;
    //auto r = x.getNorm();

    chi = 3;//pow(r, 3);

    // just manipulated chi(x), so it must still be in configuration space, and ghosts are stale.
    tdd.verify(!chi.mManager->isFourierSpace());
    tdd.verify(chi.mManager->isConfigSpace());
    tdd.verify(chi.mManager->areGhostsStale());

    // neat consequence of the implementation: an expression actually evaluates to a specific type. Keeping that instance,
  //     without passing it to an assignment operator, is simply the compiled expression. So we can do stuff with it.
    auto potential = 0.5 * phi * phi + 42 * chi * chi * phi * phi - chi + (-chi);

    // Stuff we can do:
    say << "Potential2: " << potential.toString() << "\n";

    // and awesomer:
    auto dVdPhi = potential.d(phi);

    say << "dPotential/dphi: " << dVdPhi.toString() << "\n";

    // didn't touch ghost cells of phi, so ghosts must still be stale. Also, didn't touch phi at all, so must be in fourier space.
    tdd.verify(phi.mManager->isFourierSpace());
    tdd.verify(phi.mManager->areGhostsStale());

    psi = psi + chi;
    */
#endif
}

#endif //KOKKOS_TEST_H
