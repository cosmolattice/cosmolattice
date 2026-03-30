#ifndef TEMPLAT_COSMOINTERFACE_POWERSPECTRUMGWS_H
#define TEMPLAT_COSMOINTERFACE_POWERSPECTRUMGWS_H

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
  MakeException(WrongPRJType);
  /** @brief A class which computes the power spectrum, with the appropriate rescaling to make it volume independent.
   *
   *
   **/
  template <typename T> class GWPowerSpectrumMeasurer
  {
  public:
    GWPowerSpectrumMeasurer(const RunParameters<T> &par):
    PSVersion(par.powerSpectrumVersion),
    nbins(par.nBinsSpectra),
    PSType(par.powerSpectrumType),
    PRJType(par.GWprojectorType)
    {
    }

    template <typename Model>
    auto powerSpectrumGW(Model& model)
    {
      // return  (*this).powerSpectrumGW(gwsprojectortype1(model), GetNGrid::get(model), model.kIR, model);
      if (PRJType == 1)
      {
        return  pow<2>(model.fStar / Constants::reducedMPlanck<T>) / (4 * pow(model.aI, 6) * Energies::rho(model)) * (*this).powerSpectrumGW(projectGWType1(model), GetNGrid::get(model), model.kIR, model);
      }
      else if (PRJType == 2)
      {
        return  pow<2>(model.fStar / Constants::reducedMPlanck<T>) / (4 * pow(model.aI, 6) * Energies::rho(model)) * (*this).powerSpectrumGW(projectGWType2(model), GetNGrid::get(model), model.kIR, model);
      }
      else if (PRJType == 3)
      {
        return  pow<2>(model.fStar / Constants::reducedMPlanck<T>) / (4 * pow(model.aI, 6) * Energies::rho(model)) * (*this).powerSpectrumGW(projectGWType3(model), GetNGrid::get(model), model.kIR, model);
      }

      throw(WrongPRJType("You tried to call an undefined GR Projector Type " + std::to_string(PRJType) + ", abort."));
      return (*this).powerSpectrumGW(projectGWType1(model), GetNGrid::get(model), model.kIR, model);
    }

    // This function computes the power spectrum.
    // --> The normalization factor ensures that it recovers the appropriate expression in the continuum limit.
    //     This is discussed in Sect. 3 of arXiv:2006.15122.

    template <typename PS, typename Model>
    RadialProjectionResult<T> powerSpectrumGW(PS f, ptrdiff_t N, T kIR, const Model& model)
    {
      const ptrdiff_t N3 = pow<3>(N);
      const T dx = 2 * Constants::pi<T> / kIR / N; // lattice spacing

      auto toolbox = model.getToolBox();

      const T kMaxBins = std::floor(sqrt(3.) / 2.0 * N) + 1;

      if (PSVersion != 3) {
        auto fk2 = projectRadially<3>(f, SpaceStateType::Fourier, toolbox, PSVersion == 1)
                       .measure(nbins, kMaxBins); // PSversion == true is a boolean. It tells the spectrum to use the
                                                  // centralValues, and not the bins, when rescaling.

        if (PSType == 2) {
          return Function(ntilde, pow<3>(kIR * ntilde * dx) / N3 / T(2) / pow<2>(Constants::pi<T>))  * fk2;
        } else if (PSType == 1) {
          fk2.sumInsteadOfAverage();
          return Function(ntilde, kIR * ntilde * dx / pow<5>(N) / T(2) / Constants::pi<T>) * fk2;
        } else if (PSType == 0) {
          fk2.sumInsteadOfAverage();
          return Function(ntilde, dx / pow<5>(N) / T(2) / Constants::pi<T>) * fk2;
        } else {
          throw(WrongPSType("You tried to call an undefined PSType " + std::to_string(PSType) + ", abort."));
          return fk2; // To remove moot warning.
        }
      } else {
        WaveNumber ntilde(toolbox); // WaveNumber is the same as FourierSite, except it does not require to know
                                           // the dimension at compile time.
        if (PSType == 2) {

          auto fk2 = projectRadially<3>(pow<3>(ntilde.norm()) * f, SpaceStateType::Fourier, toolbox, false)
                         .measure(nbins, kMaxBins);
          return (pow<3>(kIR * dx) / N3 / T(2) / pow<2>(Constants::pi<T>)) * fk2;
        } else if (PSType == 1) {
          auto fk2 =
              projectRadially<3>(ntilde.norm() * f, SpaceStateType::Fourier, toolbox, false).measure(nbins, kMaxBins);
          fk2.sumInsteadOfAverage();
          return (kIR * dx / pow<5>(N) / T(2) / Constants::pi<T>) * fk2;
        } else if (PSType == 0) {
          auto fk2 = projectRadially<3>(f, SpaceStateType::Fourier, toolbox, false).measure(nbins, kMaxBins);
          fk2.sumInsteadOfAverage();
          return (dx / pow<5>(N) / T(2) / Constants::pi<T>) * fk2;
        } else {
          throw(WrongPSType("You tried to call an undefined PSType " + std::to_string(PSType) + ", abort."));
          return projectRadially<3>(f, SpaceStateType::Fourier, toolbox, false)
              .measure(nbins, kMaxBins); // To remove moot warning.
        }
      }
    }

    int PSVersion;
  private:
    ptrdiff_t nbins;
    int PSType;
    int PRJType;
  };

} // namespace TempLat

#endif
