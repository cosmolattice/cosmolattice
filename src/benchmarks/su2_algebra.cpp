#include "TempLat/util/tdd/tdd.h"
#include "TempLat/session/sessionguard.h"
#include "TempLat/util/benchmark.h"

#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/lattice/algebra/gaugealgebra/plaquette.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/lattice/field/collections/vectorfield.h"

int main(int argc, char **argv)
{
  using namespace TempLat;

  SessionGuard guard(argc, argv, false);

  constexpr size_t NDim = 3;
  using T = double;
  constexpr size_t nGrid = 64;
  constexpr size_t nGhost = 1;
  constexpr size_t nSteps = 200;
  constexpr T dt = 0.01;

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost, false);

  toolBox->unsetVerbose();

  FieldCollection<SU2Field<NDim, T>, T, NDim> U1("SU2Field1", toolBox);
  SU2Field<NDim, T> result("SU2Field2", toolBox);

  U1(0_c)(1_c).inFourierSpace() = RandomGaussianField<NDim, T>("H1", toolBox);
  U1(0_c)(2_c).inFourierSpace() = RandomGaussianField<NDim, T>("H2", toolBox);
  U1(0_c)(3_c).inFourierSpace() = RandomGaussianField<NDim, T>("H3", toolBox);
  U1(1_c)(1_c).inFourierSpace() = RandomGaussianField<NDim, T>("H4", toolBox);
  U1(1_c)(2_c).inFourierSpace() = RandomGaussianField<NDim, T>("H5", toolBox);
  U1(1_c)(3_c).inFourierSpace() = RandomGaussianField<NDim, T>("H6", toolBox);
  U1(2_c)(1_c).inFourierSpace() = RandomGaussianField<NDim, T>("H7", toolBox);
  U1(2_c)(2_c).inFourierSpace() = RandomGaussianField<NDim, T>("H8", toolBox);
  U1(2_c)(3_c).inFourierSpace() = RandomGaussianField<NDim, T>("H9", toolBox);

  device::iteration::fence();

  double dummy = 0;

  Benchmark bench([&](Benchmark::Measurer &measurer) {
    for (size_t i = 0; i < nSteps; ++i) {
      measurer.measure("plaquette eval", [&]() {
        plaq(U1, 1_c, 2_c).SU2Get(0_c).eval(2, 3, 4);
        device::iteration::fence();
      });
      measurer.measure("plaquette get", [&]() {
        dummy = plaq(U1, 1_c, 2_c).SU2Get(0_c).eval(2, 3, 4);
        device::iteration::fence();
      });
      measurer.measure("plaquette assign", [&]() {
        result = plaq(U1, 1_c, 2_c);
        device::iteration::fence();
      });
    }
  });
  bench.run(1);
  sayMPI << bench;
  bench.log("phi4");
}
