#ifndef TEMPLAT_LATTICE_LATTICEPARAMETERS_H
#define TEMPLAT_LATTICE_LATTICEPARAMETERS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/constants.h"
#include "TempLat/util/exception.h"
#include "TempLat/util/almostequal.h"

namespace TempLat {


    /** \brief A class which holds physical paramteters of the lattice. Note that dx and kIr are equivalent only for isotropic lattices. For anisotropic, only one of th e two should be specified.
     *
     *
     * Unit test: make test-latticeparameters
     **/
     MakeException(LatticeParametersInconsistent);

    template<typename T>
    struct LatticeParameters {
    public:
        /* Put public methods here. These should change very little over time. */
        LatticeParameters(T pDx, T pLSide, T pKIR) :
        dx(pDx),
        lSide(pLSide),
        kIR(pKIR)
        {
          if(! AlmostEqual(kIR , 2 * Constants::pi<T> / lSide) ) throw(LatticeParametersInconsistent("kIR = "+std::to_string(kIR)+" and lSide = "+std::to_string(lSide)+" are not consistent. "));
        }

        LatticeParameters() :
        dx(1),
        lSide(1),
        kIR(1)
        {
        }

      /*  LatticeParameters(const LatticeParameters<T>& lat) :
        dx(lat.dx),
        lSide(lat.lSide),
        kIR(lat.kIR)
        {
        }

        void operator=(const LatticeParameters<T>& lat)
        {
          dx = lat.dx;
          lSide = lat.lSide;
          kIR = lat.kIR;
        }*/

        T getDx() const
        {
          return dx;
        }

        T getKIR() const
        {
          return kIR;
        }

        T getLSide() const
        {
          return lSide;
        }



    private:
        T dx, lSide, kIR;
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/latticeparameters_test.h"
#endif


#endif
