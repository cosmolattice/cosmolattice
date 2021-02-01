#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_DIMENSIONCOUNTRECORDER_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_DIMENSIONCOUNTRECORDER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"

namespace TempLat {

    MakeException(DimensionCountRecorderException);
    MakeException(DimensionCountRecorder_CoordinateSpaceException);

    /** \brief A super class which records the number of dimensions on any
     *  call to confirmSpace.
     *
     *  The constructor takes one argument, which is used by the derived classes which insist on a single space only: WaveNumber and SpatialCoordinate.
     *
     * Unit test: make test-dimensioncountrecorder
     **/

    class DimensionCountRecorder {
    public:
        /* Put public methods here. These should change very little over time. */
        DimensionCountRecorder(SpaceStateInterface::SpaceType spaceType) :
        mNDimensions(-1),
        mFixedSingleSpaceType(spaceType),
        mCurrentSpaceType(spaceType),
        mCurrentLayout({})
        {

        }

        /** \brief When making sure everyone is in configuration or fourier space, steal the number of dimensions, which we need in the coordinate manipulating objects.. */
        inline
        void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {

            mNDimensions = newLayout.getLocalSizes().size();

            if ( mFixedSingleSpaceType != SpaceStateInterface::SpaceType::undefined && mFixedSingleSpaceType != spaceType) {
                throw DimensionCountRecorder_CoordinateSpaceException(
                            "You are using coordinates in one space for an expression in another space. This coordinate object insists on",
                            SpaceStateInterface::SpaceTypeString(mFixedSingleSpaceType),
                            "while you now ask for",
                            SpaceStateInterface::SpaceTypeString(spaceType)
                  );
            }

            mCurrentSpaceType = spaceType;
            mCurrentLayout = newLayout;

        }

        inline const ptrdiff_t& getNDimensions() {
            /* As this inline function is called in heavy loops, no checking here. */
#ifdef CHECKBOUNDS
            if ( mNDimensions < 0 ) throw DimensionCountRecorderException("Call to getNDimensions, while nDimensions has never been set.", mNDimensions, " < 0");
#endif
            return mNDimensions;
        }

        inline SpaceStateInterface::SpaceType getCurrentSpaceType() const {
            return mCurrentSpaceType;
        }

        inline const LayoutStruct & getCurrentLayout() const {
            return mCurrentLayout;
        }

        std::string toString() const {
            return SpaceStateInterface::SpaceTypetoCanonicalCharacter(mFixedSingleSpaceType);
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        ptrdiff_t mNDimensions;
        SpaceStateInterface::SpaceType mFixedSingleSpaceType;
        SpaceStateInterface::SpaceType mCurrentSpaceType;
        LayoutStruct mCurrentLayout;

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/coordinates/dimensioncountrecorder_test.h"
#endif


#endif
