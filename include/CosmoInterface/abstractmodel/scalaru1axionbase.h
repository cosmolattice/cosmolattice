#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SCALARU1AXIONBASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SCALARU1AXIONBASE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "CosmoInterface/couplingsmanager.h"
#include "TempLat/parameters/parameterparser.h"

namespace TempLat
{
  template <typename T, typename SCALARU1AXIONCOUPLINGS> class ScalarU1AxionBase
  {
  public:
    using ScalarU1AxionCouplings = typename SCALARU1AXIONCOUPLINGS::template Container<T>;
    static constexpr bool IsAxionU1Coupled = SCALARU1AXIONCOUPLINGS::howManyCouples() > 0;
    
    ScalarU1AxionCouplings alphaLambda_SU1;
    // Time to switch from linear to non-linear evolution for AxionU1 coupling
    T tNonLinearAxionU1;

  protected:
    ScalarU1AxionBase(ParameterParser &parser)
    {
      tNonLinearAxionU1 = parser.get<double>("tNonLinearAxionU1", -1.0);
      auto gAxionU1 = parser.get<double, ScalarU1AxionCouplings::nGauge>("gAxionU1", 1.0);
      auto AxionU1Charges = parser.get<double, ScalarU1AxionCouplings::howManyCouples()>("alphaLambda_AxionU1", 1);
      alphaLambda_SU1.setEffectiveCharges(AxionU1Charges, gAxionU1);
    }
  };

} // namespace TempLat

#endif
