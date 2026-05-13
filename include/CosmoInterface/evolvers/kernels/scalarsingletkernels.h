#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_SCALARKERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_SCALARKERNELS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/definitions/potential.h"
#include "CosmoInterface/evolvers/kernels/kernelstypes.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
#include "CosmoInterface/definitions/axioncouplings.h"

namespace TempLat
{

  /** @brief A class that computes the kernel for the scalar singlets.
   *
   *
   * Unit test: ctest -R test-scalarkernels
   **/
  class ScalarSingletKernels
  {
  public:
    // Put public methods here. These should change very little over time.
    ScalarSingletKernels() = delete;

    // Equations of motion:
    template <class Model, int N> static auto get(Model &model, Tag<N> n, KernelsTypes::EoM<Model> eom)
    {
      auto tMinust0 = eom.tMinust0;

      return (pow(model.aI, 1 + model.alpha) * LatLapl(model.fldS(n)) -
              pow(model.aI, 3 + model.alpha) *
                  (Potential::derivS(model, n) + IfElse(Model::NonMinimalCouplings::couples(Tag<N>(), Tag<0>()),
                                                        model.xis(n, 0_c) * model.fldS(n) * model.RI, ZeroType())) +
              pow<2>(model.omegaStar) / (model.fStar * Model::MPl) * pow(model.aI, model.alpha - 1) *
                  AxionCouplings::ScalarAxionSource(model, n, tMinust0));
    }

    template <class Model, int N> static auto get_momentum(Model &model, Tag<N> n, KernelsTypes::EoM<Model> eom)
    {
      // Returns momentum for scalar singlets:
      return pow(model.aI, model.alpha - 3) * model.piS(n);
    }

    // Default function returns EoM kernels, for backward compatibility.
    template <class Model, int N> static auto get(Model &model, Tag<N> n)
    {
      return ScalarSingletKernels::get(model, n, KernelsTypes::EoM<Model>());
    }
  };
} // namespace TempLat

#endif
