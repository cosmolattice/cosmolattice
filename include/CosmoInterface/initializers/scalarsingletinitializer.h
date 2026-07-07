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
     * @param rPar User parameters, so that they can be used for the different initialization types.
     */
    template <class Model, typename T>
    static void initializeScalars(Model &model, const FluctuationsGenerator<T> &fg,
                                  const ExternalPowerSpectrumInitializer<T> &extps, RunParameters<T> &rPar)
    {

      auto flagSIC = rPar.SIC;
      if (rPar.SIC == InitialConditionsType::S::Default) flagSIC = InitialConditionsType::S::RandomWithMatter;

        if (flagSIC == InitialConditionsType::S::RandomWithMatter) {
          initializeRandomScalar(model, fg, extps, rPar, rPar.SIC == InitialConditionsType::S::Default);
          model.fldS = model.getFluctuationRatio(FieldsNumbering::fldS()) * model.fldS;
          model.piS = model.getFluctuationRatio(FieldsNumbering::piS()) * model.piS;
        }
        else if (flagSIC == InitialConditionsType::S::DefectsNetwork)
          initializeScalarDefectsNetwork(model, fg, rPar.lcorr);
        else if (flagSIC == InitialConditionsType::S::DefectsWhiteNoise)
          initializeScalarDomainWallWhiteNoise(model, fg, rPar.kCutoff, rPar.deltaNoise);
        else if (flagSIC != InitialConditionsType::S::Homogeneous)
          throw(SICNotImplemented("The initial condition provided for scalars is not implemented."));


      // We set the initial homogeneous components of the fields and derivatives.
      // model.fldCS0(i) and model.piCS0(i) are introduced in physical
      // (dimensionful variables), so we transform them to program variables
      // by dividing them by f_* and f_* omega_* respectively.

      // @label:default_scalar_homogeneous_modes
      model.fldS += model.fldS0 / model.fStar;                  // from the default ones.
      model.piS += model.piS0 / model.fStar / model.omegaStar ;
      // @endlabel

    }

  private:

    template <class Model, typename T>
    static void initializeRandomScalar(Model &model, const FluctuationsGenerator<T> &fg,
                                       const ExternalPowerSpectrumInitializer<T> &extps, RunParameters<T> &rPar,
                                       bool useAxionU1HomogeneousDefault)
    {
      ForLoop(i, 0, Model::Ns - 1, {
        if (!(useAxionU1HomogeneousDefault && isAxionU1CoupledScalar<Model>(i))) {
          auto &s = model.extPS[i];
          if (s == Constants::defaultString || s.empty() || s == "None" || s == "none") {
            // @label:default_scalar_dispatch
            fg.conjugateGaussianFluctuations(model, model.fldS(i), model.piS(i), model.masses2S[i], model.aDotI,
                                             rPar.kCutoff);
            // @endlabel
          } else {
            // @label:externalps_scalar_dispatch
            extps.conjugateGaussianInputFluctuations(model, model.fldS(i), model.piS(i), s, rPar.kCutoff,
                                                     rPar.powerSpectrumType);
            // @endlabel
          }
        }
      });
    }

    template <class Model, int N> static bool isAxionU1CoupledScalar(Tag<N> n)
    {
      if constexpr (Model::IsAxionU1Coupled) {
        bool isCoupled = false;
        ForLoop(a, 0, Model::NU1 - 1, isCoupled = isCoupled || Model::ScalarU1AxionCouplings::couples(n, a););
        return isCoupled;
      } else {
        return false;
      }
    }

    template <class Model, typename T>
    static void initializeScalarDefectsNetwork(Model &model, const FluctuationsGenerator<T> &fg, T lcorr)
    {
      auto toolBox = model.getToolBox();
      using RGF = RandomGaussianField<T,Model::NDim>;

      ForLoop(i, 0, Model::Ns - 1,

              // 1. Norm of both real and imaginarry components:
              auto fFluctuationNormDefect =  fg.getFluctuationsNormDefect(model,model.fldS(i),lcorr); // component 0

              // 2. To obtain the amplitude at each point,multiply by a random Gaussian
              auto a0 = fFluctuationNormDefect * RGF(fg.getBaseSeed() + "norm0" + model.fldS(i).toString(),toolBox); // component 0

              model.fldS(i).inFourierSpace() = a0 / sqrt(Model::Ns); // component 0
              model.fldS(i).inFourierSpace().setZeroMode(0);
              model.piS(i) =  0;
      );
    }

    template <class Model, typename T>
    static void initializeScalarDomainWallWhiteNoise(Model &model, const FluctuationsGenerator<T> &fg, T kCutoff, T delta)
    {
      auto toolBox = model.getToolBox();
      using RGF = RandomGaussianField<T,Model::NDim>;

      ForLoop(i, 0, Model::Ns - 1,

              // 1. Norm of both real and imaginarry components:
              auto fFluctuationNormDefect =  fg.getFluctuationsNormWhiteNoise(model, model.fldS(i), kCutoff, delta); // component 0

              // 2. To obtain the amplitude at each point, multiply by a random Gaussian
              auto a0 = fFluctuationNormDefect * RGF(fg.getBaseSeed() + "norm0" + model.fldS(i).toString(),toolBox); // component 0

              model.fldS(i).inFourierSpace() = a0 / sqrt(Model::Ns); // component 0
              model.fldS(i).inFourierSpace().setZeroMode(0);
              model.piS(i) =  0;
      );
    }

  };
} // namespace TempLat

#endif
