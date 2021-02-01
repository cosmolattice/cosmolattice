#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANREDUNDANCY_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANREDUNDANCY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {
    
    /** \brief A class which
     *
     * Unit test: make test-hermitianredundancy
     **/
    
    /** \brief For testing purposes: track which entries in the layout carry redundant information, and if so, what information.
     *  This again assumes FFTW layout.
     *  none - the complex value at this wavenumber has no redundancy, and can take any value.
     *  negativePartner - the value at this wavenumber is the complex conjugate of another value in the globalSpace, and we chose that this partner is the negative one.
     *  positivePartner - well, you guess.
     *  realValued - the complex value at this wavenumber must have zero imaginary part.
     */
    enum class HermitianRedundancy {
        /** \brief the complex value at this wavenumber has no redundancy, and can take any value. */
        none,
        
        /** \brief the value at this wavenumber is the complex conjugate of another value in the globalSpace, and we chose that this partner is the negative one.  */
        negativePartner,
        
        /** \brief well, you guess. */
        positivePartner,
        
        /** \brief the complex value at this wavenumber must have zero imaginary part. */
        realValued
    };
    inline std::ostream& operator<<(std::ostream& ostream, const HermitianRedundancy& hr) {
        ostream << "HermitianRedundancy::";
        switch (hr) {
            case HermitianRedundancy::negativePartner:
                ostream << "negativePartner";
                break;
            case HermitianRedundancy::positivePartner:
                ostream << "positivePartner";
                break;
            case HermitianRedundancy::realValued:
                ostream << "realValued";
                break;
            case HermitianRedundancy::none:
            default:
                ostream << "none";
                break;
        }
        return ostream;
    }
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorylayouts/hermitianredundancy_test.h"
#endif


#endif
