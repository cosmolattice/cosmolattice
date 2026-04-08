#ifndef COSMOINTERFACE_MEASUREMENTS_OCCUPATIONNUMBER_H
#define COSMOINTERFACE_MEASUREMENTS_OCCUPATIONNUMBER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/function.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/measuringtools/averager.h"
#include "TempLat/lattice/measuringtools/radialprojector.h"

#include "CosmoInterface/runparameters.h"

namespace TempLat
{
  /** @brief A class which computes the occupation number.
   *
   *
   **/
  class OccupationNumberMeasurer
  {
  public:
    template <typename T> OccupationNumberMeasurer(const RunParameters<T> &par) : deltakBin(par.deltaKBin) {}

    template <class Model, int I> auto occupationNumber(Model &model, Tag<I> i)
    {
      constexpr size_t NDim = Model::NDim;
      using T = decltype(model.dx);

      const auto N = GetNGrid::get(model); // Isotropic lattices only.
      const T kMax = pow(NDim, 0.5) / 2.0 * N;

      Field<T, NDim> tmp("tmp", GetToolBox::get(model));

      tmp = model.fldS(i);
      const auto part1 = projectRadiallyFourier(pow<2>(abs(tmp.inFourierSpace()))).measure(kMax);

      tmp = (pow(model.aI, model.alpha - 3) * model.piS(i)) + model.aDotI / model.aI * model.fldS(i);
      const auto part2 = projectRadiallyFourier(pow<2>(abs(tmp.inFourierSpace()))).measure(kMax);

      const T normalisation = pow<2>(model.aI) * pow<NDim>(model.dx / N) / 2 * pow<2>(model.fStar / model.omegaStar);

      const T m2 = average(model.potDeriv2(i));

      const auto omegaK = Function(k, sqrt(pow<2>(model.aI) * m2 + pow<2>(k)));

      return (Function(k, normalisation * omegaK(k)) * part1) +
             (Function(k, normalisation * pow(model.aI, 2.0 * (1.0 - model.alpha)) / omegaK(k)) * part2);
    }

  private:
    [[maybe_unused]] double deltakBin;
  };

} // namespace TempLat

#endif
