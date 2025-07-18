#include "TempLat/util/tdd/tdd.h"
#include "TempLat/session/sessionguard.h"
#include "TempLat/util/benchmark.h"

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"

int main(int argc, char **argv)
{
  using namespace TempLat;

  SessionGuard guard(argc, argv, false);

  constexpr size_t NDim = 3;
  using T = double;
  constexpr size_t nGrid = 128;
  constexpr size_t nGhost = 1;
  constexpr size_t nSteps = 1024 * 16;
  constexpr T dt = 0.01;

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost, false);

  toolBox->unsetVerbose();

  Field<NDim, T> phi("phi", toolBox);
  Field<NDim, T> pi("pi", toolBox);

  Benchmark bench([&](Benchmark::Measurer &measurer) {
    measurer.measure("x->k fourier", [&]() {
      phi.getMemoryManager()->confirmFourierSpace();
      pi.getMemoryManager()->confirmFourierSpace();
    });

    measurer.measure("initialize field", [&]() {
      phi.inFourierSpace() = RandomGaussianField<NDim, T>("Hoi", toolBox);
      pi.inFourierSpace() = RandomGaussianField<NDim, T>("Hai", toolBox);
    });

    measurer.measure("k->x fourier", [&]() {
      phi.getMemoryManager()->confirmConfigSpace();
      pi.getMemoryManager()->confirmConfigSpace();
    });

    for (size_t i = 0; i < nSteps; ++i) {
      measurer.measure("ghosts", [&]() { pi.updateGhosts(); });
      measurer.measure("timestepping", [&]() {
        pi = pi + LatticeLaplacian<NDim, decltype(phi)>(phi);
        phi = phi + dt * pi;
        Kokkos::fence();
      });
    }
  });
  bench.run(1);
  sayMPI << bench;
  bench.log("phi4");
}