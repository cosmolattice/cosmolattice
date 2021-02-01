#ifndef TEMPLAT_LATTICE_MEMORY_VERBOSITYLEVELS_H
#define TEMPLAT_LATTICE_MEMORY_VERBOSITYLEVELS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A struct with a bunch of flags with enable various output messages. Use to your advantage.
     *
     * Unit test: make test-verbositylevels
     **/

    struct VerbosityLevels {
    public:
        /* Put public methods here. These should change very little over time. */
        VerbosityLevels() :
        fieldAssignment(true),
        ghostUpdating(true),
        ghostsStale(false),
        allocation(false),
        spaceConfirmation(false),
        ghostConfirmationSteps(false)
        {     }

        /** \brief Show a message when a field receives an expression and takes its result as new value. */
        bool fieldAssignment;

        /** \brief Show a message when a field performs an in-place FFT. */
        bool fftPerformance;

        /** \brief Show a message when a field updates the ghost cells, which happens just before a non-local operation (spatial derivatives). */
        bool ghostUpdating;

        /** \brief Show a message when a field's values are changed, and consequentially the ghost-are-stale flag is set. */
        bool ghostsStale;
        
        /** \brief Show a message when memory is allocated. */
        bool allocation;
        
        /** \brief Show many messages outlining the entire process of confirming Fourier vs. configuration space states of a field. */
        bool spaceConfirmation;
        
        /** \brief Show many messages outlining the entire process of checking and updating ghost cells. */
        bool ghostConfirmationSteps;
        
        /** \brief Open Pandora's box. */
        void setAllOn() {
            fieldAssignment = true;
            fftPerformance = true;
            ghostUpdating = true;
            ghostsStale = true;
            allocation = true;
            spaceConfirmation = true;
            ghostConfirmationSteps = true;
        }

        /** \brief Close Pandora's box. */
        void setAllOff() {
            fieldAssignment = false;
            fftPerformance = false;
            ghostUpdating = false;
            ghostsStale = false;
            allocation = false;
            spaceConfirmation = false;
            ghostConfirmationSteps = false;
        }

        void setToDefault() {
            *this = VerbosityLevels();
        }

#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/verbositylevels_test.h"
#endif


#endif
