#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_SU2DOUBLETKERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_SU2DOUBLETKERNELS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/definitions/potential.h"
#include "CosmoInterface/definitions/gaugederivatives.h"
#include "CosmoInterface/evolvers/kernels/kernelstypes.h"

namespace TempLat
{
  /** @brief A class which holds the kernels for the SU2Doublet.
   *
   *
   * Unit test: ctest -R test-su2doubletkernels
   **/
  class SU2DoubletKernels
  {
  public:
    // Put public methods here. These should change very little over time.
    SU2DoubletKernels() = delete;

    // Equations of motion:
    template <class Model, int N> static auto get(Model &model, Tag<N> n, KernelsTypes::EoM<Model> eom)
    {
      // Returns kernel for SU2 doublets (formed by covariant laplacian and potential derivative):
      return pow(model.aI, 1 + model.alpha) * GaugeDerivatives::covLaplacianSU2Doublet(model, n) -
             pow(model.aI, 3 + model.alpha) / 2 * Potential::derivSU2Doublet(model, n);
    }

    template <class Model, int N> static auto get_momentum(Model &model, Tag<N> n, KernelsTypes::EoM<Model> eom)
    {
      // Returns momentum for SU2 doublets:
      return pow(model.aI, model.alpha - 3) * model.piSU2Doublet(n);
    }

    // Default function returns EoM kernels, for backward compatibility.
    template <class Model, int N> static auto get(Model &model, Tag<N> n)
    {
      return SU2DoubletKernels::get(model, n, KernelsTypes::EoM<Model>());
    }
  };
} // namespace TempLat

#endif
