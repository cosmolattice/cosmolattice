#ifndef TEMPLAT_COSMOINTERFACE_POWERSPECTRUM_H
#define TEMPLAT_COSMOINTERFACE_POWERSPECTRUM_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "TempLat/util/function.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/field/field.h"

namespace TempLat {


    /** \brief A class which computes the power spectrum, with the appropriate rescaling to make it volume independent.
     *
     *
     **/

    template< typename R>
    auto powerSpectrum(R f) {
        return powerSpectrum(f, GetNGrid::get(f), GetKIR::getKIR(f));
    }

    // This function computes the power spectrum.
    // --> The normalization factor ensures that it recovers the appropriate expression in the continuum limit. 
    //     This is discussed in Sect. 3 of arXiv:2006.15122.
    
    template<typename T>
    auto powerSpectrum(Field<T> f, ptrdiff_t N, T kIR) {
        ptrdiff_t N3 = pow<3>(N);
        T dx = 2 * Constants::pi<T> / kIR / N;  // lattice spacing

        auto fk2 = projectRadiallyFourier(pow<2>(abs(f.inFourierSpace()))).measure();

        return Function(ntilde, pow<3>(kIR * ntilde * dx ) / N3 / 2.0 / pow<2>(Constants::pi<T>)) * fk2;
    }

    template<typename R, typename T>
    auto powerSpectrum(R f, ptrdiff_t N, T kIR) { //This function is for expression/composite operator, which need their own memory to perform the fourier transform.
        Field<T> tmp("tmp", GetToolBox::get(f));
        tmp = f;

        return powerSpectrum(tmp, N , kIR);
    }


    class PowerSpectrumTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif

    };




} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/powerspectrum_test.h"
#endif


#endif
