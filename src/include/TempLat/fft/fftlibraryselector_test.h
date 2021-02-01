#ifndef TEMPLAT_FFT_FFTLIBRARYSELECTOR_TEST_H
#define TEMPLAT_FFT_FFTLIBRARYSELECTOR_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/almostequal.h"
#include "TempLat/fft/fftmpidomainsplit.h"
#include <functional>



template <typename T>
inline void TempLat::FFTLibrarySelector::TestBody(TempLat::TDDAssertion& tdd) {
    
    /* int main already calls the mpi guard, which calls the FFT session guards. So this should be the second time. */
    tdd.verify( Throws<FFTLibraryDoubleInitializationException> (
                                                                 []() {
                                                                     FFTLibrarySelector::getSessionGuards();
                                                                 }
                                                                 ) );
    
    /* let's create some memory, FFT forth and back, and check that the result is close enough to the input, module the normalization.. */
    
    auto&& myLittleLambda = [&tdd]( std::vector<ptrdiff_t> nGrid) {
        ptrdiff_t nDim = nGrid.size();
        auto split = FFTMPIDomainSplit::makeDomainDecomposition(MPICommReference().size(), nDim);
        auto mGroup_ = MPICartesianGroup(nDim, split);
        FFTLibrarySelector ffter(mGroup_ ,  nGrid);
        ffter.setVerbose();
        
        auto layout = ffter.getLayout();
        
        say << layout << "\n";
        
        MemoryBlock<T> mem(layout.getMinimalMemorySize());
        
        for ( int order = 0; order < 2; ++order) {
            
            auto& currentLayout= order ? layout.fourierSpace : layout.configurationSpace;
            auto memorySizes = currentLayout.getSizesInMemory();
            
            std::vector<ptrdiff_t> actualVPos(nDim);
            /* manually implement 3d: */
            std::function<bool(std::function<bool(ptrdiff_t, std::vector<ptrdiff_t>)>, ptrdiff_t, ptrdiff_t, std::vector<ptrdiff_t>)> iterate = [&](std::function<bool(ptrdiff_t, std::vector<ptrdiff_t>)> funcCall, ptrdiff_t dim, ptrdiff_t basePos, std::vector<ptrdiff_t> vPos) {
                bool carryOn = true;
                if ( dim != nDim - 1 ) {
                    for (
                         int i = 0,
                         iEnd = memorySizes[dim];
                         i < iEnd && carryOn;
                         ++i
                         ) {
                        vPos[dim] = i;
                        carryOn = iterate(funcCall, dim + 1, basePos * iEnd + i, vPos);
                    }
                } else {
                    for (
                         int k = 0,
                         kSize = (1 + order) * memorySizes[dim],
                         kEnd = (1 + order) * memorySizes[dim];
                         k < kEnd && carryOn;
                         ++k
                         ) {
                        ptrdiff_t pos = basePos * kSize + k;
                        vPos[dim] = k;
                        carryOn = funcCall(pos, vPos);
                    }
                }
                return carryOn;
            };
            
            auto coordinateToValue = [&](auto vPosctv) {
                
                std::vector<ptrdiff_t> complexMemCoordinate(vPosctv.begin(), vPosctv.end());
                std::vector<ptrdiff_t> spaceCoordinate(vPosctv.size());
                
                /* now, in this routine we walk the memory lineary, not caring about complex stuff. Hence vPosctv[2] /= 2; */
                
                bool isImaginaryPart = complexMemCoordinate[nDim - 1] % 2;
                
                complexMemCoordinate[nDim - 1] /= 2;
                for ( ptrdiff_t i = 0; i < nDim; ++i) {
                    currentLayout.putSpatialLocationFromMemoryIndexInto(complexMemCoordinate[i], i, spaceCoordinate);
                }
                
                std::vector<ptrdiff_t> hermitianPartnerCoordinate(spaceCoordinate.begin(), spaceCoordinate.end());
                
                ptrdiff_t imaginaryPartSign = 1;
                
                if ( order ) {
                    
                    auto hermQual = currentLayout.getHermitianPartners()->qualify(spaceCoordinate);
                    currentLayout.getHermitianPartners()->putHermitianPartner(spaceCoordinate, hermitianPartnerCoordinate);
                    
                    if ( isImaginaryPart ) {
                        if ( hermQual == HermitianRedundancy::realValued ) {
                            imaginaryPartSign = 0;
                        } else if ( hermQual == HermitianRedundancy::negativePartner ) {
                            imaginaryPartSign = -1;
                        } else if ( hermQual == HermitianRedundancy::positivePartner ) {
                            imaginaryPartSign = 1;
                        }
                    }
                    
                }
                
                T result = 0.;
                for ( auto&& it : hermitianPartnerCoordinate) { //WARNING: works only for isotropic grids
                    result = result * nGrid[0] + (it + nGrid[0] / 2);
                }
                
                //            if (vPosctv[0] % 8 == 0 && vPosctv[1] % 8 == 0 && vPosctv[2] % 8 == 1 )  sayMPI << "imaginaryPartSign * result: " << imaginaryPartSign * result << " " << vPosctv << " -> " << spaceCoordinate << " -> " << hermitianPartnerCoordinate << "\n";
                
//                if ( isImaginaryPart ) result += 0.5;
                
                return imaginaryPartSign * result;
            };
            
            auto setMemory = [&](auto pos, auto vPossetmem) {
                mem[pos] = coordinateToValue(vPossetmem);
                return true;
            };
            
            iterate(setMemory, 0, 0, actualVPos);
            
            /* order == 0: r2c then c2r. Order == 1: c2r then r2c. */
            sayMPI << "About to do FFT.\n";
            if (! order) ffter.r2c(mem);
            ffter.c2r(mem);
            sayMPI << "Finished FFT.\n";
            //auto emptyPadding = [&](auto pos, auto vPos) {
            //    if ( vPos.back() >= memorySizes.back() - 2) {
            //std::cerr << "Padding " << vPos << " " << mem[pos] << "\n";
            //        mem[pos] = 100000 ;
            //    }
            //    return true;
            //};
            // verified: no effect on c2r - r2c result;
            //if ( ! order ) iterate(emptyPadding, 0, 0, actualVPos);
            T norm = 1. / std::pow(nGrid[0], nDim);
            
            if ( order) {
                ffter.r2c(mem);
                //            ffter.c2r(mem);
                //            ffter.r2c(mem);
                //            norm = 1. / std::pow(nGrid, 2 * nDim);
                
            }
            
            bool allRight = true;
            auto checkMem = [&](auto pos, auto vPos) {
                //sayMPI << "checkMem " << pos << "\n";
                T valueShouldBe = coordinateToValue(vPos);
                
                if ( (!order) && vPos.back() >= memorySizes.back() - 2) return true;
                
                bool thisCheck = AlmostEqual(valueShouldBe, norm * mem[pos], std::is_same<T, float>::value ? 0.1 : 1e-4 );
                
                allRight = allRight && thisCheck;
                //            if ( order && allRight ) say << "Equal: " << vPos << " -> " << valueShouldBe << " == " << norm * mem[pos] << "\n";
                if ( ! thisCheck ) {
                    say << "Not equal: " << vPos << " -> " << valueShouldBe << " != " << norm * mem[pos] << " = " << norm << " * " << mem[pos] << "\n" ;
                    //                for ( int ii = 1; ii < 20; ++ii)
                    //                    std::cerr << "pos + " << ii << " = " << (pos + ii) << ": " << norm * mem[pos + ii] << " = " << norm << " * " << mem[pos + ii] << "\n";
                    //                std::cerr << "\n\n";
                }
                return allRight;
            };
            iterate(checkMem, 0, 0, actualVPos);
            
            bool r2c_then_c2r = allRight;
            bool c2r_then_r2c = allRight;
            if ( order ) tdd.verify(c2r_then_r2c);
            else tdd.verify(r2c_then_c2r);
            if ( ! allRight ) sayMPI << "Failed for nDim: " << nDim << ", nGrid: " << nGrid << "\n";
            else sayMPI << "Success for nDim: " << nDim << ", nGrid: " << nGrid << "\n";
        }
    };

    
    for ( ptrdiff_t inDim = 2; inDim < 5; ++inDim) {
        for (ptrdiff_t inGrid = 1; inGrid < 6; ++inGrid ) {
            myLittleLambda( std::vector<ptrdiff_t >(inDim, std::pow(2, inGrid)));
        }
    }


}

inline void TempLat::FFTLibrarySelector::Test(TempLat::TDDAssertion& tdd) {
#ifndef NOFFTFLOAT
    TestBody<float>(tdd);
#endif
    TestBody<double>(tdd);
}

#endif
