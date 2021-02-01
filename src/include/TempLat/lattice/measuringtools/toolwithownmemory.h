#ifndef TEMPLAT_LATTICE_MEASUREMENTS_TOOLWITHOWNMEMORY_H
#define TEMPLAT_LATTICE_MEASUREMENTS_TOOLWITHOWNMEMORY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"


#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"

namespace TempLat {

    /** \brief A base class for powerspectrum and twopointcorrelator: holds an optionally
     *  persistent block of memory of the same shape as the Getter that you pass to it.
     *
     * Unit test: make test-toolwithownmemory
     **/

    template <typename T>
    class ToolWithOwnMemory {
    public:
        typedef typename GetGetReturnType<T>::type SV;
        typedef typename GetFloatType<SV>::type S;
        typedef Field<S> fieldType;

        /* Put public methods here. These should change very little over time. */
        ToolWithOwnMemory(T instance) :
        mInstance(instance),
        usePersistentField(false),
        allocatedPersistentField(false)
        {

        }

        void setPersistentMemory() {
            usePersistentField = true;
        }



    private:
        /** \brief Put in a shared_ptr so we do not need to construct upon construction. */
        T mInstance;
        std::shared_ptr<fieldType> persistentField;
        bool usePersistentField;
        bool allocatedPersistentField;

    protected:

        T& getInstance() { return mInstance; }

        /** \brief Return a new field if it should be disposed of, return the persistent storage if it should be kept. */
        fieldType getFieldForMeasurement(std::string postfix) {
            auto toolBox = mInstance.getToolBox();
            std::string fieldName = /*"[" + mInstance.toString() +*/ "  [for " + postfix + "]";
            if ( usePersistentField && ! allocatedPersistentField ) {
                allocatedPersistentField = true;
                persistentField = std::make_shared<fieldType>(fieldName, toolBox);
            }
            fieldType result = usePersistentField ? *persistentField : fieldType(fieldName, toolBox);
            result.setDisableFFTBlocking();
            return result;
        }

        friend struct ToolWithOwnMemoryTester;

    };
    struct ToolWithOwnMemoryTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/toolwithownmemory_test.h"
#endif


#endif
