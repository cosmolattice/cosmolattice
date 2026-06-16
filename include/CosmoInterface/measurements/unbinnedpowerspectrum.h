#ifndef TEMPLAT_COSMOINTERFACE_UNBINNEDPOWERSPECTRUM_H
#define TEMPLAT_COSMOINTERFACE_UNBINNEDPOWERSPECTRUM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "TempLat/util/function.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/algebra.h"
#include "TempLat/lattice/measuringtools/radialprojector.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/definitions/energies.h"
#include "CosmoInterface/definitions/gwsprojector.h"
#include "CosmoInterface/definitions/chiralpowerspectrum.h"

namespace TempLat
{

  /** @brief A class which computes the power spectrum, with the appropriate rescaling to make it volume independent.
   *
   *
   **/
  template <typename T, size_t NDim> class UnbinnedPowerSpectrumMeasurer
  {
  public:

    UnbinnedPowerSpectrumMeasurer(const RunParameters<T> &par):
    PSType(par.powerSpectrumType)
    {
      if constexpr (NDim == 1 || NDim == 2) {
        if (PSType != 2) throw(WrongPSType("Power spectra for dimension 1 and 2 is only implemented for PSType = 2. Abort."));
      }
    }

    template <typename R>
    requires requires(R f) {
      f.inFourierSpace();
    }
    auto powerSpectrum(R f)
    {
      return this->powerSpectrum(pow<2>(abs(f.inFourierSpace())), GetNGrid::get(f), GetKIR::getKIR(f), GetToolBox::get(f));
    }

    template <typename Model>
    auto powerSpectrumGW(Model& model, size_t PRJType)
    {
      if constexpr (Model::NDim != 3) return (*this).powerSpectrum(pow<2>(abs(model.getOneField().inFourierSpace())), GetNGrid::get(model), model.kIR, model.getToolBox());
      else {
        if (PRJType == 1)
        {
          return  pow<2>(model.fStar / Constants::reducedMPlanck<T>) / (4 * pow(model.aI, 6) * Energies::rho(model)) * (*this).powerSpectrum(projectGWType1(model), GetNGrid::get(model), model.kIR, model.getToolBox());
        }
        else if (PRJType == 2)
        {
          return  pow<2>(model.fStar / Constants::reducedMPlanck<T>) / (4 * pow(model.aI, 6) * Energies::rho(model)) * (*this).powerSpectrum(projectGWType2(model), GetNGrid::get(model), model.kIR, model.getToolBox());
        }
        else if (PRJType == 3)
        {
          return  pow<2>(model.fStar / Constants::reducedMPlanck<T>) / (4 * pow(model.aI, 6) * Energies::rho(model)) * (*this).powerSpectrum(projectGWType3(model), GetNGrid::get(model), model.kIR, model.getToolBox());
        }

        throw(WrongPRJType("You tried to call an undefined GR Projector Type " + std::to_string(PRJType) + ", abort."));
        return (*this).powerSpectrum(projectGWType1(model), GetNGrid::get(model), model.kIR, model.getToolBox());
      }
    }
    
    template <typename Model, int U1, int C>
    auto chiralPowerSpectrumU1(Model& model, Tag<U1> u1, Tag<C> c, bool sign, bool AorE)
    {
      return this->powerSpectrum(projectChiralU1Type1(model, u1, c, sign, AorE), GetNGrid::get(model), model.kIR, model.getToolBox());
    }

    // This function computes the power spectrum.
    // --> The normalization factor ensures that it recovers the appropriate expression in the continuum limit.
    //     This is discussed in Sect. 3 of arXiv:2006.15122.
    template <typename PS, typename tBox> UnbinnedRadialProjectionResult<T> powerSpectrum(const PS& f, ptrdiff_t N, T kIR, tBox toolBox)
    {

      const T dx = 2 * Constants::pi<T> / kIR / N; // lattice spacing

      auto fk2 = projectRadially<NDim>(f, SpaceStateType::Fourier, toolBox).measureUnbinned(N);

      if (PSType == 3) return fk2;

      if (PSType == 2) {
        if constexpr (NDim == 1) {
          return Function(ntilde, pow<NDim>(kIR * ntilde * dx / N) / Constants::pi<T>) * fk2;
        }
        else if constexpr (NDim == 2) {
          return Function(ntilde, pow<NDim>(kIR * ntilde * dx / N) / T(2) / Constants::pi<T>) * fk2;
        }
        else {
          return Function(ntilde, pow<NDim>(kIR * ntilde * dx / N) / T(2) / pow<2>(Constants::pi<T>)) * fk2;
        }
      }

      fk2.renormalizeBins();
      fk2.sumInsteadOfAverage();
      if (PSType == 1) {
        return Function(ntilde, kIR * ntilde * dx / pow<2 * NDim - 1>(N) / T(2) / Constants::pi<T>) * fk2;
      } else if (PSType == 0) {
        return Function(ntilde, dx / pow<2 * NDim - 1>(N) / T(2) / Constants::pi<T>) * fk2;
      } else {
        throw(WrongPSType("You tried to call an undefined PSType " + std::to_string(PSType) + ", abort."));
      }
      return fk2; // To remove moot warning.

    }

    template <typename R> auto powerSpectrum(R f)
    { // This function is for expression/composite operator, which need their own memory to perform the fourier
      // transform.
      Field<T, NDim> tmp("tmp", GetToolBox::get(f));
      tmp = f;

      return this->powerSpectrum(pow<2>(abs(tmp.inFourierSpace())), GetNGrid::get(tmp), GetToolBox::get(tmp));
    }

  private:
    int PSType;

  };

} // namespace TempLat

#endif
