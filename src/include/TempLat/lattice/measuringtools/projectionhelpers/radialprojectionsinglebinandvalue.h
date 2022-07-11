#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEBINANDVALUE_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEBINANDVALUE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <sstream>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsingledatum.h"

namespace TempLat {

    /** \brief A class which combines a pair of RadialProjectionSingleDatums, one for
     *  the bin position, one for the function value for that bin.
     *
     *
     * Unit test: make test-radialprojectionsinglebinandvalue
     **/
    template <typename T>
    struct RadialProjectionSingleBinAndValue {
    public:
        /** \brief Default-construct binInformation and valueInformation: zeros (and equivalent), useful for summing up multiple entries. */
        RadialProjectionSingleBinAndValue() { }

        RadialProjectionSingleBinAndValue(RadialProjectionSingleDatum<T> binInformation, RadialProjectionSingleDatum<T> valueInformation) :
        mBinInformation(binInformation),
        mValueInformation(valueInformation)
        {

        }

        std::string toString(bool withBinInfo = true, bool withMultiplicity = true, int verbosity = 0) const {  // these booleans will allow to save multiple spectra in the same file.
            std::stringstream sstream;
            if(withBinInfo) sstream << mBinInformation.toString(false, verbosity) << " " << mValueInformation.toString(withMultiplicity, verbosity);
            else sstream << mValueInformation.toString(withMultiplicity, verbosity);
            return sstream.str();
        }

        std::string getBinString( int verbosity = 0) const {  // these booleans will allow to save multiple spectra in the same file.
            std::stringstream sstream;
            sstream << mBinInformation.toString(false, verbosity);
            return sstream.str();
        }

        std::string getValueString( bool withMultiplicity = true, int verbosity = 0, bool fourierMult = false) const {  // these booleans will allow to save multiple spectra in the same file.
            std::stringstream sstream;
            sstream << mValueInformation.toString(withMultiplicity, verbosity, fourierMult);
            return sstream.str();
        }


        std::string getHeader(int verbosity = 0) const {
            return "# " + mBinInformation.getHeader(0, "bin", false, verbosity) + ", " + mValueInformation.getHeader(verbosity == 3 ? 4 : 1, "values", true, verbosity);
        }
        std::string getHeaderBin(int verbosity = 0, int shift = 0) const {
            return "# " + mBinInformation.getHeader(shift, "bin", false, verbosity);
        }
        std::string getHeaderValue(int verbosity = 0, int withMultiplicities = true, int shift = 0) const {
            return  mValueInformation.getHeader(shift, "values", withMultiplicities, verbosity);
        }

        friend
        std::ostream& operator<< ( std::ostream& ostream, const RadialProjectionSingleBinAndValue &dp) {
            ostream << dp.toString();
            return ostream;
        }

        RadialProjectionSingleDatum<T>& getBin() { return mBinInformation; }
        RadialProjectionSingleDatum<T>& getValue() { return mValueInformation; }
        const RadialProjectionSingleDatum<T>& getBin() const { return mBinInformation; }
        const RadialProjectionSingleDatum<T>& getValue() const { return mValueInformation; }

        /** \brief Summing two instances, lets you do a re-binning without having to think. Combines both values taking into account their original weights. */
        friend RadialProjectionSingleBinAndValue<T> combine(const RadialProjectionSingleBinAndValue<T>& a, const RadialProjectionSingleBinAndValue<T>& b) {
            return RadialProjectionSingleBinAndValue<T>( combine(a.mBinInformation, b.mBinInformation), combine(a.mValueInformation, b.mValueInformation));
        }

        /** \brief Summing two instances, lets you do a re-binning without having to think. Combines both values taking into account their original weights. */
        RadialProjectionSingleBinAndValue<T>& combineTo(const RadialProjectionSingleBinAndValue<T>& other) {
            *this = combine(*this, other);
            return *this;
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        RadialProjectionSingleDatum<T> mBinInformation;
        RadialProjectionSingleDatum<T> mValueInformation;



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsinglebinandvalue_test.h"
#endif


#endif
