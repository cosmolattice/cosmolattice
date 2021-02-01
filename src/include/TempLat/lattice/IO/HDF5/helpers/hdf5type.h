#ifndef TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5TYPE_H
#define TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5TYPE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"

#ifdef HDF5
#include <hdf5.h>



namespace TempLat {


    /** \brief SFINAE class to get appropriate hdf5 type-
     *
     * 
     * Unit test: make test-hdf5type
     **/

    namespace HDF5TypeConstant {
        static constexpr int FixedSizeStringLength = 256;
    }


    template<typename T>
    struct HDF5Type{
        void close()
        {
        }
    };

    template<>
    struct HDF5Type<double>{
        HDF5Type<double>()
        {
            type = H5T_NATIVE_DOUBLE;
        }
        void close()
        {
        }
        hid_t type;
    };

    template<>
    struct HDF5Type<float>{
        HDF5Type<float>()
        {
            type = H5T_NATIVE_FLOAT;
        }
        void close()
        {
        }
        hid_t type;
    };

    template<>
    struct HDF5Type<double*>{
        HDF5Type<double*>()
        {
            type = H5T_NATIVE_DOUBLE;
        }
        void close()
        {
        }
        hid_t type;
    };

    template<>
    struct HDF5Type<float*>{
        HDF5Type<float*>()
        {
            type = H5T_NATIVE_FLOAT;
        }
        void close()
        {
        }
        hid_t type;
    };

    template<>
    struct HDF5Type<const char*>{
        HDF5Type<const char*>()
        {
            auto memtype = H5Tcopy (H5T_C_S1);
            H5Tset_size (memtype, HDF5TypeConstant::FixedSizeStringLength);
            type =  memtype;
        }
        void close()
        {
            H5Tclose(type);
        }
        hid_t type;
    };

    template<>
    struct HDF5Type<char*>{
        HDF5Type<char*>()
        {
            auto memtype = H5Tcopy (H5T_C_S1);
            H5Tset_size (memtype, HDF5TypeConstant::FixedSizeStringLength);
            type =  memtype;
        }
        void close()
        {
            H5Tclose(type);
        }
        hid_t type;
    };
    template<>
    struct HDF5Type<char[HDF5TypeConstant::FixedSizeStringLength]>{
        HDF5Type<char[HDF5TypeConstant::FixedSizeStringLength]>()
        {
            auto memtype = H5Tcopy (H5T_C_S1);
            H5Tset_size (memtype, HDF5TypeConstant::FixedSizeStringLength);
            type =  memtype;
        }
        void close()
        {
            H5Tclose(type);
        }
        hid_t type;
    };



    struct HDF5TypeTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif

    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/IO/HDF5/helpers/hdf5type_test.h"
#endif

#endif

#endif
