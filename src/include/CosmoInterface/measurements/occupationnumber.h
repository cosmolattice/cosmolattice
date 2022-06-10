#ifndef COSMOINTERFACE_MEASUREMENTS_OCCUPATIONNUMBER_H
#define COSMOINTERFACE_MEASUREMENTS_OCCUPATIONNUMBER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/function.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/lattice/field/field.h"


namespace TempLat {


    /** \brief A class which computes the occupation number.
     *
     *
     **/



 class OccupationNumberMeasurer{
      public:
        template<typename T>
        OccupationNumberMeasurer(const RunParameters<T>& par) :
        nbins(par.nBinsSpectra)
        {}

        template< class Model, int I>
        auto occupationNumber(Model& model, Tag<I> i) {
            using T = decltype(model.dx);
            auto N = GetNGrid::get(model); //Isotropic lattices only.
            T kMaxBins = std::floor(pow(3, 0.5) / 2.0 * N) + 1;


            Field<T> tmp("tmp", GetToolBox::get(model));

            tmp = model.fldS(i);
            auto part1 = projectRadiallyFourier(pow<2>(abs(tmp.inFourierSpace()))).measure(nbins, kMaxBins);

            tmp = (pow(model.aI, model.alpha - 3) * model.piS(i)) + model.aDotI / model.aI * model.fldS(i);
            auto part2 = projectRadiallyFourier(pow<2>(abs(tmp.inFourierSpace()))).measure(nbins, kMaxBins);

            T normalisation = pow<2>(model.aI) * pow<3>(model.dx) / pow<3>(N)  / 2 * pow<2>(model.fStar / model.omegaStar);

            T m2 = average(model.potDeriv2(i));

            auto omegaK = Function(k, sqrt(pow<2>(model.aI) * m2 + pow<2>(k)));

            return (Function(k,normalisation * omegaK(k)) * part1)
            +
            (Function(k,normalisation * pow(model.aI, 2.0 * (1.0 - model.alpha)) / omegaK(k)) * part2);
        }


      private:
        ptrdiff_t nbins;
     };



    class OccupationNumberTester {

    public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */
#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/occupationnumber_test.h"
#endif


#endif
