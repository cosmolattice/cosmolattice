#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/field/field.h"

inline void TempLat::LatticeForwardGradientTester::Test(TempLat::TDDAssertion& tdd) {



  ptrdiff_t nGrid = 32, nGhost = 1;

  auto toolBox = MemoryToolBox::makeShared(3, nGrid, nGhost);
  SpatialCoordinate x(toolBox);

  toolBox->setVerbose();

  Field<double> sc ("SC", toolBox);
  sc = getVectorComponent(x,0);

  auto fgSC = LatForwardGrad(sc).norm2();

  sc.updateGhosts(); //Need to force the ghost updating, as we do not assign the gradient to anything.

  //LayoutStruct newLayout(5);

  //fgSC.confirmSpace( newLayout, SpaceStateInterface::SpaceType::Configuration);

  //Field<double> dum("dummy",toolBox);

//  dum = 1;


  //dum =  LatForwardGrad(sc).norm2();//fgSC;

//  /* should get zero for the dimensions in which singleCoordinate is NOT returning the coordinate, one in the only dimension where it does. */
  //  tdd.verify( dum(31,8,7) == 1 );
  //  tdd.verify( dum(31,18,7) == 1 );
  //  tdd.verify( dum(15,8,17) == 1 );

  //say << dum(-33,9,8);

    bool OK = true;


    //say <<  dum(16,-15,14) ;
    auto it = toolBox->itX();

    for( it.begin(); it.end() ; ++it ){
      auto coord = toolBox->getCoordConfiguration(it());
      if( coord[0] != 16)           //coordinates range from -15 to 16.
        OK *= fgSC.get(it()) == 1;
      else
        OK *= fgSC.get(it()) == 961; // Go accross the boundary need to get (-15-16)^2 == 961
    }


    tdd.verify( OK );
//

  auto fgSC2 = LatForwardGrad(3 * sc).norm2();

  say << fgSC2.getJumps();

  OK = true;

  for( it.begin(); it.end() ; ++it ){
    auto coord = toolBox->getCoordConfiguration(it());
    if( coord[0] != 16)           //coordinates range from -15 to 16.
      OK *= fgSC2.get(it()) == 3 * 3;
    else
      OK *= fgSC2.get(it()) == 961 *9; // Go accross the boundary need to get (-15-16)^2 == 961
  }


  tdd.verify( OK );

  auto fgSC3 = LatForwardGrad(sc + sc).norm2();


  OK = true;

  for( it.begin(); it.end() ; ++it ){
    auto coord = toolBox->getCoordConfiguration(it());
    if( coord[0] != 16)           //coordinates range from -15 to 16.
      OK *= fgSC3.get(it()) == 2 * 2;
    else
      OK *= fgSC3.get(it()) == 961 *4; // Go accross the boundary need to get (-15-16)^2 == 961
  }
  tdd.verify( OK );

}

#endif
