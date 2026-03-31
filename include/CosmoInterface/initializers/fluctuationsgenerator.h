#ifndef COSMOINTERFACE_INITIALIZERS_GAUSSIANFLUCTUATIONSGENERATOR_H
#define COSMOINTERFACE_INITIALIZERS_GAUSSIANFLUCTUATIONSGENERATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/lattice/algebra/complexalgebra/asfourier.h"
#include "TempLat/lattice/algebra/constants/symbols.h"
#include <numbers>

namespace TempLat
{
  /** @brief  A class that contains functions related with initial random fluctuations for scalar fields.
   *
   *  The functions of this class deal with the initial random fluctuations of the scalar singlets.
   *  As described in the documentation, they consist in a sum of left-moving and right-moving waves in momentum space,
   *  whose amplitudes follow a Gaussian random distribution
   **/
  template <typename T> class FluctuationsGenerator
  {
  public:
    // Put public methods here. These should change very little over time.
    template <class Model>
    FluctuationsGenerator(Model &model, T pLSide, std::string pSeed) : lSide(pLSide), baseSeed(pSeed)
    {
    }

    // This function computes the norm of the left-moving and right-moving waves:
    //  --> It's also used to initialize the complex scalars and SU2 doublets
    // (called from u1initializer.h and su2initializer.h).
    template <class Model> auto getFluctuationsNorm(Model &model, Field<T, Model::NDim> f, T mass2, T kCutOff) const
    {
      FourierSite<Model::NDim> ntilde(f.getToolBox());
      // Fourier lattice site, see eq.(57) of arXiv:2006.15122v2
      auto k = ntilde.norm() * f.getKIR();
      // comoving momentum (in program units), obtained by multiplying it by k_IR
      auto omega = omega_k(k, mass2, f.toString());
      // mode frequency

      auto Hcut = heaviside(kCutOff - k) * pow(k,(3. - Model::NDim) / 2.);
      // function that sets to zero all modes over a certain cutoff

      // Returns the rms of the (real and imaginary) parts of the fluctuations
      // (see Sec. 7.1. of arXiv:2006.15122 for a derivation)

      if constexpr (Model::NDim == 1)
        return Hcut * (model.omegaStar / model.fStar * pow(lSide / pow<2>(f.getDx()),.5)) * pow(2 * omega, -0.5) / sqrt(4.*Constants::pi<T>) ;
      else if constexpr (Model::NDim == 2)
          return Hcut * (model.omegaStar / model.fStar * (lSide / pow<2>(f.getDx()))) * pow(2 * omega, -0.5) / sqrt(2.*Constants::pi<T>) ;
      else
          return Hcut  * (model.omegaStar / model.fStar * pow(lSide / pow<2>(f.getDx()), 1.5)) * pow(2 * omega, -0.5) / sqrt(2) ;

      // Here 1/sqrt{2omega_k} characterises rms of |phi_k|, but since |phi_k|^2 =
      //  Re(phi_k)^2 + Im(phi_k)^2, hence there is extra 1/sqrt{2} as this 'return' is
      //  in reality the rms of either Re(phi_k) or Im(phi_k).
    }

    // Returns the amplitude of the (left- or right-moving) waves,
    // which follows a Gaussian distribution:
    template <class Model>
    auto getNormedFluctuations(Model &model, Field<T, Model::NDim> f, T mass2, std::string mySeed, T kCutOff) const
    {
      auto fFluctuationNorm = getFluctuationsNorm(model, f, mass2, kCutOff); // norm
      return fFluctuationNorm * RandomGaussianField<T, Model::NDim>(baseSeed + mySeed + f.toString(),
                                                                    f.getToolBox()); // baseSeed is given in input file
    }

    // Sums left-moving and right-moving waves, both following a Gaussian distribution
    template <class Model> void gaussianFluctuations(Model &model, Field<T, Model::NDim> f, T mass2, T kCutOff) const
    {
      auto fLeft = getNormedFluctuations(model, f, mass2, "Random left", kCutOff);
      // left wave
      auto fRight = getNormedFluctuations(model, f, mass2, "Random right", kCutOff);
      // right wave

      f.inFourierSpace() = (fLeft + fRight) / sqrt(2);
      // the sum (which is also Gaussian) is imposed to the field in momentum space
      // extra 1/sqrt{2} is due to having summed Left and Right movers.
      f.inFourierSpace().setZeroMode(0);
      // sets the zero mode to 0
    }

    // This does the same as the previous function, but also sets fluctuations to the time-derivatives
    template <class Model>
    void conjugateGaussianFluctuations(Model &model, Field<T, Model::NDim> f, Field<T, Model::NDim> p, T mass2, T aDot,
                                       T kCutOff) const
    {
      auto fLeft = getNormedFluctuations(model, f, mass2, "Random left", kCutOff);
      auto fRight = getNormedFluctuations(model, f, mass2, "Random right", kCutOff);

      f.inFourierSpace() = (fLeft + fRight) / sqrt(2);
      f.inFourierSpace().setZeroMode(0); // sets the zero mode to 0

      FourierSite<Model::NDim> ntilde(f.getToolBox()); // Fourier lattice site, see eq.(57) of arXiv:2006.15122v2
      auto k = ntilde.norm() * f.getKIR(); // comoving momentum (in program units), obtained by multiplying it by k_IR
      auto omega = omega_k(k, mass2, f.toString()); // mode frequency (defined below)

      p.inFourierSpace() = Constants::I<T> * omega * (fLeft - fRight) / sqrt(2) -
                           aDot * f.inFourierSpace(); // derived in Sec. 7.1. of arXiv:2006.15122
      p.inFourierSpace().setZeroMode(0);              // sets the zero mode to 0
    }

    std::string getBaseSeed() const { return baseSeed; }

    template <class Model, class VF>
    void planeWaves(Model &model, VF f, VF p, VF tmpF, VF tmpP, T aDot, T kCutOff) const
    {
      ForLoop(i, 1, Model::NDim - 1,
              ForLoop(a, 1, 3, conjugateGaussianFluctuations(model, tmpF(i), tmpP(i), 0.0, aDot, kCutOff);
                      tmpF(i).inFourierSpace().setZeroMode(0); tmpP(i).inFourierSpace().setZeroMode(0);));

      FourierSite<Model::NDim> ntilde(model.getToolBox());
      size_t N = GetNGrid::get(model);

      auto expIK = MakeVector(i, 1, Model::NDim, complexPhase(-2.0 * Constants::pi<T> / N * ntilde(i)));
      auto keffm = MakeVector(i, 1, Model::NDim, 1_c - expIK(i));
      auto keffm2 = Total(i, 1, Model::NDim, norm2(keffm(i)));

      auto e_basis = make_templatvector(0.25, 0.25, std::numbers::sqrt2 / 4.0);

      auto edotk = Total(i, 1, 3, e_basis(i - 1) * keffm(i));

      auto lambda1 = MakeVector(i, 1, 3, e_basis(i - 1) - edotk * (1.0 / keffm2) * conj(keffm(i)));
      auto invNLambda1 = safeDivide(1.0, sqrt(Total(i, 1, Model::NDim, norm2(lambda1(i)))));

      auto lambda2 =
          MakeVector(i, 1, 3, Total(j, 1, 3, Total(k, 1, 3, Symbols::epsilon(i, j, k) * e_basis(j - 1) * keffm(k))));
      auto invNLambda2 = safeDivide(1.0, sqrt(Total(i, 1, Model::NDim, norm2(lambda2(i)))));

      ForLoop(i, 1, Model::NDim,
              f(i).inFourierSpace() = model.fStar / model.omegaStar *
                                      (tmpF(1_c).inFourierSpace() * asFourier(invNLambda1 * lambda1(i)) +
                                       tmpF(2_c).inFourierSpace() * asFourier(invNLambda2 * lambda2(i)));
              p(i).inFourierSpace() = model.fStar / model.omegaStar *
                                      (tmpP(1_c).inFourierSpace() * asFourier(invNLambda1 * lambda1(i)) +
                                       tmpP(2_c).inFourierSpace() * asFourier(invNLambda2 * lambda2(i)));
              f(i).inFourierSpace().setZeroMode(0); p(i).inFourierSpace().setZeroMode(0););
    }

    // Returns initial frequency of the mode in program units (this assumes initial scale factor is 1)
    template <typename Q, typename R> static auto omega_k(const Q &k, const R &m2, std::string fieldName = "")
    {
      if (m2 < 0)
        say << "WARNING: the initial mass squared m2 of " + fieldName +
                   " is negative. For this field, the dispersion relation for all modes is taken to be omega_k = |k|. "
                   "Please think whether this is what you want or not.";
      R meff2 = m2 > 0 ? m2 : 0;
      return pow(k * k + meff2, 0.5);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    T lSide;
    std::string baseSeed;
  };

} // namespace TempLat

#endif
