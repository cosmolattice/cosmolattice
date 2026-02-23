#ifndef TEMPLAT_LATTICE_ALGEBRA_AXIONALGEBRA_ELECTRICFIELD2_H
#define TEMPLAT_LATTICE_ALGEBRA_AXIONALGEBRA_ELECTRICFIELD2_H


#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"



namespace TempLat {


    /** \brief A class which return the average between the 4 links of the magnetic field from the gauge potential. Specialise to 3D.
     *
     *
     * Unit test: make test-magneticfield
     **/

    template<typename R>
    auto electricField2(R Es, Tag<1> t){
        return 0.5 * (Es(1_c) + shift(Es(1_c), -1_c));
    }

    template<typename R>
    auto electricField2(R Es, Tag<2> t){
        return 0.5 * (Es(2_c) + shift(Es(2_c), -2_c));
    }

    template<typename R>
    auto electricField2(R Es, Tag<3> t){
        return 0.5 * (Es(3_c) + shift(Es(3_c), -3_c));
    }

    template<typename R>
    auto electricField2(R&& Es){
        return MakeVector(i, 1, 3, electricField2(Es, i));
    }

} /* TempLat */

#endif
