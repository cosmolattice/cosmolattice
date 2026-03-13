#ifndef COSMOINTERFACE_INITIALIZERS_INITIALCONDITIONSTYPE_H
#define COSMOINTERFACE_INITIALIZERS_INITIALCONDITIONSTYPE_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Year: 2025

#include "TempLat/util/exception.h"

namespace TempLat {


    /** \brief A namespace that collects different types to specify different initial conditions,
     *
     *
     * Unit test: make test-initialconditionstype
     **/

    MakeException(NotAnICType);

    namespace  InitialConditionsType {
        enum U1 {RandomWithMatter, PlaneWaves, PlaneWavesZeroB, BunchDavisTransverseU1};

    }

    std::istream& operator>>(std::istream& in, InitialConditionsType::U1& ICType){
        std::string tmp;
        in>>tmp;
        if(tmp=="RandomWithMatter"||tmp=="0") ICType=InitialConditionsType::RandomWithMatter;
        else if(tmp=="PlaneWaves"||tmp=="1") ICType=InitialConditionsType::PlaneWaves;
        else if(tmp=="PlaneWavesZeroB"||tmp=="2") ICType=InitialConditionsType::PlaneWavesZeroB;
        else if(tmp=="BunchDavisTransverseU1"||tmp=="3") ICType=InitialConditionsType::BunchDavisTransverseU1;
        else if(tmp.empty()){}
        else throw(NotAnICType(tmp +" is not a valid initial condition for U(1), abort."));
        return in;
    }

    std::string to_string(InitialConditionsType::U1&  ICType){

        if(ICType == InitialConditionsType::RandomWithMatter) return "RandomWithMatter";
        else if(ICType == InitialConditionsType::PlaneWavesZeroB) return "PlaneWavesZeroB";
        else if(ICType == InitialConditionsType::PlaneWaves) return "PlaneWaves";
        else if(ICType == InitialConditionsType::BunchDavisTransverseU1) return "BunchDavisTransverseU1";
        else return "";
    }


} /* TempLat */

#endif
