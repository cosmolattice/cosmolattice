#ifndef TEMPLAT_LATTICE_MEASUREMENTS_TWOPOINTCORRELATOR_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_TWOPOINTCORRELATOR_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"
#include "TempLat/lattice/algebra/operators/complexconjugate.h"
#include "TempLat/lattice/algebra/operators/squareroot.h"
#include "TempLat/lattice/algebra/operators/exponential.h"

inline void TempLat::TwoPointCorrelatorTester::Test(TempLat::TDDAssertion& tdd) {

    /* this test is a quite tautological, as we define the result from the beginning in fourier space.
     * It actually only tests taking the absolute value squared.
     */
    auto && myLittleLambda = [&tdd](ptrdiff_t nDim, ptrdiff_t nGrid, ptrdiff_t nGhost, double expo) {
        auto toolBox = MemoryToolBox::makeShared(nDim, nGrid, nGhost);

        Field<double> base("base", toolBox);

        Field<double> ic("ic", toolBox);

        /* make two-point correlation r^n */

        WaveNumber k(toolBox);

        /* now equal to two-point correlation */
        /* Ugly line to get 1 if r == 0, r otherwise: Heaviside(r - 1) * (r - 1) + 1 */
        base.inFourierSpace() = pow(k.norm(), expo);

        ic.inFourierSpace() = base.inFourierSpace();

        auto checkResult = projectRadially(ic, SpaceStateInterface::SpaceType::Configuration,toolBox, false, false).measure();

//        base.iterate([&base](auto &piter) { say << piter.getCoordinates() << " " << base.get(piter) << "\n"; return true; });

        /* now decompose |f_k|^2 into random phased re(f_k) and im(f_k) */

        auto phaseAsGauss = RandomGaussianField<double>("hoi",toolBox);

        /* exp(i phi) - take the imaginary part of the gaussian field as phase, because
         * that one is zero at the real-valued places -> preserve that.
         */
        auto singlePhase = 5 * (phaseAsGauss - conj(phaseAsGauss));

//        base.inFourierSpace().iterate([&base](auto &piter) { say << piter.getCoordinates() << " " << base.inFourierSpace().get(piter) << "\n"; return true; });

        base.inFourierSpace() = sqrt(abs(base.inFourierSpace())) * exp(singlePhase);

//        base.inFourierSpace().iterate([&base](auto &piter) { say << piter.getCoordinates() << " " << base.inFourierSpace().get(piter) << "\n"; return true; });

        auto fr = TwoPointCorrelator(base);

        auto result = fr.measure(-1);

        bool allGood = true;
        for ( ptrdiff_t i = 0, iEnd = result.size(); i < iEnd; ++i) {
            /* had to add 1e3 to the result, because of all the numerically close to zeros. */
            bool thisGood = AlmostEqual(1e3 + checkResult[i].getValue().average, 1e3 + result[i].getValue().average, 0.1);
            allGood = allGood && thisGood;

//            say << (thisGood ? "EQUAL " : "UNEQUAL " ) << checkResult[i].getValue().average << " vs. " << result[i].getValue().average << " ratio: " << result[i].getValue().average / checkResult[i].getValue().average <<  "\n";
        }

        tdd.verify(allGood);
        //say << result << "\n";

    };

    myLittleLambda(2, 16, 1, 1);

//    myLittleLambda(3, 256, 1, 4);

//    myLittleLambda(4, 32, 1, 0.25);

}

#endif
