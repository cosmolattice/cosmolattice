#ifndef TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::RandomGaussianFieldTester::Test(TempLat::TDDAssertion &tdd)
{
  /* test the stability of the getter at various coordinates. */

  ptrdiff_t nGrid = 32, nGhost = 1;

  auto toolBox = MemoryToolBox<3>::makeShared(nGrid, nGhost);
  RandomGaussianField<3, double> myField("Hello world", toolBox);
  //
  //  //  auto myFieldGetter = GetterWrap(myField);
  //
  //  //  auto test = myFieldGetter + myFieldGetter + myFieldGetter;
  //
  //  //  /* let it register the dimensionality */
  //  //  myField.confirmSpace(LayoutStruct(3), SpaceStateInterface::SpaceType::Fourier);
  //
  //    say << myField.getNDimensions() << " dimensions detected.\n";

  Kokkos::View<complex<double> *> a("a", 10);
  Kokkos::View<complex<double> *> b("b", 10);
  auto a_host = Kokkos::create_mirror_view(a);
  auto b_host = Kokkos::create_mirror_view(b);

  Kokkos::parallel_for(
      Kokkos::RangePolicy(0, a.size()), KOKKOS_LAMBDA(const size_t i) { a[i] = myField.get(0, 0, i); });

  auto firstSeed = myField.getCurrentSeed();

  // test rewinding
  Kokkos::parallel_for(
      Kokkos::RangePolicy(0, a.size()), KOKKOS_LAMBDA(const size_t i) { b[i] = myField.get(0, 0, i); });
  Kokkos::deep_copy(a_host, a);
  Kokkos::deep_copy(b_host, b);

  for (ptrdiff_t i = 0, iEnd = a.size(); i < iEnd; ++i)
    std::cout << "a = " << a_host[i] << ", b = " << b_host[i] << std::endl;

  bool rewindingWorks = true;
  for (ptrdiff_t i = 0, iEnd = a.size(); i < iEnd; ++i)
    rewindingWorks = rewindingWorks && AlmostEqual(a_host[i], b_host[i]);
  tdd.verify(rewindingWorks);
  tdd.verify(firstSeed == myField.getCurrentSeed());

  /*
  // test moving
  bool movingWorks = true;
  bool seedChanged = false;
  coord[1] = 10;
  for (ptrdiff_t i = 0, iEnd = a.size(); i < iEnd; ++i) {
    coord[2] = i;
    b[i] = myField.get(coord);
    seedChanged = firstSeed != myField.getCurrentSeed();
    if (!seedChanged) break;
    movingWorks = movingWorks && !AlmostEqual(a[i], b[i]);
  }
  tdd.verify(seedChanged);
  tdd.verify(movingWorks);

  // test moving back
  bool movingBackWorks = true;
  coord[1] = 0;
  for (ptrdiff_t i = 0, iEnd = a.size(); i < iEnd; ++i) {
    coord[2] = i;
    b[i] = myField.get(coord);
    movingBackWorks = movingBackWorks && AlmostEqual(a[i], b[i]);
  }
  tdd.verify(movingBackWorks);

  tdd.verify(Throws<RandomGaussianFieldNegativeFrequencyException>([&]() {
    coord[2] = -1;
    myField.get(coord);
  }));
  */
}

#endif
