#include "CosmoInterface/measurements/meansmeasurer.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/session/sessionguard.h"
#include "TempLat/util/benchmark.h"

#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/lattice/algebra/complexalgebra/complexalgebra.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
#include "CosmoInterface/measurements/complexscalarmeasurer.h"

#include "TempLat/util/rangeiteration/sum_in_range.h"

int main(int argc, char **argv)
{
  using namespace TempLat;

  SessionGuard guard(argc, argv, false);

  constexpr size_t NDim = 3;
  using T = double;
  constexpr size_t nGrid = 64;
  constexpr size_t nGhost = 1;
  constexpr size_t nSteps = 100;
  constexpr T dt = 0.01;
  constexpr double m2 = 1.0;

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost, false);

  toolBox->unsetVerbose();

  ComplexField<NDim, T> phi("phi", toolBox);
  ComplexField<NDim, T> pi("pi", toolBox);

  FilesManager<NDim> filesManager("./", toolBox, false, false, true);
  MeasurementsSaver<double> saver_phi_0(filesManager, phi(0_c), true, false, MeansMeasurer::header());
  MeasurementsSaver<double> saver_phi_1(filesManager, phi(1_c), true, false, MeansMeasurer::header());

  Benchmark bench([&](Benchmark::Measurer &measurer) {
    // measurer.measure("x->k fourier", [&]() {
    //   phi(0_c).getMemoryManager()->confirmFourierSpace();
    //   phi(1_c).getMemoryManager()->confirmFourierSpace();
    //   pi(0_c).getMemoryManager()->confirmFourierSpace();
    //   pi(1_c).getMemoryManager()->confirmFourierSpace();
    // device::iteration::fence();
    // });

    measurer.measure("initialize field", [&]() {
      phi(0_c).inFourierSpace() = RandomGaussianField<NDim, T>("Hoi", toolBox);
      phi(0_c).inFourierSpace().setZeroMode(complex<double>{powr<NDim>(nGrid), 0.0});
      phi(1_c).inFourierSpace() = RandomGaussianField<NDim, T>("Hoi", toolBox);
      phi(1_c).inFourierSpace().setZeroMode(complex<double>{powr<NDim>(nGrid), 0.0});

      pi(0_c).inFourierSpace() = RandomGaussianField<NDim, T>("Hai", toolBox);
      pi(1_c).inFourierSpace() = RandomGaussianField<NDim, T>("Hai", toolBox);
      // device::iteration::fence();
    });

    // measurer.measure("k->x fourier", [&]() {
    //   phi(0_c).getMemoryManager()->confirmConfigSpace();
    //   phi(1_c).getMemoryManager()->confirmConfigSpace();
    //   pi(0_c).getMemoryManager()->confirmConfigSpace();
    //   pi(1_c).getMemoryManager()->confirmConfigSpace();
    // device::iteration::fence();
    // });

    for (size_t i = 0; i < nSteps; ++i) {
      measurer.measure("ghosts", [&]() {
        // phi.updateGhosts();
        // pi.updateGhosts();
        // device::iteration::fence();
      });
      measurer.measure("timestepping", [&]() {
        auto covPlus = Total(j, 1, NDim, shift(phi, j));
        auto covMinus = Total(j, 1, NDim, shift(phi, -j));
        auto expression_pi = pi + dt * (covPlus + covMinus - (2.0 * NDim) * phi) - m2 * phi;
        auto expression_phi = phi + dt * pi;

        pi = expression_pi;
        phi = expression_phi;
        // device::iteration::fence();
      });
      MeansMeasurer::measure(saver_phi_0, phi(0_c), pi(0_c), i * dt);
      MeansMeasurer::measure(saver_phi_1, phi(1_c), pi(1_c), i * dt);
      saver_phi_0.save();
      saver_phi_1.save();
    }
  });
  bench.run(1);
  sayMPI << bench;
  bench.log("phi4");
}
