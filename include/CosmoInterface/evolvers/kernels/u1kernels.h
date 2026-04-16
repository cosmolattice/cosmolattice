#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_U1KERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_U1KERNELS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/definitions/mattercurrents.h"
#include "CosmoInterface/evolvers/kernels/kernelstypes.h"
#include "TempLat/lattice/algebra/spatialderivatives/backdiff.h"
#include "TempLat/lattice/algebra/spatialderivatives/forwdiff.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
#include "CosmoInterface/definitions/axioncouplings.h"

namespace TempLat
{
  /** @brief A class that computes the kernel of the U(1) gauge fields.
   *
   *
   * Unit test: ctest -R test-u1kernels
   **/
  class U1Kernels
  {
  public:
    // Put public methods here. These should change very little over time.
    U1Kernels() = default;

    // Equations of motion:
    template <class Model, int N> static auto get(Model &model, Tag<N> a, KernelsTypes::EoM<Model> eom)
    {
      auto tMinust0 = eom.tMinust0;

      auto U1Source = MatterCurrents::U1Current(model, a);

      // --> lapl(A_i)
      auto LaplU1 = MakeVector(i, 1, Model::NDim, LatLapl(model.fldU1(a)(i)));

      // --> \partial_i \partial_j A_i
      auto GradU1 =
          MakeVector(i, 1, Model::NDim, Total(j, 1, Model::NDim, backDiff(forwDiff(model.fldU1(a)(j), i), j)));

      auto AxionScalarSource = AxionCouplings::U1AxionCoupling(model, a, tMinust0);
      auto normU1AxionScalarSource = (model.fStar / Model::MPl);

      auto normU1Source = pow(model.aI, 1 + model.alpha);
      auto normGrad = pow(model.aI, -1 + model.alpha);

      return normGrad * (LaplU1 - GradU1) - normU1Source * U1Source + normU1AxionScalarSource * AxionScalarSource;
    }

    template <class Model, int N> static auto get_momentum(Model &model, Tag<N> a, KernelsTypes::EoM<Model> eom)
    {
      // Computes U(1) momentum:
      return pow(model.aI, model.alpha - 1) * model.piU1(a);
    }

    // Default function returns EoM kernels, for backward compatibility.
    template <class Model, int N> static auto get(Model &model, Tag<N> a)
    {
      return U1Kernels::get(model, a, KernelsTypes::EoM<Model>());
    }
  };
} // namespace TempLat

#endif
