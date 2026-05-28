#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_GWSKERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_GWSKERNELS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros, Adrien Florio, Nicolás Layza,  Year: 2022

#include "CosmoInterface/definitions/potential.h"
#include "CosmoInterface/definitions/PITensor.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"

namespace TempLat
{

  /** @brief A class which stores the kernel for the GWs fields.
   *
   *
   * Unit test: ctest -R test-gwskernels
   **/
  class GWsKernels
  {
  public:
    // Put public methods here. These should change very little over time.
    GWsKernels() = delete;

    template <class Model> static auto get(Model &model)
    {
      return pow(model.aI, 1 + model.alpha) * GaugeDerivatives::LaplacianGWs(model) +
             pow(model.aI, 1 + model.alpha) * 2. * (PITensor::effectiveAnisotropicTensor(model));
    }
  };
} // namespace TempLat

#endif
