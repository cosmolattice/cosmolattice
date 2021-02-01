#ifndef TEMPLAT_LATTICE_ALGEBRA_SPACESTATEINTERFACE_H
#define TEMPLAT_LATTICE_ALGEBRA_SPACESTATEINTERFACE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <string>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/jumpsholder.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"

namespace TempLat {

    /** \brief An interface class which all getter-like objects inherit from, so we can access their confirm(Config/Fourier)Space methods by their virtualness.
     *
     * Unit test: make test-spacestateinterface
     **/

    class SpaceStateInterface {
    public:
    /* Put public methods here. These should change very little over time. */

        /** \brief An enum for passing the right space type, all while having the compiler type check it. */
        enum class SpaceType {
            Configuration, Fourier, undefined
        };

        /** \brief A preparation function: pass this call to all the members in the tree / chain, make sure everyone is in configuration or fourier space, and everyone has the actual same layout. */
        virtual
        void confirmSpace(const LayoutStruct &newLayout, const SpaceType &spaceType) = 0;

        /** \brief A preparation function: pass this call to all the members in the tree / chain.
         *   We only want to update ghost cells on those fields where this expression tree actually uses
         *   a spatially shifted view (which needs ghost cells). So only such views have permission
         *   to toggle the value of the detector to true. The Fields on the receiving end of this chain,
         *   then update ghosts.
         */
        //virtual
        //ptrdiff_t confirmGhostsUpToDate(FieldShiftedViewDetection detector) = 0;


        /** \brief In order to take spatial derivatives, the GetterShifted needs to know the jumps.
         *  This way the user has the most minimal need-to-know interface for spatial derivatives.
         *  Also, this is the perfect moment for verifying that all elements in
         *  the tree are of the same layout (or a constant).
         */
        virtual inline
        JumpsHolder getConfigSpaceJumps() = 0;

        /** \brief In order to take spatial derivatives, the GetterShifted needs to know the jumps.
         *  This way the user has the most minimal need-to-know interface for spatial derivatives.
         *  Also, this is the perfect moment for verifying that all elements in
         *  the tree are of the same layout (or a constant).
         */
        virtual inline
        JumpsHolder getFourierSpaceJumps() = 0;

        /** For measurement objects: need the toolbox for easiest access to loopers and whatever else. */
        virtual inline
        std::shared_ptr<MemoryToolBox> getToolBox() = 0;


        static inline std::string SpaceTypeString(SpaceType st) {
            std::string result;
            switch (st) {
                case SpaceType::Configuration:
                    result = "configuration space";
                    break;
                case SpaceType::Fourier:
                    result = "fourier space";
                    break;
                default:
                case SpaceType::undefined:
                    result = "undefined space type";
                    break;
            }
            return result;
        }

        static inline std::string SpaceTypetoCanonicalCharacter(SpaceType st) {
            std::string result;
            switch (st) {
                case SpaceType::Configuration:
                    result = "x";
                    break;
                case SpaceType::Fourier:
                    result = "k";
                    break;
                default:
                case SpaceType::undefined:
                    result = "[x or k]";
                    break;
            }
            return result;
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */


    };

    inline std::ostream& operator<< (std::ostream& ostream, SpaceStateInterface::SpaceType st) {
        ostream << "SpaceType::" + SpaceStateInterface::SpaceTypeString(st);
        return ostream;
    }

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/spacestateinterface_test.h"
#endif


#endif
