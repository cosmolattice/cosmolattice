#ifndef TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_VERIFYHERMITIANLAYOUT_TEST_H
#define TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_VERIFYHERMITIANLAYOUT_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <chrono>

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"

inline void TempLat::VerifyHermitianLayout::Test(TempLat::TDDAssertion& tdd) {


    auto&& myLittleLambda = [&tdd](ptrdiff_t nDim, ptrdiff_t nGrid, ptrdiff_t nGhost) {

        auto toolBox = MemoryToolBox::makeShared(nDim, nGrid, nGhost);

        auto HP = toolBox->mLayouts.getFourierSpaceLayout().getHermitianPartners();

        Field<double> phi("phi", toolBox);

        SpatialCoordinate x(toolBox);
        phi = norm(x);



        Field<double> phi_of_k_Real("phi_of_k", toolBox);

        auto phi_of_k = phi_of_k_Real.inFourierSpace();

        phi_of_k = phi.inFourierSpace();

        phi_of_k.getMemoryManager()->confirmFourierSpace();

        /* tadaaa... fft done. */
        bool allRight = true;

        /* loop over the fourier view, and check that the hermitian symmetry is respected. Must sadly skip partners that are not locally available when we are in MPI. */
        ptrdiff_t skipped = 0, checked = 0;



        auto it = toolBox->itP();

        for(it.begin(); it.end(); ++it)
        {
            bool thisResult = true;

            auto quality = HP->qualify(toolBox->getCoordFourier(it()));
            auto thisValue = phi_of_k.get(it());


            /* only do checks on entries with a hermitian partner. */
            if ( quality != HermitianRedundancy::none ) {

                std::vector<ptrdiff_t> hermPartner(nDim);

                HP->putHermitianPartner(toolBox->getCoordFourier(it()), hermPartner);


                /* make sure we get a copy, not a reference, of the partner: */
                bool dummy;
                std::complex<double> partnerValue = phi_of_k(dummy,hermPartner);

                /* partner may not have been locally available: if so, skip. */
                if ( dummy ) {
                    ++checked;
                    //                auto cpPartnerValue = partnerValue;

                    /* flip the sign of imaginary part for the equality comparison */
                    if ( quality == HermitianRedundancy::negativePartner) partnerValue = std::complex<double>(partnerValue.real(), - partnerValue.imag());

                    /* compare */
                    thisResult = AlmostEqual(thisValue, partnerValue );

                    /* real valued? Verify that imaginary part is zero. */
                    if ( quality == HermitianRedundancy::realValued ) {
                        thisResult = thisResult && AlmostEqual(thisValue.imag(), 0);
                    }

                    if ( ! thisResult ) {
                        allRight = thisResult;
                        auto coords = toolBox->getCoordConfiguration(it());
                        std::string coordsStr("(");
                        for(size_t j = 0; j<coords.size() ; ++j) coordsStr+=std::to_string(coords[j])+" ";
                        coordsStr+=")";
                        say << "Hermitian partnering fails: " << thisValue << " != " << partnerValue << " from quality: " << quality << " position: " << coordsStr << "(" << it() << ")" << " and partner position: " << hermPartner << "\n";
                        //                } else {
                        //                    say << "Match: " << quality << " " << thisValue << " " << cpPartnerValue << "\n";
                    }
                } else if ( quality != HermitianRedundancy::negativePartner) {
                    auto coords = toolBox->getCoordConfiguration(it());
                    std::string coordsStr("(");
                    for(size_t j = 0; j<coords.size() ; ++j) coordsStr+=std::to_string(coords[j])+" ";
                    coordsStr+=")";
                    say << "Wrong:mpositiv partner is out of bounds for " << coordsStr  << "\n";
                    allRight = thisResult = false;
                } else {
                    ++skipped;
                }

            }
            allRight = allRight && thisResult;
        }



        say << "skipped: " << skipped << ", checked: " << checked << ", hpartners: " << *HP << "\n";
        tdd.verify(allRight);

    };

    for ( ptrdiff_t inGhost = 0; inGhost < 2; ++inGhost) {
        for ( ptrdiff_t inDim = 2; inDim < 5; ++inDim) {
            for (ptrdiff_t inGrid = 4; inGrid < 5; ++inGrid ) {
                myLittleLambda(inDim, std::pow(2, inGrid), inGhost);
            }
        }
    }
}

#endif
