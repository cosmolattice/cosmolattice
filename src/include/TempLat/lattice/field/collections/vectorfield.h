#ifndef COSMOINTERFACE_3D_VECTORFIELD3D_H
#define COSMOINTERFACE_3D_VECTORFIELD3D_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/field/collections/fieldcollection.h"
#include "TempLat/util/tdd/tdd.h"




namespace TempLat {


    /** \brief A class which
     * Field collections. Allows to have vector fields, index starting from one.
     *
     *
     * Unit test: make test-vectorfield3d
     **/
    template <template <class> class Arg, class T, int N, bool flatAssign = false>
            using VectorField = FieldCollection<Arg, T, N, flatAssign, 1>;


} /* TempLat */



#endif
