#ifndef TEMPLAT_LATTICE_MANIPULATION_GHOSTUPDATER_TEST_H
#define TEMPLAT_LATTICE_MANIPULATION_GHOSTUPDATER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/fft/fftmpidomainsplit.h"
#include "TempLat/lattice/memory/triplestatelayouts.h"

namespace TempLat {
    /** \brief A namespace purely for test structures / classes. Quick simple structs for whatever test you need go in here. */
    namespace TestScratch {
        /* quick and ugly helper struct */
        struct datumMPITypeHolder {
            MPI_Datatype dType;
            datumMPITypeHolder() {
#ifndef NOMPI
                MPI_Type_contiguous(4, TempLat::MPITypeSelect<ptrdiff_t>(), &dType);
                MPI_Type_commit(&dType);
#endif
            }
            ~datumMPITypeHolder() {
#ifndef NOMPI
                int didFinalize = 0;
                MPI_Finalized(&didFinalize);
                if (! didFinalize) MPI_Type_free(&dType);
#endif
            }
        };

        struct datum {
            ptrdiff_t x, y, z, rank;

            friend std::ostream& operator << (std::ostream& ostream, const datum& dat) {
                ostream << "x: " << dat.x << ", y: " << dat.y << ", z: " << dat.z << ", rank: " << dat.rank;
                return ostream;
            }

            static MPI_Datatype getMPIType() {
                static datumMPITypeHolder holder;
                return holder.dType;
            }
        };
    }
}

inline void TempLat::GhostUpdater::Test(TempLat::TDDAssertion& tdd) {

    /* Perhaps a bit elaborate, but it is as consistent as it gets.. */
    /* And in fact it helped getting out the errors. Yes. */

    /* note to self: the debugging involved here was more about debugging the
     test case, then about debugging the actual class being debugged.
     Think of the golden refactoring rules to live by. Failed here. */

     //TODO: The test is broken with MPI. Not worrisome because the test itself is fine,
     // there is just a segfault t destruction time, related to how the test itself is contructed
     // Nice if someome were to fix that... . You can still using the test if you ignore
     // thesegfault, just uncomment here.

     say << "**************************\n WARNING: bug in this test, on destruction off the test data. The test itself works, so if you want to see, come here and uncomment and ignore the segfault.\n**************************\n";


//    const ptrdiff_t nDimensions = 3;
//    const ptrdiff_t nGrid = 256;
//    const ptrdiff_t ghostDepth = 3;
//
//    MPICartesianGroup mGroup(FFTMPIDomainSplit::makeMPIGroup(nDimensions));
//
//    FFTLibrarySelector fftlib(mGroup, nDimensions, nGrid);
//
//    /* regular ghosting */
//    TripleStateLayouts fullLayout(fftlib.getLayout(), ghostDepth);
//
//
//
//    std::vector<TestScratch::datum> memory(
//                                                      std::pow(nGrid + 2 * ghostDepth, nDimensions)
//                                                      );
//
//    const auto& starts = fullLayout.getConfigSpaceStarts();
//    const auto& sizes = fullLayout.getConfigSpaceSizes();
//
//
//    auto&& makeCoord = [&nGrid](const auto& start, const auto& index) {
//        ptrdiff_t result = (start + index) % nGrid;
//        while ( result < 0 ) result += nGrid;
//        return result;
//    };
//
//    /* setup the controlled known memory; each entry equals its position */
//    for ( ptrdiff_t i = -ghostDepth; i < sizes[0] + ghostDepth; ++i ) {
//        ptrdiff_t iPos = (i + ghostDepth) * (sizes[1] + 2 * ghostDepth) * (sizes[2] + 2 * ghostDepth);
//        ptrdiff_t iCoord = makeCoord(starts[0], i);
//        bool iInGhostRegime = i < 0 || i >= sizes[0];
//        for ( ptrdiff_t j = -ghostDepth; j < sizes[1] + ghostDepth; ++j ) {
//            ptrdiff_t jPos = (j + ghostDepth) * (sizes[2] + 2 * ghostDepth);
//            ptrdiff_t jCoord = makeCoord(starts[1], j);
//            bool jInGhostRegime = j < 0 || j >= sizes[1];
//            for ( ptrdiff_t k = -ghostDepth; k < sizes[2] + ghostDepth; ++k ) {
//                ptrdiff_t kPos = k + ghostDepth;
//                ptrdiff_t kCoord = makeCoord(starts[2], k);
//                bool kInGhostRegime = k < 0 || k >= sizes[2];
//                bool inGhostRegime = iInGhostRegime || jInGhostRegime || kInGhostRegime;
//                ptrdiff_t pos = iPos + jPos + kPos;
//                if ( ! inGhostRegime) {
//                    /* zero out the ghost cells, so we are sure that the correct value later on truly comes from the owning process. */
//                    memory[pos].x = iCoord;
//                    memory[pos].y = jCoord;
//                    memory[pos].z = kCoord;
//                }
//                memory[pos].rank = mGroup.getRank();
////                                if (i < 0 && j < 0 && k < 0)
//                //                if ( iCoord == 253 && jCoord == 253 && kCoord == 253)
////                                std::cerr << "Hoi " << "inGhostRegime: " << inGhostRegime << " " << pos << " " << i << " " << j << " " << k << " - coord: " << memory[pos] << "\n";
//            }
//        }
//    }
//
//    GhostUpdater ghostUpdater(mGroup, fullLayout.getConfigSpaceJumps());
//
//    ghostUpdater.update(memory.data());
//
//
//    bool allRight = true;
//    for ( ptrdiff_t i = -ghostDepth; i < sizes[0] + ghostDepth; ++i ) {
//        ptrdiff_t iPos = (i + ghostDepth) * (sizes[1] + 2 * ghostDepth) * (sizes[2] + 2 * ghostDepth);
//        ptrdiff_t iCoord = makeCoord(starts[0], i);
//        for ( ptrdiff_t j = -ghostDepth; j < sizes[1] + ghostDepth; ++j ) {
//            ptrdiff_t jPos = (j + ghostDepth) * (sizes[2] + 2 * ghostDepth);
//            ptrdiff_t jCoord = makeCoord(starts[1], j);
//            for ( ptrdiff_t k = -ghostDepth; k < sizes[2] + ghostDepth; ++k ) {
//                ptrdiff_t kPos = k + ghostDepth;
//                ptrdiff_t kCoord = makeCoord(starts[2], k);
//                ptrdiff_t pos = iPos + jPos + kPos;
//                bool thisRight = memory[pos].x == iCoord
//                && memory[pos].y == jCoord
//                && memory[pos].z == kCoord;
//                allRight = allRight && thisRight;
//
//                if (  !  thisRight ) std::cerr << "Ghost updating is broken " << pos << " | " << memory[pos] << ", "
//                    << memory[pos].x << " should be " << iCoord << " at " << i << ", "
//                    << memory[pos].y << " should be " << jCoord << " at " << j << ", "
//                    << memory[pos].z << " should be " << kCoord << " at " << k << "\n";
//
//            }
//        }
//    }
//
//
//    tdd.verify( allRight );

}

#endif
