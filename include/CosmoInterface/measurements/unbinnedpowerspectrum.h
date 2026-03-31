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

namespace TempLat
{

  /** @brief A class which computes the power spectrum, with the appropriate rescaling to make it volume independent.
   *
   *
   **/
  template <typename T, size_t NDim> class UnbinnedPowerSpectrumMeasurer
  {
  public:
    UnbinnedPowerSpectrumMeasurer() = default;

    template <typename R> auto powerSpectrum(R f)
    {
      return this->powerSpectrum(pow<2>(abs(f.inFourierSpace())), GetNGrid::get(f), GetKIR::getKIR(f), GetToolBox::get(f));
    }

    template <typename Model>
    auto powerSpectrumGW(Model& model, size_t PRJType)
    {
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

    // This function computes the power spectrum.
    // --> The normalization factor ensures that it recovers the appropriate expression in the continuum limit.
    //     This is discussed in Sect. 3 of arXiv:2006.15122.

    template <typename PS, typename tBox> UnbinnedRadialProjectionResult<T> powerSpectrum(const PS& f, ptrdiff_t N, T kIR, tBox toolBox)
    {
      return projectRadially<NDim>(f, SpaceStateType::Fourier, toolBox).measureUnbinned();
    }

    template <typename R> auto powerSpectrum(R f, ptrdiff_t N, T kIR)
    { // This function is for expression/composite operator, which need their own memory to perform the fourier
      // transform.
      Field<T, NDim> tmp("tmp", GetToolBox::get(f));
      tmp = f;

      return this->powerSpectrum(tmp, N, kIR);
    }

  };

} // namespace TempLat

#endif
