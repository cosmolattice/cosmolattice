#include "CosmoInterface/measurements/meansmeasurer.h"
#include "TempLat/parallel/device.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/session/sessionguard.h"
#include "TempLat/util/benchmark.h"

#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/lattice/algebra/complexalgebra/complexalgebra.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
#include "CosmoInterface/measurements/complexscalarmeasurer.h"

#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "CosmoInterface/couplingsmanager.h"
#include "TempLat/lattice/field/collections/fieldcollection.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include <iostream>

namespace TempLat
{
  class ComplexScalarModel
  {
  public:
    static constexpr size_t NCs = 1; // complex scalars
    static constexpr size_t NU1 = 0; // U(1) gauge fields

    ComplexScalarModel(auto toolBox, std::string name)
        : fldCS(name + "_cmplx_scalar", toolBox), piCS(name + "_pi_cmplx_scalar", toolBox),
          fldU1(name + "_U1", toolBox), piU1(name + "_pi_U1", toolBox)
    {
    }

    typedef CouplingsManager<NCs, NU1> CsU1Couplings; // couplings U(1) gauge-complex scalar
    CsU1Couplings gQ_CsU1;

    static constexpr size_t NDim = 3;
    using T = double;

    static constexpr double aI = 1.;
    static constexpr double alpha = 1.;
    static constexpr double dx = 1.;

    FieldCollection<ComplexField<NDim, T>, NCs> fldCS;
    FieldCollection<ComplexField<NDim, T>, NCs> piCS;

    VectorFieldCollection<Field<NDim, T>, NU1> fldU1;
    VectorFieldCollection<Field<NDim, T>, NU1> piU1;

    auto potDerivNormCS(Tag<0>) const { return ZeroType() * norm(fldCS(0_c)); }
  };

  template <class NormDeriv, class Fld, int I>
  static inline auto derivComponentFromNorm(NormDeriv nd, Fld fld, Tag<I> index)
  {
    return nd / norm(fld) * fld(index);
  }

  template <class Model, int N> static inline auto derivCS(Model &model, Tag<N> n)
  {
    return Complexify(derivComponentFromNorm(model.potDerivNormCS(n), model.fldCS(n), 0_c),
                      derivComponentFromNorm(model.potDerivNormCS(n), model.fldCS(n), 1_c));
  }

  template <class Model, int N, int I>
  static auto U1sForCSCovDerivs(Model &model, Tag<N> n,
                                Tag<I> i) // N is the matter index and i is the spatial dimension
  {

    return fold_multiply(
        MakeArray(a, 0, Model::NU1 - 1,
                  IfElse(Model::CsU1Couplings::couples(Tag<N>(), a),
                         complexPhase(-model.dx * model.gQ_CsU1(n, a) *
                                      model.fldU1(a)(i)), // if this CS couples to this U1, then U1 link
                         OneType())));                    // else, 1
  }

  template <class Model, int N> static auto covLaplacianCS(Model &model, Tag<N> n)
  {
    auto covPlus = Total(i, 1, Model::NDim, U1sForCSCovDerivs(model, n, i) * shift(model.fldCS(n), i));
    auto covMinus = Total(i, 1, Model::NDim, shift(conj(U1sForCSCovDerivs(model, n, i)) * model.fldCS(n), -i));
    return (covPlus + covMinus - (2.0 * Model::NDim) * model.fldCS(n)) / pow<2>(model.dx);
  }

  class ComplexScalarKernels
  {
  public:
    // Put public methods here. These should change very little over time.
    ComplexScalarKernels() = delete;

    template <class Model, int N> static auto get(Model &model, Tag<N> n)
    {
      // Returns kernel for complex scalars (formed by the covariant laplacian and potential derivative terms):
      return pow(model.aI, 1 + model.alpha) * covLaplacianCS(model, n) -
             pow(model.aI, 3 + model.alpha) / 2 * derivCS(model, n);
    }
  };
} // namespace TempLat

constexpr size_t NDim = 3;
using T = double;
constexpr size_t nGrid = 16;
constexpr size_t nGhost = 1;
constexpr size_t nSteps = 100;
constexpr size_t saveEvery = 10;
constexpr T dt = 0.01;

