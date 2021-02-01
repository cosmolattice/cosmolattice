#ifndef COSMOINTERFACE_SU2ALGEBRA_PAULIMATRICES_H
#define COSMOINTERFACE_SU2ALGEBRA_PAULIMATRICES_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"



namespace TempLat {


    /** \brief A class which implements the i*sigma matrices
     *
     * 
     * Unit test: make test-paulimatrices
     **/

    namespace Constants{

        struct Sigma0 {
            OneType SU2Get(Tag<0> t){
                return OneType();
            }
            ZeroType SU2Get(Tag<1> t){
                return ZeroType();
            }
            ZeroType SU2Get(Tag<2> t){
                return ZeroType();
            }
            ZeroType SU2Get(Tag<3> t){
                return ZeroType();
            }

            std::string toString() const
            {
                return "Id";
            }
            static constexpr size_t size =4;
        };
        constexpr Sigma0 sigma0;

        struct Sigma1 {
            ZeroType SU2Get(Tag<0> t){
                return ZeroType();
            }
            OneType SU2Get(Tag<1> t){
                return OneType();
            }
            ZeroType SU2Get(Tag<2> t){
                return ZeroType();
            }
            ZeroType SU2Get(Tag<3> t){
                return ZeroType();
            }
            static constexpr size_t size =4;

            std::string toString() const
            {
                return "i\u03C3_1";
            }
        };
        constexpr Sigma1 sigma1;

        struct Sigma2 {
            ZeroType SU2Get(Tag<0> t){
                return ZeroType();
            }
            ZeroType SU2Get(Tag<1> t){
                return ZeroType();
            }
            OneType SU2Get(Tag<2> t){
                return OneType();
            }
            ZeroType SU2Get(Tag<3> t){
                return ZeroType();
            }
            std::string toString() const
            {
                return "i\u03C3_2";
            }
            static constexpr size_t size =4;
        };
        constexpr Sigma2 sigma2;

        struct Sigma3 {
            ZeroType SU2Get(Tag<0> t){
                return ZeroType();
            }
            ZeroType SU2Get(Tag<1> t){
                return ZeroType();
            }
            ZeroType SU2Get(Tag<2> t){
                return ZeroType();
            }
            OneType SU2Get(Tag<3> t){
                return OneType();
            }

            std::string toString() const
            {
                return "i\u03C3_3";
            }
            static constexpr size_t size = 4;
        };
        constexpr Sigma3 sigma3;


        inline  Sigma0 i_sigma(Tag<0> t)
        {
            return sigma0;
        }
        inline  Sigma1 i_sigma(Tag<1> t)
        {
            return sigma1;
        }
        inline  Sigma2 i_sigma(Tag<2> t)
        {
            return sigma2;
        }
        inline  Sigma3 i_sigma(Tag<3> t)
        {
            return sigma3;
        }


    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2generators_test.h"
#endif


#endif
