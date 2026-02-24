#define FORCE_ACCESS_PATTERN 0

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/session/sessionguard.h"
#include "TempLat/util/benchmark.h"

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"

int main(int argc, char **argv)
{
  constexpr size_t NDim = 3;
  using T = double;
  constexpr size_t nGrid = 512;
  constexpr size_t nGhost = 1;
  constexpr size_t nSteps = 512;
  constexpr T dt = 0.01;

  using namespace TempLat;

  SessionGuard guard(argc, argv, false);

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost, false);
  toolBox->unsetVerbose();

  Field<NDim, T> phi("phi", toolBox);
  Field<NDim, T> pi("pi", toolBox);

  Benchmark bench([&](Benchmark::Measurer &measurer) {
    phi.inFourierSpace() = RandomGaussianField<NDim, T>("Rand", toolBox);
    pi.inFourierSpace() = RandomGaussianField<NDim, T>("Rand2", toolBox);

    for (size_t i = 0; i < nSteps; ++i) {
      pi.updateGhosts();
      device::iteration::fence();

      measurer.measure("timestepping", [&]() {
        pi = pi + dt * LatticeLaplacian<NDim, decltype(phi)>(phi);
        phi = phi + dt * pi;
        device::iteration::fence();
      });
    }
  });

  // Run the benchmark once
  bench.run(1);

  // Output the results
  sayMPI << bench;
  bench.log("access_pattern_0");
}
