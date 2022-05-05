#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONREBINNER_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONREBINNER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <vector>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialbincomputer.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsinglebinandvalue.h"

namespace TempLat {

    /** \brief A class which takes a std::vector<RadialProjectionSingleBinAndValue<T>> and rebins it into
     *  a smaller number of bins.
     *
     * Unit test: make test-radialprojectionrebinner
     **/

    template <typename T>
    class RadialProjectionRebinner {
    public:

        static
        std::vector<RadialProjectionSingleBinAndValue<T>>
        rebin(
              const std::vector<RadialProjectionSingleBinAndValue<T>>& old,
              ptrdiff_t newNBins,
              std::vector<T>& oldCentralValues,
              T customRange
              )
        {
            std::vector<RadialProjectionSingleBinAndValue<T>> result;
            if ( newNBins >= (ptrdiff_t) old.size() ) {
                result = old;
            } else {
                T minVal = old[0].getBin().minVal;
                T maxVal = old.back().getBin().maxVal;

                //say << maxVal;
                //say << customRange;

                RadialBinComputer pc(minVal, customRange < 0 ? maxVal : customRange, newNBins);
                result.clear();
                result.resize(newNBins);
                for (auto&& it : old) {
                    /* just take the central value for the bin indexing. */
                    result[pc(it.getBin().average)].combineTo(it);
                }
                pc.setCentralBinBounds(oldCentralValues);
            }
            return result;
        }



    private:
        /** \brief Only one static method. No instantiation. */
        RadialProjectionRebinner() {

        }



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionrebinner_test.h"
#endif


#endif
