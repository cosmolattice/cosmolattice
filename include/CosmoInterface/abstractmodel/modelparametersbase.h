#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_MODELPARAMETERSBASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_MODELPARAMETERSBASE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include <string>
#include "CosmoInterface/runparameters.h"

namespace TempLat
{

  template <typename T> class ModelParametersBase
  {
  public:
    T dx, kIR, dt;
    std::string name;
    T t0, t;
    T alpha, fStar, omegaStar;

    std::string extraInfoFn(int verbosity) { return verbosity > 0 ? name + "_" : ""; }

  protected:
    ModelParametersBase(const LatticeParameters<T> &par, T pDt, std::string pName)
        : dx(par.getDx()), kIR(par.getKIR()), dt(pDt), name(std::move(pName)),
          // Initialize time variables
          t0(0), t(0),
          // And model parameters
          alpha(1), fStar(1), omegaStar(1)
    {
    }
  };

} // namespace TempLat

#endif
