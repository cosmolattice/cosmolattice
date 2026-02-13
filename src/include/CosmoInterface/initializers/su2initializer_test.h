#ifndef COSMOINTERFACE_INITIALIZERS_SU2INITIALIZER_TEST_H
#define COSMOINTERFACE_INITIALIZERS_SU2INITIALIZER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

inline void TempLat::SU2Initializer::Test(TempLat::TDDAssertion &tdd)
{
  static constexpr size_t NDim = 3;
  static constexpr ptrdiff_t nGrid = 4, nGhost = 0;
  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);
  using T = double;

  FourierSite<NDim> ntilde(toolBox);

  auto expIK = MakeVector(i, 1, NDim, complexPhase(-2.0 * Constants::pi<T> / nGrid * ntilde(i))); // e^(-2*pi*k/N)
  auto keffm = MakeVector(i, 1, NDim, 1_c - expIK(i));
  auto keffm2 = Total(i, 1, NDim, norm2(keffm(i))); // |keffm|^2

  const auto &fftSizes = toolBox->mLayouts.getFourierSpaceSizes();

  auto print_it = [&](const auto obj) {
    device::IdxArray<3> see_at{};
    for (device::Idx i = 0; i < 1; ++i) {
      for (device::Idx j = 0; j < fftSizes[1]; ++j) {
        for (device::Idx k = 0; k < fftSizes[2]; ++k) {
          see_at = {i, j, k};
          device::apply([&](const auto &...idx) { DoEval::eval(obj, idx...); }, see_at);
          device::apply([&](const auto &...idx) { std::cout << std::setw(5) << obj.get(idx...); }, see_at);
          if (k == fftSizes[2] - 1)
            std::cout << std::endl;
          else
            std::cout << ", ";
        }
        if (j == fftSizes[1] - 1) std::cout << std::endl << std::endl;
      }
    }
  };

  std::cout << "ntilde(1_c):" << std::endl;
  print_it(ntilde(1_c));
  std::cout << "ntilde(2_c):" << std::endl;
  print_it(ntilde(2_c));
  std::cout << "ntilde(3_c):" << std::endl;
  print_it(ntilde(3_c));

  std::cout << "Re expIK(1_c):" << std::endl;
  print_it(expIK(1_c).ComplexFieldGet(0_c));
  std::cout << "Im expIK(1_c):" << std::endl;
  print_it(expIK(1_c).ComplexFieldGet(1_c));

  std::cout << "Re expIK(2_c):" << std::endl;
  print_it(expIK(2_c).ComplexFieldGet(0_c));
  std::cout << "Im expIK(2_c):" << std::endl;
  print_it(expIK(2_c).ComplexFieldGet(1_c));

  std::cout << "Re expIK(3_c):" << std::endl;
  print_it(expIK(3_c).ComplexFieldGet(0_c));
  std::cout << "Im expIK(3_c):" << std::endl;
  print_it(expIK(3_c).ComplexFieldGet(1_c));

  std::cout << "Re keffm(1_c):" << std::endl;
  print_it(keffm(1_c).ComplexFieldGet(0_c));
  std::cout << "Im keffm(1_c):" << std::endl;
  print_it(keffm(1_c).ComplexFieldGet(1_c));

  std::cout << "Re keffm(2_c):" << std::endl;
  print_it(keffm(2_c).ComplexFieldGet(0_c));
  std::cout << "Im keffm(2_c):" << std::endl;
  print_it(keffm(2_c).ComplexFieldGet(1_c));

  std::cout << "Re keffm(3_c):" << std::endl;
  print_it(keffm(3_c).ComplexFieldGet(0_c));
  std::cout << "Im keffm(3_c):" << std::endl;
  print_it(keffm(3_c).ComplexFieldGet(1_c));

  std::cout << "keffm2:" << std::endl;
  print_it(keffm2);

  Field<NDim, T> j0a("j0a", toolBox);
  j0a.inFourierSpace() = Complexify(1.0, 1.0);

  auto expr = asFourier(conj(keffm(3_c)) * (1 / keffm2)) * j0a.inFourierSpace();
  std::cout << "expr (in Fourier space):" << std::endl;
  print_it(expr);

  Field<NDim, T> result("result", toolBox);
  result.inFourierSpace() = expr;
  result.inFourierSpace().setZeroMode(0);
  std::cout << "result (in Fourier space):" << std::endl;
  print_it(result.inFourierSpace());

  using fldt = Field<NDim, T>;
  using cfldt = ComplexField<NDim, T>;
  using cfldtvec = VectorField<cfldt, T, NDim>;
  cfldtvec keffm_f("keffm", toolBox);
  ForLoop(i, 1, NDim, keffm_f(i).inFourierSpace() = keffm(i););

  tdd.verify(true);
}

#endif
