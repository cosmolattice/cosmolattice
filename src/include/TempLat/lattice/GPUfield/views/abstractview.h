#ifndef TEMPLAT_LATTICE_GPUFIELD_ABSTRACTVIEW_H
#define TEMPLAT_LATTICE_GPUFIELD_ABSTRACTVIEW_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/memory/jumpsholder.h"
#include "TempLat/lattice/latticeparameters.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"

#include "TempLat/lattice/algebra/helpers/getderiv.h"

namespace TempLat {
    MakeException(GPUFieldValueGetterException);

    /** \brief A simple class which provides a get method for basic types.
     * Field class
     *
     **/
    template<typename T>
    class AbstractView {
    public:
        /* Put public methods here. These should change very little over time. */

        AbstractView(std::string name, LatticeParameters<T> pLatPar)
            : latPar(pLatPar) {
        }

        KOKKOS_FORCEINLINE_FUNCTION
        void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
            switch (spaceType) {
                case SpaceStateInterface::SpaceType::Fourier:
                    // TODO
                    //mManager.confirmFourierSpace();
                    break;
                case SpaceStateInterface::SpaceType::Configuration:
                default:
                    // TODO
                    //mManager.confirmConfigSpace();
                    break;
            }
        }

        ptrdiff_t confirmGhostsUpToDate() {
            // TODO
            //return mManager.confirmGhostsUpToDate();
            return 0;
        }

        /** \brief Check the current state. */
        bool isConfigSpace() const {
            // TODO
            //return mManager.isConfigSpace();
            return 0;
        }

        /** \brief Check the current state. */
        bool isFourierSpace() const {
            // TODO
            //return mManager.isFourierSpace();
            return 0;
        }

        void setGhostsAreStale() {
            // TODO
            //mManager.setGhostsAreStale();
        }

        bool areGhostsStale() const {
            // TODO
            //return mManager.areGhostsStale();
            return 0;
        }

        KOKKOS_FORCEINLINE_FUNCTION
        auto getDx() const {
            return latPar.getDx();
        }

        KOKKOS_FORCEINLINE_FUNCTION
        auto getKIR() const {
            return latPar.getKIR();
        }

    protected:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        LatticeParameters<T> latPar; // Information about the lattice (dx, kir...)

        // Conceptually not amazing but really useful.
        ptrdiff_t getOffsetFromCoords(bool &test, std::vector<ptrdiff_t> position) {
            return 0;
            // TODO
            /*
            auto mJumps = getJumps();

            if (mJumps.size() != position.size())
                throw GPUFieldValueGetterException(
                    "Wrong size / number of arguments to Field<T>::operator(). Dimensionality of field:", mJumps.size(),
                    ", dimensionality of your arguments:", position.size());

            ptrdiff_t offset = mJumps.getTotalOffsetFromSpatialCoordinates(position);

            // our hack to give something that is not in the memory, without throwing an exception.
            if (offset > -1) test = true;
            else test = false;
            return offset;
            */
        }
    };
} /* TempLat */

#endif
