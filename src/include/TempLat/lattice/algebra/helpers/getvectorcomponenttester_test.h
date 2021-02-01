#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENTTESTER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENTTESTER_TEST_H
/* File created by , 2019 */
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): ,  Year: YEAR

inline void TempLat::GetVectorComponentTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
  /*  auto toolBox = MemoryToolBox::makeShared(3, 32, 1);
    toolBox->setVerbose();
    FieldCollection<double> fc(3,"pimpin",toolBox);
    fc[2].inFourierSpace() = 2;
    auto test1 = GetVectorComponentHelper<FieldCollection<double>>(fc,1);
    auto test2 = GetVectorComponentHelper<FieldCollection<double>>(fc,2);

    test1.confirmSpace(toolBox->mLayouts.getConfigSpaceLayout(), SpaceStateInterface::SpaceType::Configuration);
    tdd.verify( fc[2].isFourierSpace() == true );
    test2.confirmSpace(toolBox->mLayouts.getConfigSpaceLayout(), SpaceStateInterface::SpaceType::Configuration);
    tdd.verify( fc[2].isFourierSpace() == false );

    fc[1].setGhostsAreStale();
    test2.confirmGhostsUpToDate();
    tdd.verify(fc[1].areGhostsStale() == true);
    test1.confirmGhostsUpToDate();
    tdd.verify(fc[1].areGhostsStale() == false);

    tdd.verify( test1.toString() == "pimpin#1" );
    */



}

#endif
