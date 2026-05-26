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
        enum class S {Default, RandomWithMatter, DefectsNetwork, DefectsWhiteNoise};
        enum class U1 {Default, RandomWithMatter, PlaneWaves, PlaneWavesZeroB, BunchDavisTransverseU1, DefectsNetwork, DefectsWhiteNoise};

        std::istream& operator>>(std::istream& in, InitialConditionsType::S& ICType){
            std::string tmp;
            in>>tmp;
            if(tmp=="Default"||tmp=="0") ICType=InitialConditionsType::S::Default;
            else if(tmp=="RandomWithMatter"||tmp=="1") ICType=InitialConditionsType::S::RandomWithMatter;
            else if(tmp=="DefectsNetwork"||tmp=="2") ICType=InitialConditionsType::S::DefectsNetwork;
            else if(tmp=="DefectsWhiteNoise"||tmp=="3") ICType=InitialConditionsType::S::DefectsWhiteNoise;
            else if(tmp.empty()){}
            else throw(NotAnICType(tmp +" is not a valid initial condition for scalar singlet, abort."));
            return in;
        }

        std::istream& operator>>(std::istream& in, InitialConditionsType::U1& ICType){
            std::string tmp;
            in>>tmp;
            if(tmp=="Default"||tmp=="0") ICType=InitialConditionsType::U1::Default;
            else if(tmp=="RandomWithMatter"||tmp=="1") ICType=InitialConditionsType::U1::RandomWithMatter;
            else if(tmp=="PlaneWaves"||tmp=="2") ICType=InitialConditionsType::U1::PlaneWaves;
            else if(tmp=="PlaneWavesZeroB"||tmp=="3") ICType=InitialConditionsType::U1::PlaneWavesZeroB;
            else if(tmp=="BunchDavisTransverseU1"||tmp=="4") ICType=InitialConditionsType::U1::BunchDavisTransverseU1;
            else if(tmp=="DefectsNetwork"||tmp=="5") ICType=InitialConditionsType::U1::DefectsNetwork;
            else if(tmp=="DefectsWhiteNoise"||tmp=="6") ICType=InitialConditionsType::U1::DefectsWhiteNoise;
            else if(tmp.empty()){}
            else throw(NotAnICType(tmp +" is not a valid initial condition for U(1), abort."));
            return in;
        }

        std::ostream& operator<<(std::ostream& out, const InitialConditionsType::S& ICType){
            if(ICType == InitialConditionsType::S::Default) return out << "Default";
            else if(ICType == InitialConditionsType::S::RandomWithMatter) return out << "RandomWithMatter";
            else if(ICType == InitialConditionsType::S::DefectsNetwork) return out << "DefectsNetwork";
            else if(ICType == InitialConditionsType::S::DefectsWhiteNoise) return out << "DefectsWhiteNoise";
            return out;
        }

        std::ostream& operator<<(std::ostream& out, const InitialConditionsType::U1& ICType){
            if(ICType == InitialConditionsType::U1::Default) return out << "Default";
            else if(ICType == InitialConditionsType::U1::RandomWithMatter) return out << "RandomWithMatter";
            else if(ICType == InitialConditionsType::U1::PlaneWaves) return out << "PlaneWaves";
            else if(ICType == InitialConditionsType::U1::PlaneWavesZeroB) return out << "PlaneWavesZeroB";
            else if(ICType == InitialConditionsType::U1::BunchDavisTransverseU1) return out << "BunchDavisTransverseU1";
            else if(ICType == InitialConditionsType::U1::DefectsNetwork) return out << "DefectsNetwork";
            else if(ICType == InitialConditionsType::U1::DefectsWhiteNoise) return out << "DefectsWhiteNoise";
            return out;
        }

        std::string to_string(const InitialConditionsType::S& ICType){
            if(ICType == InitialConditionsType::S::Default) return "Default";
            else if(ICType == InitialConditionsType::S::RandomWithMatter) return "RandomWithMatter";
            else if(ICType == InitialConditionsType::S::DefectsNetwork) return "DefectsNetwork";
            else if(ICType == InitialConditionsType::S::DefectsWhiteNoise) return "DefectsWhiteNoise";
            else return "";
        }

        std::string to_string(const InitialConditionsType::U1& ICType){
            if(ICType == InitialConditionsType::U1::Default) return "Default";
            else if(ICType == InitialConditionsType::U1::RandomWithMatter) return "RandomWithMatter";
            else if(ICType == InitialConditionsType::U1::PlaneWavesZeroB) return "PlaneWavesZeroB";
            else if(ICType == InitialConditionsType::U1::PlaneWaves) return "PlaneWaves";
            else if(ICType == InitialConditionsType::U1::BunchDavisTransverseU1) return "BunchDavisTransverseU1";
            else if(ICType == InitialConditionsType::U1::DefectsNetwork) return "DefectsNetwork";
            else if(ICType == InitialConditionsType::U1::DefectsWhiteNoise) return "DefectsWhiteNoise";
            else return "";
        }

    }
} /* TempLat */

#endif
