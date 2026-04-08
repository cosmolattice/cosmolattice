#ifndef COSMOINTERFACE_INITIALIZERS_EXTERNALPOWERSPECTRUMINITIALIZER_H
#define COSMOINTERFACE_INITIALIZERS_EXTERNALPOWERSPECTRUMINITIALIZER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti, Ander Urio and Nicolas Loayza
// Year: 2025

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/lattice/algebra/complexalgebra/asfourier.h"
#include "TempLat/lattice/algebra/constants/symbols.h"
#include "TempLat/lattice/algebra/adapter/momentuminterpolator.h"

#include "TempLat/lattice/algebra/coordinates/momentummultiplicity.h"
#include "CosmoInterface/definitions/phaseBunchDavies.h"

namespace TempLat
{

  MakeException(PSTypeINotSupportedForNDIMDifferentFrom3);

  /** @brief  A class that contains functions related with initial random fluctuations for scalar fields given by an
   * external power spectrum.
   *
   *  The functions of this class deal with the initial random fluctuations of the scalar singlets.
   *  As described in the documentation, they consist in a sum of left-moving and right-moving waves in momentum space,
   *  whose amplitudes follow a Gaussian random distribution with variance given by and external power spectrum
   * function.
   */
  template <typename T> class ExternalPowerSpectrumInitializer
  {
  public:
    /* Put public methods here. These should change very little over time. */
    template <class Model>
    ExternalPowerSpectrumInitializer(Model &model, T pLSide, std::string pSeed) : lSide(pLSide), baseSeed(pSeed)
    {
    }

    template <class Model>
    void readSpectrumFile(Model &model, const std::string &str, std::vector<T> &kIN, std::vector<T> &PSamp,
                          std::vector<T> &PSampConj) const
    {
      int lineCounter = 0;
      std::ifstream t;
      t.open(str);
      std::string tmp;

      // This just to count number of lines of the input file
      if (t.good()) {
        while (getline(t, tmp))
          lineCounter++;
      }
      t.close();

      kIN = std::vector<T>(lineCounter);
      PSamp = std::vector<T>(lineCounter);
      PSampConj = std::vector<T>(lineCounter);

      t.open(str);
      if (t.good()) {
        for (int j = 0; j < lineCounter; j++) {
          t >> kIN[j];
          kIN[j] /= model.omegaStar; // Now is in omegaStar units, i.e. it's kappa = k/omegaStar
          t >> PSamp[j];
          PSamp[j] *= model.omegaStar; // Now is in 1/omegaStar units, i.e. \tilde{P} = omegaStar * P(k), e.g. P(k) =
                                       // 1/(2\omega_k)
          t >> PSampConj[j];
          PSampConj[j] /= model.omegaStar; // Now is in 1/omegaStar units, i.e. \tilde{P} = omegaStar * P(k), e.g. P(k)
                                           // = 1/(2\omega_k)
        }
      }
      t.close();
    }

    template <class Model, size_t NDim>
    auto getInputFluctuationsNormTypeI(const MomentumInterpolator<T, NDim> &PSinterp, Model &model, Field<T, NDim> f,
                                       T kCutOff) const
    {
      if (NDim != 3)
        throw(PSTypeINotSupportedForNDIMDifferentFrom3("Initialization with PSType = 1 is only supported for NDim=3."));

      FourierSite<NDim> ntilde(f.getToolBox());
      auto k = ntilde.norm() * f.getKIR();
      auto Hcut = heaviside(kCutOff - k);

      auto invmult = MomentumMultiplicity<T, NDim>(f.getToolBox());

      return Hcut * (model.omegaStar / model.fStar) * (T(1) / pow(model.aI, T(1.5))) *
             pow(lSide / pow<2>(f.getDx()), T(1.5)) *
             safeSqrt(T(4) * Constants::pi<T> * pow(ntilde.norm(), T(2)) * invmult * PSinterp / T(2));
    }

    template <class Model, size_t NDim>
    auto getInputFluctuationsNormTypeII(const MomentumInterpolator<T, NDim> &PSinterp, Model &model, Field<T, NDim> f,
                                        T kCutOff) const
    {
      FourierSite<NDim> ntilde(f.getToolBox());
      auto k = ntilde.norm() * f.getKIR();
      auto Hcut = heaviside(kCutOff - k);

      return Hcut * (model.omegaStar / model.fStar) * (T(1) / pow(model.aI, T(1.5))) *
             pow(lSide / pow<2>(f.getDx()), T(1.5)) * safeSqrt(PSinterp) / sqrt(T(2));
    }

    template <class Model, size_t NDim>
    auto getInputFluctuationsTypeI(const MomentumInterpolator<T, NDim> &PSinterp, Model &model, Field<T, NDim> f,
                                   const std::string &mySeed, T kCutOff) const
    {
      auto norm = getInputFluctuationsNormTypeI<Model, NDim>(PSinterp, model, f, kCutOff);
      return norm * RandomGaussianField<T, NDim>(baseSeed + mySeed + f.toString(), f.getToolBox());
    }

    template <class Model, size_t NDim>
    auto getInputFluctuationsTypeII(const MomentumInterpolator<T, NDim> &PSinterp, Model &model, Field<T, NDim> f,
                                    const std::string &mySeed, T kCutOff) const
    {
      auto norm = getInputFluctuationsNormTypeII<Model, NDim>(PSinterp, model, f, kCutOff);
      return norm * RandomGaussianField<T, NDim>(baseSeed + mySeed + f.toString(), f.getToolBox());
    }

    template <class Model, size_t NDim>
    void conjugateGaussianInputFluctuations(Model &model, Field<T, NDim> f, Field<T, NDim> p, const std::string &str,
                                            T kCutOff, int PSType) const
    {
      std::vector<T> kIN, PSfld, PSmom;
      readSpectrumFile(model, str, kIN, PSfld, PSmom);

      MomentumInterpolator<T, NDim> PSinterpFld(kIN, PSfld, f.getToolBox(), f.getKIR());
      MomentumInterpolator<T, NDim> PSinterpMom(kIN, PSmom, f.getToolBox(), f.getKIR());

      if (PSType == 1) {
        auto fLeft = getInputFluctuationsTypeI<Model, NDim>(PSinterpFld, model, f, "Random left", kCutOff);
        auto fRight = getInputFluctuationsTypeI<Model, NDim>(PSinterpFld, model, f, "Random right", kCutOff);

        f.inFourierSpace() = (fLeft + fRight) / sqrt(T(2));
        f.inFourierSpace().setZeroMode(0);

        auto pLeft = getInputFluctuationsTypeI<Model, NDim>(PSinterpMom, model, p, "Random left", kCutOff);
        auto pRight = getInputFluctuationsTypeI<Model, NDim>(PSinterpMom, model, p, "Random right", kCutOff);

        p.inFourierSpace() = pow(model.aI, model.alpha) * (pLeft + pRight) / sqrt(T(2));
        p.inFourierSpace().setZeroMode(0);
      } else {
        auto fLeft = getInputFluctuationsTypeII<Model, NDim>(PSinterpFld, model, f, "Random left", kCutOff);
        auto fRight = getInputFluctuationsTypeII<Model, NDim>(PSinterpFld, model, f, "Random right", kCutOff);

        f.inFourierSpace() = (fLeft + fRight) / sqrt(T(2));
        f.inFourierSpace().setZeroMode(0);

        auto pLeft = getInputFluctuationsTypeII<Model, NDim>(PSinterpMom, model, p, "Random left", kCutOff);
        auto pRight = getInputFluctuationsTypeII<Model, NDim>(PSinterpMom, model, p, "Random right", kCutOff);

        p.inFourierSpace() = pow(model.aI, model.alpha) * (pLeft + pRight) / sqrt(T(2));
        p.inFourierSpace().setZeroMode(0);
      }
    }

    template <class Model, size_t NDim>
    auto getInputFluctuationsNormTypeI(Model &model, Field<T, NDim> f, T kCutOff) const
    {
      if (NDim != 3)
        throw(PSTypeINotSupportedForNDIMDifferentFrom3("Initialization with PSType = 1 is only supported for NDim=3."));

      FourierSite<NDim> ntilde(f.getToolBox());
      auto k = ntilde.norm() * f.getKIR();
      auto Hcut = heaviside(kCutOff - k);

      auto invmult = MomentumMultiplicity<T, NDim>(f.getToolBox());

      return Hcut * safeSqrt(T(4) * Constants::pi<T> * pow(ntilde.norm(), T(2)) * invmult) *
             (T(1) / pow(model.aI, T(1.5))) * pow(lSide / pow<2>(f.getDx()), T(1.5)) * sqrt(0.5);
    }

    template <class Model, size_t NDim>
    auto getInputFluctuationsTypeI(Model &model, Field<T, NDim> f, const std::string &mySeed, T kCutOff) const
    {
      auto norm = getInputFluctuationsNormTypeI<Model, NDim>(model, f, kCutOff);
      return norm * RandomGaussianField<T, NDim>(baseSeed + mySeed + f.toString(), f.getToolBox());
    }

    template <class Model, size_t NDim>
    auto ConjugateGaussianFluctuationsVarianceOne(Model &model, Field<T, NDim> f, Field<T, NDim> p, T kCutOff) const
    {
      auto fLeft = getInputFluctuationsTypeI<Model, NDim>(model, f, "Random left", kCutOff);
      auto fRight = getInputFluctuationsTypeI<Model, NDim>(model, f, "Random right", kCutOff);

      f.inFourierSpace() = (fLeft + fRight) / sqrt(T(2));
      f.inFourierSpace().setZeroMode(0);

      auto pLeft = getInputFluctuationsTypeI<Model, NDim>(model, p, "Random left", kCutOff);
      auto pRight = getInputFluctuationsTypeI<Model, NDim>(model, p, "Random right", kCutOff);

      p.inFourierSpace() = (pLeft + pRight) / sqrt(T(2));
      p.inFourierSpace().setZeroMode(0);
    }

    template <class Model, class VF>
    void BunchDavisTransverseU1(Model &model, VF f, VF p, VF tmpF, VF tmpP, T aDot, T kCutOff) const
    {

      size_t N = GetNGrid::get(model); // Reads N (number of points per dimension)
      FourierSite<Model::NDim> ntilde(model.getToolBox());
      auto knorm = ntilde.norm() * model.fldU1(0_c)(1_c).getKIR();

      auto keffm = MakeVector(i, 1, Model::NDim, sin(Constants::pi<T> / N * ntilde(i)));
      auto keffm2 = Total(i, 1, Model::NDim, pow<2>(keffm(i)));

      // definition of the phase shifts to properly have gauge fields living on the links when doing fourier
      // transformations
      auto extraphaseminus = MakeVector(i, 1, Model::NDim, complexPhase(-1.0 * Constants::pi<T> / N * ntilde(i)));
      auto extraphase = MakeVector(i, 1, Model::NDim, complexPhase(1.0 * Constants::pi<T> / N * ntilde(i)));

      /* Initialization of Gauge Field */

      // We set the RGF fluctuations with variance = 1 for the gauge and electric field
      ForLoop(i, 1, Model::NDim, ConjugateGaussianFluctuationsVarianceOne(model, f(i), p(i), kCutOff););

      ForLoop(k, 1, Model::NDim, f(k).inFourierSpace() = asFourier(extraphaseminus(k)) * f(k).inFourierSpace();
              f(k).inFourierSpace().setZeroMode(0);
              p(k).inFourierSpace() = asFourier(extraphaseminus(k)) * p(k).inFourierSpace();
              p(k).inFourierSpace().setZeroMode(0););
      // Saves the transverse projected vector field in a auxiliar field
      ForLoop(k, 1, Model::NDim,
              tmpF(k).inFourierSpace() = Total(
                  j, 1, Model::NDim, ((k == j ? T(1) : T(0)) - keffm(k) * keffm(j) / keffm2) * f(j).inFourierSpace());
              tmpP(k).inFourierSpace() = Total(
                  j, 1, Model::NDim, ((k == j ? T(1) : T(0)) - keffm(k) * keffm(j) / keffm2) * p(j).inFourierSpace()););

      auto phaseA = BDPhasePi2A<T, Model::NDim>(model.getToolBox(), model.kIR, model.aDotI);
      auto phaseE = BDPhasePi2E<T, Model::NDim>(model.getToolBox(), model.kIR, model.aDotI);

      /* Initialization of Gauge Field */
      // Sets the purely transverse initial vector field with its Bunch-Davis amplitude and appropiate phase
      //
      ForLoop(k, 1, Model::NDim, f(k).inFourierSpace() = sqrt(1. / (2. * knorm)) * phaseA * tmpF(k).inFourierSpace();
              f(k).inFourierSpace() = asFourier(extraphase(k)) * f(k).inFourierSpace();
              f(k).inFourierSpace().setZeroMode(0);

              // f(k).inFourierSpace() = heaviside(kCutOff - knorm)*f(k).inFourierSpace();
      );

      /* Initialization of Electric Field */
      //
      ForLoop(k, 1, Model::NDim, p(k).inFourierSpace() = sqrt(knorm / (2.0)) * phaseE * tmpP(k).inFourierSpace();
              p(k).inFourierSpace() = asFourier(extraphase(k)) * p(k).inFourierSpace();
              p(k).inFourierSpace().setZeroMode(0);

              // p(k).inFourierSpace() = heaviside(kCutOff - knorm)*p(k).inFourierSpace();
      );
    }

    std::string getBaseSeed() const { return baseSeed; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    T lSide;
    std::string baseSeed;
  };
} // namespace TempLat

#endif
