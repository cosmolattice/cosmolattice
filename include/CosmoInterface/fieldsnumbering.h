#ifndef COSMOINTERFACE_FIELDSNUMBERING_H
#define COSMOINTERFACE_FIELDSNUMBERING_H
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio  Year: 2024

#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/util/rangeiteration/for_in_range.h"

namespace TempLat {


    /** \brief A class which defines an ordering for the fields, to be used across the interface.
     *
     *
     **/

    class FieldsNumbering {
    public:
        /* Put public methods here. These should change very little over time. */
        FieldsNumbering() = delete;


        //Programatically, it can be useful to access the field by some number. The numbering is defined here.
        using fldS = Tag<0>;
        using piS = Tag<1>;
        using fldCS = Tag<2>;
        using piCS = Tag<3>;
        using fldSU2Doublet = Tag<4>;
        using piSU2Doublet = Tag<5>;
        using fldU1 = Tag<6>;
        using piU1 = Tag<7>;
        using fldSU2 = Tag<8>;
        using piSU2 = Tag<9>;
        //TODO: GW are not fully supported by RK2N, which is the only place that uses this mechanism.
        //static constexpr size_t fldGWNum = 10;
        //static constexpr size_t piGWNum = 11;
        static constexpr size_t maxNum = 9;


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

    };



} /* TempLat */

// Iterate over all field instances: calls expr with fld=Tag<K> and n=Tag<J>
// for each active species K and each instance J in [0, getNFields-1].
#define ForEachField(Model, fld, n, expr) \
    ForLoop(fld, 0, TempLat::FieldsNumbering::maxNum, \
        if constexpr (Model::getNFields(fld) > 0) { \
            ForLoop(n, 0, Model::getNFields(fld) - 1, expr); \
        } \
    )

// Iterate over active field species only: calls expr with fld=Tag<K>
// for each species K with getNFields > 0.
#define ForEachSpecies(Model, fld, expr) \
    ForLoop(fld, 0, TempLat::FieldsNumbering::maxNum, \
        if constexpr (Model::getNFields(fld) > 0) { expr; } \
    )

#endif