void run(auto toolBox, int manual)
{
  using namespace TempLat;

  ComplexScalarModel model(toolBox, manual == 1 ? "manual" : (manual == 2 ? "alt" : "kernel"));

  FilesManager<NDim> filesManager("./", toolBox, false, false, true);
  MeasurementsSaver<double> saver_phi_0(filesManager, model.fldCS(0_c)(0_c), true, false, MeansMeasurer::header());
  MeasurementsSaver<double> saver_phi_1(filesManager, model.fldCS(0_c)(1_c), true, false, MeansMeasurer::header());

  model.fldCS(0_c)(0_c).inFourierSpace() = RandomGaussianField<NDim, T>("Hoi", toolBox);
  model.fldCS(0_c)(0_c).inFourierSpace().setZeroMode(complex<double>{0.0, 0.0});
  model.fldCS(0_c)(1_c).inFourierSpace() = RandomGaussianField<NDim, T>("Hoi", toolBox);
  model.fldCS(0_c)(1_c).inFourierSpace().setZeroMode(complex<double>{0.0, 0.0});

  model.piCS(0_c)(0_c).inFourierSpace() = RandomGaussianField<NDim, T>("Hai", toolBox);
  model.piCS(0_c)(0_c).inFourierSpace().setZeroMode(complex<double>{0.0, 0.0});
  model.piCS(0_c)(1_c).inFourierSpace() = RandomGaussianField<NDim, T>("Hai", toolBox);
  model.piCS(0_c)(1_c).inFourierSpace().setZeroMode(complex<double>{0.0, 0.0});
  device::iteration::fence();

  MeansMeasurer::measure(saver_phi_0, model.fldCS(0_c)(0_c), model.piCS(0_c)(0_c), 0.);
  MeansMeasurer::measure(saver_phi_1, model.fldCS(0_c)(1_c), model.piCS(0_c)(1_c), 0.);
  saver_phi_0.save();
  saver_phi_1.save();

  {
    model.piCS(0_c)(0_c).getMemoryManager()->confirmGhostsUpToDate();
    auto dat = model.piCS(0_c)(0_c).getMemoryManager()->getRawView();
    double sum = 0.0;
    for (size_t i = 0; i < (1 + nGrid) * (1 + nGrid) * (1 + nGrid); ++i) {
      sum += dat[i];
      if (i > nGrid * nGrid + 1 && i < nGrid * nGrid + 6 + 1) {
        std::cout << "dat[" << i << "] = " << dat[i] << std::endl;
      }
    }
    std::cout << "Initial sum of real part of pi: " << sum << std::endl;
  }

  for (int s = 0; s < nSteps; ++s) {
    auto expression_phi = model.fldCS(0_c) + dt * model.piCS(0_c);

    if (manual == 1) {
      auto covPlus = Total(j, 1, NDim, shift(model.fldCS(0_c), j));
      auto covMinus = Total(j, 1, NDim, shift(model.fldCS(0_c), -j));
      auto expression_pi = model.piCS(0_c) + dt * ((covPlus + covMinus - (2.0 * NDim) * model.fldCS(0_c)));
      model.piCS(0_c) = expression_pi;
    } else if (manual == 2) {
      auto covPlusRe = Total(j, 1, NDim, shift(model.fldCS(0_c)(0_c), j));
      auto covMinusRe = Total(j, 1, NDim, shift(model.fldCS(0_c)(0_c), -j));
      auto covPlusIm = Total(j, 1, NDim, shift(model.fldCS(0_c)(1_c), j));
      auto covMinusIm = Total(j, 1, NDim, shift(model.fldCS(0_c)(1_c), -j));
      auto covPlus = Complexify(covPlusRe, covPlusIm);
      auto covMinus = Complexify(covMinusRe, covMinusIm);
      // auto expression_pi = model.piCS(0_c) + dt * ((covPlus + covMinus - (2.0 * NDim) * model.fldCS(0_c)));
      // auto expression_pi = model.piCS(0_c) + dt * ((covPlus - (3) * model.fldCS(0_c)));
      auto expression_pi_re = ((covPlusRe - (NDim)*model.fldCS(0_c)(0_c)));
      auto expression_pi_im = ((covPlusIm - (NDim)*model.fldCS(0_c)(1_c)));
      model.piCS(0_c)(0_c) = expression_pi_re;
      model.piCS(0_c)(1_c) = expression_pi_im;
    } else {
      auto expression_pi = model.piCS(0_c) + dt * ComplexScalarKernels::get(model, 0_c);
      model.piCS(0_c) = expression_pi;
    }
    model.fldCS(0_c) = expression_phi;
    device::iteration::fence();

    if (s > 0 && s % saveEvery == 0) {
      MeansMeasurer::measure(saver_phi_0, model.fldCS(0_c)(0_c), model.piCS(0_c)(0_c), s * dt);
      MeansMeasurer::measure(saver_phi_1, model.fldCS(0_c)(1_c), model.piCS(0_c)(1_c), s * dt);
      saver_phi_0.save();
      saver_phi_1.save();
    }
  }

  {
    model.piCS(0_c)(0_c).getMemoryManager()->confirmGhostsUpToDate();
    auto dat = model.piCS(0_c)(0_c).getMemoryManager()->getRawView();
    double sum = 0.0;
    for (size_t i = 0; i < (1 + nGrid) * (1 + nGrid) * (1 + nGrid); ++i) {
      sum += dat[i];
      if (i > nGrid * nGrid + 1 && i < nGrid * nGrid + 6 + 1) {
        std::cout << "dat[" << i << "] = " << dat[i] << std::endl;
      }
    }
    std::cout << "Final sum of real part of pi: " << sum << std::endl;
  }
}

int main(int argc, char **argv)
{
  using namespace TempLat;
  SessionGuard guard(argc, argv, false);

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost, false);
  toolBox->unsetVerbose();

  run(toolBox, 0);
  run(toolBox, 1);
  run(toolBox, 2);

  // ComplexScalarKernels::get(model, 0_c);
}
