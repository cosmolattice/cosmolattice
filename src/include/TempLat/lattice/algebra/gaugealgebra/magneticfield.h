#ifndef TEMPLAT_LATTICE_ALGEBRA_GAUGEALGEBRA_MAGNETICFIELD_H
#define TEMPLAT_LATTICE_ALGEBRA_GAUGEALGEBRA_MAGNETICFIELD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/lattice/algebra/gaugealgebra/fieldstrength.h"

namespace TempLat {


    /** \brief A class which return the magnetic field from the gauge potential. Specialise to 3D.
     *
     * 
     * Unit test: make test-magneticfield
     **/

    template<typename R>
    auto magneticField(R As, Tag<1> t){
        return fieldStrength(As, 2_c, 3_c);
    }

    template<typename R>
    auto magneticField(R&& As, Tag<2> t){
        return fieldStrength(As, 3_c, 1_c);
    }

    template<typename R>
    auto magneticField(R&& As, Tag<3> t){
        return fieldStrength(As, 1_c, 2_c);
    }

    template<typename R>
    auto magneticField(R&& As){
        return MakeVector(i, 1, 3, magneticField(As, i));
    }




} /* TempLat */

#include "TempLat/lattice/algebra/gaugealgebra/magneticfield_test.h"


#endif
