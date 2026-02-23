#ifndef TEMPLAT_LATTICE_ALGEBRA_AXIONALGEBRA_MAGNETICFIELD4_H
#define TEMPLAT_LATTICE_ALGEBRA_AXIONALGEBRA_MAGNETICFIELD4_H


#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"



namespace TempLat {


    /** \brief A class which return the average between the 4 links of the magnetic field from the gauge potential. Specialise to 3D.
     *
     *
     * Unit test: make test-magneticfield
     **/

    template<typename R>
    auto magneticField4(R Bs, Tag<1> t){
        return 0.25 * (Bs + shift(Bs, -2_c) + shift(Bs, -3_c) + shift(shift(Bs, -2_c), -3_c));
    }

    template<typename R>
    auto magneticField4(R&& Bs, Tag<2> t){
        return  0.25 * (Bs + shift(Bs, -1_c) + shift(Bs, -3_c) + shift(shift(Bs, -1_c), -3_c));
    }

    template<typename R>
    auto magneticField4(R&& Bs, Tag<3> t){
        return  0.25 * (Bs + shift(Bs, -1_c) + shift(Bs, -2_c) + shift(shift(Bs, -2_c), -1_c));
    }

    template<typename R>
    auto magneticField4(R&& Bs){
        return MakeVector(i, 1, 3, magneticField4(Bs, i));
    }

} /* TempLat */

#endif
