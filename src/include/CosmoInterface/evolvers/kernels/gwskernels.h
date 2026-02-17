#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_GWSKERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_GWSKERNELS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros, Adrien Florio, Nicolás Layza,  Year: 2022

#include "TempLat/util/tdd/tdd.h"
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

    template <class Model, int N> static auto get(Model &model, Tag<N> n)
    {
      return (pow(model.aI, 1 + model.alpha) * LatLapl<Model::NDim>((*model.fldGWs)(n)) +
              pow(model.aI, 1 + model.alpha) * 2. * (PITensor::totalTensor(model, n)));
    }

#ifdef TEMPLATTEST
  public:
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
