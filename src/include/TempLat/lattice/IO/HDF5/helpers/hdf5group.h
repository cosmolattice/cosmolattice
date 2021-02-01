#ifndef TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5GROUP_H
#define TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5GROUP_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020
#ifdef  HDF5

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5object.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5type.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5dataset.h"


namespace TempLat {


    /** \brief A class which overloads hdf5 groups.
     *
     * 
     * Unit test: make test-hdf5group
     **/

    class HDF5Group : public HDF5Object {
    public:
        // Put public methods here. These should change very little over time.
        HDF5Group() {

        }

        HDF5Group(const hid_t& id) :
                HDF5Object(id),
                alreadyClosed(false)
        {

        }

        HDF5Group(hid_t&& id) :
                HDF5Object(id),
                alreadyClosed(false)
        {

        }

        ~HDF5Group(){
            this->close();
        }



        void close()
        {
            if(!alreadyClosed) {
                H5Gclose(mId);
                alreadyClosed = true;
            }
        }

        template<typename T>
        HDF5Dataset createDataset(std::string name, std::vector<hsize_t> dims)
        {
            auto dataspace_id = H5Screate_simple (dims.size(), dims.data(), NULL);


            HDF5Dataset ret(H5Dcreate2 (mId, name.c_str(), HDF5Type<T>().type, dataspace_id,
                                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT) );

            H5Sclose(dataspace_id);

            return ret;
        }

        template<typename T>
        HDF5Dataset createDataset(std::string name, std::vector<size_t> dims)
        {
            std::vector<hsize_t> tmp;
            for(hsize_t x : dims) tmp.emplace_back(x);
            return createDataset<T>(name, tmp);
        }
        template<typename T>
        HDF5Dataset createDataset(std::string name, std::vector<ptrdiff_t> dims)
        {
            std::vector<hsize_t> tmp;
            for(hsize_t x : dims) tmp.emplace_back(x);
            return createDataset<T>(name, tmp);
        }


        HDF5Group getGroup(std::string gn = "/")
        {
            return {H5Gopen(mId, gn.c_str(), H5P_DEFAULT)};
        }

        HDF5Group createGroup(std::string gn)
        {
            return {H5Gcreate(mId, gn.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT)};
        }

        HDF5Group createOrOpenGroup(std::string gn)
        {
            auto status = H5Eset_auto(H5P_DEFAULT, NULL, NULL);
            status = H5Gget_objinfo (mId, gn.c_str(), 0, NULL);
            H5Eset_auto(H5P_DEFAULT, NULL, NULL);
            if(status == 0) return getGroup(gn);
            else return createGroup(gn);
        }


    private:

        bool alreadyClosed;
        // Put all member variables and private methods here. These may change arbitrarily.

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/IO/HDF5/helpers/hdf5group_test.h"
#endif

#endif

#endif
