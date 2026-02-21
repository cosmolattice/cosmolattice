#ifndef COSMOINTERFACE_MEASUREMENTS_ABSTRACTMEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_ABSTRACTMEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Year: 2025

namespace TempLat {

    class AbstractMeasurer {
    public:
        AbstractMeasurer() : lastMeas(false) {}

        void setLastMeas(bool last) {
            lastMeas = last;
        }

    protected:
        bool lastMeas;
    };

} /* TempLat */

#endif
