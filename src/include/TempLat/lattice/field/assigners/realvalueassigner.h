#ifndef TEMPLAT_LATTICE_FIELD_ASSIGNERS_REALVALUEASSIGNER_H
#define TEMPLAT_LATTICE_FIELD_ASSIGNERS_REALVALUEASSIGNER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien,  Year: 2019

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/memory/memorymanager.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"


namespace TempLat {


    /** \brief A class which outsource the assignement operator.
     *
     * 
     * Unit test: make test-realvalueassigner
     **/

    template <typename R, typename  T >
    class RealValueAssigner {
    public:
        /* Put public methods here. These should change very little over time. */
        RealValueAssigner(R pR, Field<T> fld) : mR(pR),
        mManager(fld.getMemoryManager()) {

        }

        void operator()(ptrdiff_t i)
        {
            mManager->operator[](i) = GetValue::get(mR,i);
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
    R mR;
    std::shared_ptr<MemoryManager<T>> mManager;


    };

    struct RealValueAssignerTester
    {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
    template <typename R, typename  T>
    auto MakeRealValueAssigner(R pR, Field<T> fld)
    {
        return RealValueAssigner<R, T>(pR,fld);
    }


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/assigners/realvalueassigner_test.h"
#endif


#endif
