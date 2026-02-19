/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/algebra/spatialderivatives/normgradientsquare.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/util/ndloop.h"

namespace TempLat
{

  template <size_t NDim> struct NormGradientSquareTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> inline void NormGradientSquareTester<NDim>::Test(TDDAssertion &tdd)
  {
    const device::Idx nGrid = 8, nGhost = 2;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    /* create the random field once, keep in memory. Trade off between RAM use and redundant computations. */
    Field<NDim, double> gaussian("gaussian", toolBox);
    gaussian.inFourierSpace() = 1 * RandomGaussianField<NDim, double>("hoi", toolBox);
    gaussian += 0;

    Field<NDim, double> normGradSq("normGradSq", toolBox);
    normGradSq = Grad2<NDim>(gaussian);
    Field<NDim, double> LatForwardGradNorm2("LatForwardGradNorm2", toolBox);
    LatForwardGradNorm2 = LatForwardGrad<NDim>(gaussian).norm2();

    bool allGood = true;

    // Check element-wise equality between LatForwardGradNorm2 and normGradSq
    auto normGradSq_view = normGradSq.getLocalNDHostView();
    auto LatForwardGradNorm2_view = LatForwardGradNorm2.getLocalNDHostView();

    // Get layout for computing global coordinates (for error reporting)
    auto layout = toolBox->mLayouts.getConfigSpaceLayout();

    NDLoop<NDim>(normGradSq_view, [&](const auto &...indices) {
      device::IdxArray<NDim> local_idx = {indices...};

      const double val_normGradSq = normGradSq_view(indices...);
      const double val_LatForwardGradNorm2 = LatForwardGradNorm2_view(indices...);

      if (std::abs(val_LatForwardGradNorm2 - val_normGradSq) > 1e-14) {
        allGood = false;

        // Compute global coordinates for error message
        device::IdxArray<NDim> global_idx;
        layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

        std::stringstream ss;
        ss << "Mismatch at local (";
        for (device::Idx d = 0; d < NDim; ++d) {
          ss << local_idx[d];
          if (d < (device::Idx)NDim - 1) ss << ", ";
        }
        ss << "), global (";
        for (device::Idx d = 0; d < NDim; ++d) {
          ss << global_idx[d];
          if (d < (device::Idx)NDim - 1) ss << ", ";
        }
        ss << "): LatForwardGradNorm2 = " << val_LatForwardGradNorm2 << ", normGradSq = " << val_normGradSq << "\n";
        sayMPI << ss.str();
      }
    });

    tdd.verify(allGood);
  }

} // namespace TempLat

namespace
{
  // TODO 1-Dim fails due to fourier transform - FIND OUT WHY
  // TempLat::TDDContainer<TempLat::NormGradientSquareTester<1>> test1;
  TempLat::TDDContainer<TempLat::NormGradientSquareTester<2>> test2;
  TempLat::TDDContainer<TempLat::NormGradientSquareTester<3>> test3;
  TempLat::TDDContainer<TempLat::NormGradientSquareTester<4>> test4;
  TempLat::TDDContainer<TempLat::NormGradientSquareTester<5>> test5;
} // namespace
