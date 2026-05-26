#ifndef COSMOINTERFACE_INITIALIZERS_MODELINITIALIZER_H
#define COSMOINTERFACE_INITIALIZERS_MODELINITIALIZER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/initializers/fluctuationsgenerator.h"
#include "CosmoInterface/initializers/externalpowerspectruminitializer.h"
#include "CosmoInterface/initializers/scalefactorinitializer.h"
#include "CosmoInterface/initializers/scalarsingletinitializer.h"
#include "CosmoInterface/initializers/gwsinitializer.h"
#include "CosmoInterface/initializers/u1initializer.h"
#include "CosmoInterface/initializers/su2initializer.h"
#include "CosmoInterface/definitions/averages.h"
#include "CosmoInterface/definitions/hubbleconstraint.h"
#include "CosmoInterface/definitions/nonminimalcoupling.h"

namespace TempLat
{
  /** @brief A class initializes a model's fields and the scale factor.
   *
   * On calling initialize(), the model is set up with the appropriate initial conditions
   * for all fields and parameters.
   *
   **/
  template <typename T> class ModelInitializer
  {
  public:
    // Put public methods here. These should change very little over time.
    template <class Model> ModelInitializer(Model &model, T pLSide, std::string pSeed) : 
    fg(model, pLSide, pSeed),
    extps(model, pLSide, pSeed)  
    {}

    /**
     * @brief Initializes the model's fields and scale factor, according to the RunParameters and the model's field
     * content.
     *
     * @param model The model to initialize.
     * @param rPar The run parameters, which determine the initial conditions for the fields and scale factor.
     */
    template <class Model> void initialize(Model &model, RunParameters<T> &rPar, ExtraFields<Model> extraFields)
    {
      // Initialize scale factor:
      if (rPar.expansion) ScaleFactorInitializer::initializeScaleFactor(model, rPar);

      // Initialize scalar singlets:
      if constexpr (Model::Ns > 0) ScalarSingletInitializer::initializeScalars(model, fg, extps, rPar);

      // Initialize GWs:
      if (model.fldGWs != nullptr) GWsInitializer::initializeGWs(model);

      // Initialize the SU2 gauge fields and SU2 doublets:
      //  --> Note: It is important to initialize SU2 first, as the doublet contributes to the U1 currents.
      if constexpr (Model::NSU2Doublet > 0) SU2Initializer::initializeSU2(model, fg, rPar.kCutoff);

      // Initialize the U1 gauge fields and complex scalars:
      if constexpr (Model::NCs > 0 || Model::NU1 > 0)
        U1Initializer::initializeU1(model, fg, extps, rPar, extraFields);

      Averages::setAllAverages(model);
      if constexpr (Model::IsNonMinimallyCoupled) model.RI = NonMinimalCoupling::R(model);
      if (rPar.expansion) {
        // For consistency, correct the scale factor time-derivative with the fluctuations.
        // Relevant only for higher order evolvers.
        auto hubbleLaw = HubbleConstraint::get(model);
        model.aDotI = sqrt(hubbleLaw[2]);
        if constexpr (Model::IsNonMinimallyCoupled) model.piAI = model.aDotI * pow(model.aI, 1 - model.alpha);
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    FluctuationsGenerator<T> fg;
    ExternalPowerSpectrumInitializer<T> extps;

  };
} // namespace TempLat

#endif
