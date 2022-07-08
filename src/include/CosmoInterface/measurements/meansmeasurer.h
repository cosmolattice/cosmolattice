#ifndef COSMOINTERFACE_MEASUREMENTS_STANDARDMEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_STANDARDMEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/measurements/measurementsIO/measurementssaver.h"
#include "TempLat/lattice/algebra/operators/power.h"

namespace TempLat {


    /** \brief A class which implements common measurement to all fields, like mean and variance, in a standardized format.
     *
     *
     **/

    class MeansMeasurer {
    public:
        /* Put public methods here. These should change very little over time. */
        MeansMeasurer() = delete;

        template<typename S, typename Q, typename T>
        static void measure(MeasurementsSaver<T>& ms, S fld, Q mom, T t)
        {
            auto phi = average(fld);
            auto phi2 = average(pow<2>(fld));
            auto pi = average(mom);
            auto pi2 = average(pow<2>(mom));

            ms.addAverage(t);
            ms.addAverage(phi);
            ms.addAverage(pi);
            ms.addAverage(phi2);
            ms.addAverage(pi2);

            auto rmsPhi2 = phi2 - pow<2>(phi);
            auto rmsPi2 = pi2 - pow<2>(pi);

            // Sometimes, machine precision leads to imaginary rms.
            // We check for this explicitly and set it to zero when it happens
            ms.addAverage(rmsPhi2 > 0 ? sqrt(rmsPhi2) : 0);  // rms(phi)
            ms.addAverage(rmsPi2 > 0 ? sqrt(rmsPi2) : 0);  // rms(pi)
        }

		// Header for average files of scalar fields (scalar singlets, complex scalars, and SU(2) doublets)
        static std::vector<std::string> header()
        {
            return {"t","<phi>" ,"<pi>" ,"<phi^2>" ,"<pi^2>" ,"rms(phi)" ,"rms(pi)"};
        }

         // Header for average files of gauge fields (U(1) and SU(2) sectors)
        static std::vector<std::string> headerEB()
        {
            return {"t","<|E|>" ,"<|B|>" ,"<|E|^2>" ,"<|B|^2>" ,"rms(|E|)" ,"rms(|B|)"};
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/meansmeasurer_test.h"
#endif


#endif
