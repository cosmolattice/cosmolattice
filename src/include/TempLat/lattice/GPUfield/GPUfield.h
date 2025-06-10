#ifndef TEMPLAT_LATTICE_GPUFIELD_FIELD_H_GPU
#define TEMPLAT_LATTICE_GPUFIELD_FIELD_H_GPU

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/GPUfield/views/GPUfieldviewconfig.h"
//#include "TempLat/lattice/GPUfield/views/fieldviewfourier.h"
//#include "TempLat/lattice/algebra/constants/zerotype.h"
//#include "TempLat/lattice/algebra/constants/onetype.h"
//#include "TempLat/lattice/algebra/constants/halftype.h"

namespace TempLat {
    /** \brief A class which is a classical field on your n-dimensional equisized grid.
     * You use it as a scalar field, a vector component, whatever.
     * Template parameter is your type of floating point precision: float or double. Default: double.
     *
     *  Implements a get method, and is hence suitable for all algebra.
     *
     * Unit test: make test-field
     **/
    template<typename T>
    class GPUField : public GPUConfigView<T> {
    public:
        /* Put public methods here. These should change very little over time. */

        GPUField(std::string name, LatticeParameters<T> pLatPar = LatticeParameters<T>())
            : GPUConfigView<T>(name, pLatPar) {
        }

        template<typename R>
        KOKKOS_FORCEINLINE_FUNCTION
        void operator=(R &&g) {
            GPUConfigView<T>::operator=(g);
        }

        KOKKOS_FORCEINLINE_FUNCTION
        void operator=(const GPUField<T> &other) {
            //overwrite the default = operator.
            operator=(1 * other);
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

    public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/GPUfield/GPUfield_test.h"
#endif


#endif
