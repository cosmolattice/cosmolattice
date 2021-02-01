#ifndef TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5FILE_H
#define TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5FILE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"


#ifdef HDF5

#include "TempLat/lattice/IO/HDF5/helpers/hdf5dataset.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5group.h"

namespace TempLat {


    /** \brief A class which overloads c hdf5 file interface.
     *
     * 
     * Unit test: make test-hdf5file
     **/

    enum FileMode {Overwrite, Exclusive, ReadOnly, ReadWrite};

    class HDF5File {
    public:
        /* Put public methods here. These should change very little over time. */
        HDF5File() {

        }

        void create(std::string fn, FileMode flag = Overwrite)
        {
            #ifndef NOMPI
                hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
                H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
                if(flag == Overwrite) file_id = H5Fcreate(fn.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
                if(flag == Exclusive) file_id = H5Fcreate(fn.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, plist_id);
                H5Pclose(plist_id);
            #else
                if(flag == Overwrite) file_id = H5Fcreate(fn.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
                if(flag == Exclusive) file_id = H5Fcreate(fn.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
            #endif
        }

        void open(std::string fn, FileMode flag = ReadWrite)
        {
            #ifndef NOMPI
                hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
                H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
                if(flag == ReadOnly) file_id = H5Fopen(fn.c_str(), H5F_ACC_RDONLY, plist_id);
                if(flag == ReadWrite) file_id = H5Fopen(fn.c_str(), H5F_ACC_RDWR,  plist_id);
                H5Pclose(plist_id);
            #else
                if(flag == ReadOnly) file_id = H5Fopen(fn.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
                if(flag == ReadWrite) file_id = H5Fopen(fn.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
            #endif
        }

        void close()
        {
            H5Fclose(file_id);
        }

        HDF5Group getGroup(std::string gn = "/")
        {
            return {H5Gopen(file_id, gn.c_str(), H5P_DEFAULT)};
        }

        HDF5Group createGroup(std::string gn)
        {
            return {H5Gcreate(file_id, gn.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT)};
        }

        HDF5Group createOrOpenGroup(std::string gn)
        {
            auto status = H5Eset_auto(H5P_DEFAULT, NULL, NULL);
            status = H5Gget_objinfo (file_id, gn.c_str(), 0, NULL);
            H5Eset_auto(H5P_DEFAULT, NULL, NULL);
            if(status == 0) return getGroup(gn);
            else return createGroup(gn);
        }

        template<typename T, typename inttype>
        HDF5Dataset createDataset(std::string name, std::vector<inttype> dims)
        {
            return getGroup().createDataset<T>(name,dims);
        }


        HDF5Dataset openDataset(std::string name)
        {
            name = "/" + name;
            return {H5Dopen2(file_id, name.c_str(), H5P_DEFAULT)};
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        hid_t file_id;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/IO/HDF5/helpers/hdf5file_test.h"
#endif
#endif

#endif
