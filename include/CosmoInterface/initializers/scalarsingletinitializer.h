#ifndef COSMOINTERFACE_INITIALIZERS_SCALARINITIALIZER_H
#define COSMOINTERFACE_INITIALIZERS_SCALARINITIALIZER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/rangeiteration/for_in_range.h"
#include "CosmoInterface/initializers/fluctuationsgenerator.h"
#include "CosmoInterface/initializers/externalpowerspectruminitializer.h"
#include "CosmoInterface/fieldsnumbering.h"

namespace TempLat
{
  MakeException(SICNotImplemented);
  /** @brief A class which is used to initialize the scalar singlets.
   *
   **/
  class ScalarSingletInitializer
  {
  public:
    // Put public methods here. These should change very little over time.
    ScalarSingletInitializer() = delete;

    /**
     * @brief Initializes the scalar singlet fields and their derivatives with Gaussian fluctuations sampled from a
     * known power spectrum or an external input one, according to the model's parameters and the given cutoff scale.
     * The initial homogeneous components are set, respectively, to
     *   - `model.fldS += model.fldS0 / model.fStar;`
     *   - `model.piS += model.piS0 / model.fStar / model.omegaStar;`
     *
     * @param model The model to initialize.
     * @param fg The fluctuations generator to use for creating the initial fluctuations.
     * @param extps The fluctuations generator to use for creating the initial fluctuations from a external input power
     * spectrum.
     * @param PSType Power spectrum type for initialization.
     * @param kCutOff The cutoff scale for the fluctuations.
     */
    template <class Model, typename T>
    static void initializeScalars(Model &model, const FluctuationsGenerator<T> &fg,
                                  const ExternalPowerSpectrumInitializer<T> &extps, RunParameters<T> &rPar)
    {

      auto flagSIC = rPar.SIC;
      if (rPar.SIC == InitialConditionsType::S::Default) flagSIC = InitialConditionsType::S::RandomWithMatter;

      if (flagSIC == InitialConditionsType::S::RandomWithMatter)
        initializeRandomScalar(model, fg, extps, rPar);
      else
        throw(SICNotImplemented("The initial condition provided for scalars is not implemented."));
            
      model.fldS = model.getFluctuationRatio(FieldsNumbering::fldS()) * model.fldS;
      model.piS = model.getFluctuationRatio(FieldsNumbering::piS()) * model.piS;

      // We set the initial homogeneous components of the fields and derivatives.
      // model.fldCS0(i) and model.piCS0(i) are introduced in physical
      // (dimensionful variables), so we transform them to program variables
      // by dividing them by f_* and f_* omega_* respectively.

      model.fldS += model.fldS0 / model.fStar;                  // from the default ones.
      model.piS += model.piS0 / model.fStar / model.omegaStar ;

      }

  private:

    template <class Model, typename T>
    static void initializeRandomScalar(Model &model, const FluctuationsGenerator<T> &fg,
                                       const ExternalPowerSpectrumInitializer<T> &extps, RunParameters<T> &rPar)
    {
      ForLoop(i, 0, Model::Ns - 1, {
        auto &s = model.extPS[i];
        if (s == Constants::defaultString || s.empty() || s == "None" || s == "none") {
          fg.conjugateGaussianFluctuations(model, model.fldS(i), model.piS(i), model.masses2S[i], model.aDotI, rPar.kCutoff);
        } else {
          extps.conjugateGaussianInputFluctuations(model, model.fldS(i), model.piS(i), s, rPar.kCutoff, rPar.powerSpectrumType);
        }
      });
    }

  };
} // namespace TempLat

#endif
