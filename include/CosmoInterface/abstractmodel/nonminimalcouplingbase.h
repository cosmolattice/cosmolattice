#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_NONMINIMALCOUPLINGBASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_NONMINIMALCOUPLINGBASE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "TempLat/util/templatarray.h"
#include "CosmoInterface/couplingsmanager.h"
#include "TempLat/parameters/parameterparser.h"

namespace TempLat
{

  template <typename T, size_t NS, typename NONMINCOUPLINGS> class NonMinimalCouplingBase
  {
  public:
    using NonMinimalCouplings = typename NONMINCOUPLINGS::template Container<T>;
    static constexpr bool IsNonMinimallyCoupled = NONMINCOUPLINGS::howManyCouples() > 0;

    NonMinimalCouplings xis;

    // Averages needed for non-minimal coupling
    T RI;
    TempLatArray<T, NS> fld2AvSI_i;
    TempLatArray<T, NS> grad2AvSI_i;
    TempLatArray<T, NS> pi2AvSI_i;
    TempLatArray<T, NS> fldPiAvSI;
    TempLatArray<T, NS> fldVpAvSI;

    T piAI;

  protected:
    NonMinimalCouplingBase(ParameterParser &parser)
    {
      auto xiCouplings = parser.get<T, NonMinimalCouplings::howManyCouples()>("xis", 1);
      xis.setEffectiveCharges(xiCouplings, {1});
    }
  };

} // namespace TempLat

#endif
