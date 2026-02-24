#include "TempLat/util/tdd/tdd.h"
#include "TempLat/session/sessionguard.h"
#include "TempLat/util/benchmark.h"

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"

int main(int argc, char **argv)
{
  using namespace TempLat;

  SessionGuard guard(argc, argv, false);

  constexpr size_t NDim = 2;
  using T = double;
  constexpr size_t nGrid = 4;
  constexpr size_t nGhost = 1;
  constexpr size_t nSteps = 10;
  constexpr T dt = 0.01;

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost, false);

  toolBox->unsetVerbose();

  Field<NDim, T> phi("phi", toolBox);
  Field<NDim, T> pi("pi", toolBox);

  SpatialCoordinate x(toolBox);

  phi = getVectorComponent(x, 0);

  auto layout = toolBox->mLayouts.getConfigSpaceLayout();

  phi.getMemoryManager()->confirmFourierSpace();
  pi.getMemoryManager()->confirmFourierSpace();
  phi.getMemoryManager()->confirmConfigSpace();
  pi.getMemoryManager()->confirmConfigSpace();

  for (size_t i = 0; i < nSteps; ++i) {
    std::cout << "Step " << i << ":\n";

    pi = pi + dt * LatticeLaplacian<NDim, decltype(phi)>(phi) * dt;
    phi = phi + dt * pi;

    // phi.getMemoryManager()->confirmFFTConfigSpace();
    // pi.getMemoryManager()->confirmFFTConfigSpace();
    // phi.getMemoryManager()->confirmConfigSpace();
    // pi.getMemoryManager()->confirmConfigSpace();

    // phi.getMemoryManager()->confirmFourierSpace();
    // pi.getMemoryManager()->confirmFourierSpace();
    // phi.getMemoryManager()->confirmConfigSpace();
    // pi.getMemoryManager()->confirmConfigSpace();

    device::iteration::fence();

    std::stringstream ss;
    ss << "Field values:\n ";

    // phi.updateGhosts();
    // pi.updateGhosts();
    for (device::Idx j = 0; j < layout.getLocalSizes()[0] + 2 * nGhost; ++j) {
      for (device::Idx k = 0; k < layout.getLocalSizes()[1] + 2 * nGhost; ++k) {
        ss << std::setw(7) << std::setprecision(3) << device::memory::getAtOnePoint(phi, device::IdxArray<NDim>{j, k})
           << std::setw(3) << " ";
      }
      ss << "\n";
    }
    ss << "\n";
    sayMPI << ss.str();
  }
}
