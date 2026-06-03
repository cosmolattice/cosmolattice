#ifndef COSMOINTERFACE_EVOLVERS_POSITIONVERLET_H
#define COSMOINTERFACE_EVOLVERS_POSITIONVERLET_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti, Nicolas Loayza  Year: 2026

#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/listoperators/listoperators.h"
#include "TempLat/lattice/algebra/su2algebra/su2algebra.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/lattice/measuringtools/averager.h"
#include "TempLat/lattice/algebra/gaugealgebra/u1exponential.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldshift.h"
#include "CosmoInterface/evolvers/positionverletparameters.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/evolvers/kernels/kernels.h"
#include "CosmoInterface/definitions/fixedbackgroundexpansion.h"
#include "CosmoInterface/definitions/energies.h"
#include "CosmoInterface/definitions/averages.h"
namespace TempLat
{
  /** @brief A class which implements a position Verlet algorithm that evolves scalar singlets, complex scalars, SU2
   * doublets, and U(1) and SU(2) gauge fields.
   *
   *
   **/
  template <typename T = double> class PositionVerlet
  {
  public:
    // Put public methods here. These should change very little over time.

    template <class Model>
    PositionVerlet(Model &model, RunParameters<T> &rPar)
        : expansion(rPar.expansion), fixedBackground(rPar.fixedBackground), // A boolean. If true, expansion is fixed
          aBackground(model, rPar),                                                                         // Used for fixed background expansion
          ws(PositionVerletParameters<T>::getCoeffs(
              rPar.eType)), // Vector containing the numerical parameters of the chosen PV method
          order(PositionVerletParameters<T>::getOrder(rPar.eType)) // Order of the PV algorithm
    {
    }

    template <class Model> void evolve(Model &model, T tMinust0, bool evolveGWs)
    {
      /*
       * Position Verlet can be understood as moving fields by half step, momenta by a step and fields by half a step
       * again.
       *
       * 1. phi_1/2 = phi_0 + dt/2 * pi_0  [DRIFT]
       * 2. pi_1 = pi_0 + dt * kernel_1/2  [KICK]
       * 3. phi_1 = phi_1/2 + dt/2 * pi_1  [DRIFT]
       *
       * */

      T w;
      T tcurrent = tMinust0;
      size_t parSize = ws.size(); // Number of distinct parameters (1 for PV2, 2 for PV4, 4 for PV6...)

      size_t stages = (parSize - 1) * 2 + 1; // Number of operations in each iteration

      for (size_t i = 0; i < stages; ++i) { // loop over operations...

        w = ws[(i < parSize ? i : stages - i - 1)];

        // We start by computing the first drifts (phi_0 --> phi_1/2):

        if constexpr (Model::Ns > 0) driftScalar(model, w);
        if (model.fldGWs != nullptr && evolveGWs) driftGWs(model, w);
        if constexpr (Model::NCs > 0) driftCS(model, w);
        if constexpr (Model::NSU2Doublet > 0) driftSU2Doublet(model, w);
        if constexpr (Model::NU1 > 0) driftU1Vector(model,w);
        if constexpr (Model::NSU2 > 0) driftSU2Vector(model,w);

        if (expansion) driftScaleFactorFirstHalf(model, tcurrent, w);
        
        if (expansion&& !fixedBackground) storeFieldsAveragesHalf(model);

        // Now we compute the kicks (pi_0 --> pi_1):

        if (model.fldGWs != nullptr && evolveGWs) kickGWs(model,w);
        if constexpr (Model::Ns > 0) kickScalar(model,w);
        if constexpr (Model::NCs > 0) kickCS(model,w);
        if constexpr (Model::NSU2Doublet > 0) kickSU2Doublet(model,w);
        if constexpr (Model::NU1 > 0) kickU1Vector(model,w);
        if constexpr (Model::NSU2 > 0) kickSU2Vector(model,w);

        if (expansion && !fixedBackground) {
          storeMomentaAverages(model);
          kickScaleFactor(model, w);
        }

        // Now we compute the second drifts (phi_1/2 --> phi_1):
        if (expansion) driftScaleFactorSecondHalf(model, tcurrent, w);

        if constexpr (Model::Ns > 0) driftScalar(model, w);
        if (model.fldGWs != nullptr && evolveGWs) driftGWs(model, w);
        if constexpr (Model::NCs > 0) driftCS(model, w);
        if constexpr (Model::NSU2Doublet > 0) driftSU2Doublet(model, w);
        if constexpr (Model::NU1 > 0) driftU1Vector(model, w);
        if constexpr (Model::NSU2 > 0) driftSU2Vector(model, w);

        if (expansion && !fixedBackground) storeFieldsAveragesInt(model);

        tcurrent += model.dt * w;
      }
    }

    // This function is called before doing the measurements. It is used only to set aDotI to its correct value in case
    // of a background expansion.
    template <class Model> void sync(Model &model, T tMinust0) { 

       if (fixedBackground) model.aDotI = aBackground.dot(tMinust0);
       
    }

     /********
     * KICKS
     *********/

    // Evolves piS(i)
    template <class Model> void kickScalar(Model &model, T w)
    {
      ForLoop(i, 0, Model::Ns - 1, model.piS(i) += (w * model.dt) * ScalarSingletKernels::get(model, i););
    }

    template <class Model> void kickGWs(Model &model, T w)
    {
      (*model.piGWs) += (w * model.dt) * GWsKernels::get(model);
    }

    // Evolves piCS(n)
    template <class Model> void kickCS(Model &model, T w)
    {
      ForLoop(n, 0, Model::NCs - 1, model.piCS(n) += model.dt * w * ComplexScalarKernels::get(model, n););
    }

    // Evolves piSU2Doublet(n)
    template <class Model> void kickSU2Doublet(Model &model, T w)
    {
      ForLoop(n, 0, Model::NSU2Doublet - 1,
              model.piSU2Doublet(n) += model.dt * w * (SU2DoubletKernels::get(model, n)););
    }

    // Evolves piU1(a)
    template <class Model> void kickU1Vector(Model &model, T w)
    {
      ForLoop(a, 0, Model::NU1 - 1, model.piU1(a) += model.dt * w * U1Kernels::get(model, a));
    }

    // Evolves piSU2(n)
    template <class Model> void kickSU2Vector(Model &model, T w)
    {
      ForLoop(n, 0, Model::NSU2 - 1, model.piSU2(n) += model.dt * w * SU2Kernels::get(model, n););
    }

    // Evolves aDotI
    template <class Model> void kickScaleFactor(Model &model, T w)
    {
      model.aDotI += model.dt * ScaleFactorKernels::getSI(model) * w;
    }

    /********
     * DRIFTS
     *********/

    // Evolves fldS
    template <class Model> void driftScalar(Model &model, T w)
    {
      model.fldS += pow(model.aI, model.alpha - 3) * (model.dt/2.0 * w * model.piS);
    }

    // Evolves fldGWs
    template <class Model> void driftGWs(Model &model, T w)
    {
      (*model.fldGWs) += pow(model.aI, model.alpha - 3) * (model.dt/2.0 * w * (*model.piGWs));
    }

    // Evolves fldCS
    template <class Model> void driftCS(Model &model, T w)
    {
      model.fldCS += pow(model.aI, model.alpha - 3) * model.dt/2.0 * w * model.piCS;
    }

    // Evolves fldSU2Doublet
    template <class Model> void driftSU2Doublet(Model &model, T w)
    {
      model.fldSU2Doublet += pow(model.aI, model.alpha - 3) * model.dt/2.0 * w * model.piSU2Doublet;
    }

    // Evolves fldU1
    template <class Model> void driftU1Vector(Model &model, T w)
    {
      model.fldU1 += pow(model.aI, model.alpha - 1) * w * model.dt/2.0 * model.piU1;
    }

    // Evolves fldSU2
    template <class Model> void driftSU2Vector(Model &model, T w)
    {
      ForLoop(n, 0, Model::NSU2 - 1,
              auto rescaledPi = MakeVector(
                  i, 1, Model::NDim,
                  exp(w * pow(model.aI, model.alpha - 1) * model.dx * model.dt/2.0 * model.gQ_SU2DblSU2(0_c, n) *
                      model.piSU2(n)(i))); // The 0_c is correct. In our convention, the link is normalized wrt the
                                           // first doublet charge.

              model.fldSU2(n) = rescaledPi * model.fldSU2(n););
    }

    // Prepares the first half-step scale-factor drift. 
    template <class Model> void driftScaleFactorFirstHalf(Model &model, T tMinust0, T w)
    {
      model.aIM = model.aI;
      if (fixedBackground) { // if fixed background, the scale factor is given by the power-law function in
                             // fixedbackgroundexpansion.h
          model.aI = aBackground(tMinust0 + w * model.dt / 2.0);
          model.aSI = model.aI;
          if constexpr (Model::IsNonMinimallyCoupled) model.RI = aBackground.R(tMinust0  + w * model.dt / 2.0);
      } else { // if self-consistent expansion, the scale factor is evolved with the VV algorithm
          model.aI += model.dt / 2.0 * model.aDotI * w;
          model.aSI = model.aI;
      }
    }

    // Prepares the second half-step scale-factor drift. 
    template <class Model> void driftScaleFactorSecondHalf(Model &model, T tMinust0, T w)
    {
      if (fixedBackground) {
        model.aI = aBackground(tMinust0 + w * model.dt);
        if constexpr (Model::IsNonMinimallyCoupled) model.RI = aBackground.R(tMinust0  + w * model.dt);
      } else {
        model.aI += model.dt / 2.0 * model.aDotI * w;
        model.aSI = (model.aIM + model.aI) / 2.0;
      }
    }

    /********
     * FUNCTIONS STORING VOLUME AVERAGES OF COMPOSITE FIELDS AND MOMENTA at different times
     *********/

    template <class Model> void storeMomentaAverages(Model &model)
    {
      model.pi2AvIM = model.pi2AvI;
      model.CSpi2AvIM = model.CSpi2AvI;
      model.SU2DblPi2AvIM = model.SU2DblPi2AvI;
      model.U1pi2AvIM = model.U1pi2AvI;
      model.SU2pi2AvIM = model.SU2pi2AvI;

      if constexpr (Model::Ns > 0) model.pi2AvI = Averages::pi2S(model);
      if constexpr (Model::NCs > 0) model.CSpi2AvI = Averages::pi2CS(model);
      if constexpr (Model::NSU2Doublet > 0) model.SU2DblPi2AvI = Averages::pi2SU2Doublet(model);
      if constexpr (Model::NU1 > 0) model.U1pi2AvI = Averages::pi2U1(model);
      if constexpr (Model::NSU2 > 0) model.SU2pi2AvI = Averages::pi2SU2(model);

       model.pi2AvSI = 0.5*(model.pi2AvI+model.pi2AvIM);
       model.CSpi2AvSI = 0.5*(model.CSpi2AvI+model.CSpi2AvIM);
       model.SU2DblPi2AvSI = 0.5*(model.SU2DblPi2AvI+model.SU2DblPi2AvIM);
       model.U1pi2AvSI = 0.5*(model.U1pi2AvI+model.U1pi2AvIM);
       model.SU2pi2AvSI = 0.5*(model.SU2pi2AvI+model.SU2pi2AvIM);
    }

    template <class Model> void storeFieldsAveragesHalf(Model &model)
    {
      if constexpr (Model::Ns > 0) model.grad2AvSI = Averages::grad2S(model);
      if constexpr (Model::NCs > 0) model.CSgrad2AvSI = Averages::grad2CS(model);
      if constexpr (Model::NSU2Doublet > 0) model.SU2DblGrad2AvSI = Averages::grad2SU2Doublet(model);
      if constexpr (Model::NU1 > 0) model.U1Mag2AvSI = Averages::B2U1(model);
      if constexpr (Model::NSU2 > 0) model.SU2Mag2AvSI = Averages::B2SU2(model);
      model.potAvSI = average(Potential::potential(model));
    }


    template <class Model> void storeFieldsAveragesInt(Model &model)
    {
      if constexpr (Model::Ns > 0) model.grad2AvI = Averages::grad2S(model);
      if constexpr (Model::NCs > 0) model.CSgrad2AvI = Averages::grad2CS(model);
      if constexpr (Model::NSU2Doublet > 0) model.SU2DblGrad2AvI = Averages::grad2SU2Doublet(model);
      if constexpr (Model::NU1 > 0) model.U1Mag2AvI = Averages::B2U1(model);
      if constexpr (Model::NSU2 > 0) model.SU2Mag2AvI = Averages::B2SU2(model);
      model.potAvI = average(Potential::potential(model));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    const bool expansion;
    bool fixedBackground;

    FixedBackgroundExpansion<T> aBackground;

    const std::vector<T> ws;
    size_t order;
  };
} // namespace TempLat

#endif
