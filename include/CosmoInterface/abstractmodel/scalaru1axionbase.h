#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SCALARU1AXIONBASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SCALARU1AXIONBASE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "CosmoInterface/couplingsmanager.h"
#include "TempLat/parameters/parameterparser.h"

namespace TempLat {

template <typename T, typename SCALARU1AXIONCOUPLINGS>
class ScalarU1AxionBase {
public:
    using ScalarU1AxionCouplings = SCALARU1AXIONCOUPLINGS;

    SCALARU1AXIONCOUPLINGS alphaLambda_SU1;
    T InverseAxionLambda;

protected:
    ScalarU1AxionBase(ParameterParser &parser)
    {
        auto gAxionU1 = parser.get<double, SCALARU1AXIONCOUPLINGS::nGauge>("gAxionU1", 1.0);
        auto AxionU1Charges = parser.get<double, SCALARU1AXIONCOUPLINGS::howManyCouples()>("alphaLambda_AxionU1", 1);
        alphaLambda_SU1.setEffectiveCharges(AxionU1Charges, gAxionU1);
    }
};

} // namespace TempLat

#endif
