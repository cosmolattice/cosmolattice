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
  //Helper to get NScalars from CouplingsManager
  template <typename CM> struct GetNScalars { static constexpr size_t nScalars = 0; };

  template <size_t NScalars, size_t NU1, bool Coupled>
  struct GetNScalars<CouplingsManager<NScalars, NU1, Coupled>> {
    static constexpr size_t nScalars = NScalars;
  };
  template <typename T, typename SCALARU1AXIONCOUPLINGS> class ScalarU1AxionBase
  {
  public:
    using ScalarU1AxionCouplings = typename SCALARU1AXIONCOUPLINGS::template Container<T>;
    static constexpr bool IsAxionU1Coupled = SCALARU1AXIONCOUPLINGS::howManyCouples() > 0;

    // Currently, the code is restricted to an ALP coupled to a single U(1) gauge field, as generalizations involving an additional axion with a different coupling parameter have not been tested. 
    // If, as a user, you wish to remove this restriction in order to explore these untested configurations, you can comment out this line at your own risk.
    static_assert(!(IsAxionU1Coupled && GetNScalars<SCALARU1AXIONCOUPLINGS>::nScalars > 1),
                  "ERROR: The Axion-U(1) coupling currently only supports exactly 1 scalar field (ALP). Compilation aborted.");

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
