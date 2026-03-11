#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SCALEFACTORBASE_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_SCALEFACTORBASE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "TempLat/util/constants.h"

namespace TempLat {

template <typename T>
class ScaleFactorBase {
public:
    static constexpr T MPl = Constants::reducedMPlanck<T>;

    T aSI, aI, aIM;
    T aDotSI, aDotSIM, aDotI, aDotIM;
    T b0;

protected:
    ScaleFactorBase() : aSI(1), aI(1), aIM(1) {}
};

} // namespace TempLat

#endif
