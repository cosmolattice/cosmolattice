#ifndef TEMPLAT_UTIL_CONSTANTS_H
#define TEMPLAT_UTIL_CONSTANTS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include <complex>

namespace TempLat {



    /** \brief A class which
     * Class to store your constant. Some basic constants here, inherit from here and make a new class
     * in your project if you want more.
     *
     *
     * Unit test: make test-constants
     **/
    namespace Constants {

        template<typename T=double>
        constexpr T pi = T(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303820);

        //constexpr static T reducedMPlanck = 2.435363e18; //GEV2, PDG value.
        template<typename T=double>
        constexpr T reducedMPlanck = T(2.435e18); // Agreement between PDG and CODATA

        template<typename T=double>
        constexpr std::complex<T> I = std::complex<T>(0,1);

        const std::string defaultString = "\u2716";

        // For convenience, redundantly define planck mass again, in float and double. can use whatever you want that way

        constexpr double MPl = 2.435e18;
        constexpr float fMPl = 2.435e18;


    };

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/constants_test.h"
#endif


#endif
