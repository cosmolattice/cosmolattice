#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEDATUM_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEDATUM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <sstream>
#include <algorithm>
#include <iomanip>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"

namespace TempLat {

    /** \brief A class which holds a single bin which results from the radial
     *   projection of something. Used for describing both the bin position and values,
     *   namely combined in `RadialProjectionSingleBinAndValue`.
     *
     * Unit test: make test-radialprojectionsingledatum
     **/


    template <typename T>
    struct RadialProjectionSingleDatum {

        typedef typename GetFloatType<T>::type floatType;

        /** \brief default constructor: values such that adding a non-default constructed RadialProjectionSingleDatum<T> leads to the right RadialProjectionSingleDatum<T> */
        RadialProjectionSingleDatum() :
        multiplicity(0),
        average(0),
        sampleVariance(0),
        minVal(std::numeric_limits<T>::max()),
        maxVal(std::numeric_limits<T>::min())
        {}

        RadialProjectionSingleDatum(T sum, T sumOfSquares, T inputMinVal, T inputMaxVal, floatType inputMultiplicity) :
        multiplicity(inputMultiplicity),
        average(sum / multiplicity),
        sampleVariance(sumOfSquares / multiplicity - average * average),
        minVal(inputMinVal),
        maxVal(inputMaxVal)
        { }

        floatType multiplicity;
        T average;
        T sampleVariance;
        T minVal;
        T maxVal;

        std::string getHeader(ptrdiff_t start, std::string prefix, bool withMultiplicity, int verbosity = 0) const {
            return std::to_string(start) + ": " + prefix + "Average "
            + (verbosity == 2 ? std::to_string(start + 1) + ": " + prefix + "SampleVariance, "
            + std::to_string(start + 2) + ": " + prefix + "Minimum, "
            + std::to_string(start + 3) + ": " + prefix + "Maximum"
            + (withMultiplicity ? (", " + std::to_string(start + 4) + ": multiplicity") : "") : (withMultiplicity ? (", " + std::to_string(start + 1) + ": multiplicity") : "") );
        }

        std::string toString(bool withMultiplicity = false, int verbosity = 0, bool fourierMult = true) const {
            std::stringstream sstream;
            switch(verbosity){
                case 0 : case 1: {
                    sstream << std::setprecision(16) << average;
                    break;
                }
                case 2:{
                    sstream << std::setprecision(16) << average << " " << sampleVariance << " " << minVal << " " << maxVal;
                    break;
                }
            }
            if ( withMultiplicity ) sstream << " " <<  (fourierMult ? 2 * multiplicity : multiplicity); 
            return sstream.str();
        }

        friend
        std::ostream& operator<< ( std::ostream& ostream, const RadialProjectionSingleDatum &dp) {
            ostream << dp.toString(true);
            return ostream;
        }


        /** \brief For rescaling your results. */
        RadialProjectionSingleDatum<T>& operator*=(floatType value) {
            average *= value;
            sampleVariance *= value * value;
            minVal *= value;
            maxVal *= value;
            return *this;
        }

        /** \brief For  getting a sum instead of an average */
        RadialProjectionSingleDatum<T>& sumInsteadOfAverage() {
            floatType intMultiplicity = 2 *multiplicity; // *2 is to get the full number of modes, against original design
            return (*this) *= intMultiplicity;
        }


        /** \brief Summing two instances, lets you do a re-binning without having to think. Combines both values taking into account their original weights. */
        friend RadialProjectionSingleDatum<T> combine(const RadialProjectionSingleDatum<T>& a, const RadialProjectionSingleDatum<T>& b) {
            T a_sumOfSquares = (a.sampleVariance + a.average * a.average) * a.multiplicity;
            T b_sumOfSquares = (b.sampleVariance + b.average * b.average) * b.multiplicity;
            T a_sum = a.average * a.multiplicity;
            T b_sum = b.average * b.multiplicity;

            return RadialProjectionSingleDatum<T>(
                a_sum + b_sum,
                a_sumOfSquares + b_sumOfSquares,
                std::min(a.minVal, b.minVal),
                std::max(a.maxVal, b.maxVal),
                a.multiplicity + b.multiplicity
            );

        }

#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsingledatum_test.h"
#endif


#endif
