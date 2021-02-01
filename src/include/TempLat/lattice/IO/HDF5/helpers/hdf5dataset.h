#ifndef TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5DATASET_H
#define TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5DATASET_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"


#ifdef HDF5

#include "TempLat/lattice/IO/HDF5/helpers/hdf5type.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5object.h"



namespace TempLat {


    /** \brief A class which overloads hdf5 dataset and implement relevant write method.
     *
     * 
     * Unit test: make test-hdf5dataset
     **/

    //using HDF5Dataset = hid_t;

    class HDF5Dataset : public HDF5Object {
    public:
        // Put public methods here. These should change very little over time.
        HDF5Dataset():
                        alreadyClosed(false)
        {

        }


        HDF5Dataset(const hid_t& id) :
        HDF5Object(id),
        alreadyClosed(false)

        {
        }

        HDF5Dataset(hid_t&& id) :
         HDF5Object(id),
         alreadyClosed(false)

        {
        }
        ~HDF5Dataset(){
          //  this->close();
        }

        void close()
        {
            if(!alreadyClosed) {
                H5Dclose(mId);
                alreadyClosed = true;
            }
        }
        template<typename T>
        void write(std::vector<T> data)
        {
            HDF5Type<T> type;
#ifndef NOMPI
            hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
                //H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
                H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);
                H5Dwrite(mId, type.type, H5S_ALL, H5S_ALL, plist_id, data.data());
                H5Pclose(plist_id);
#else
            H5Dwrite(mId, type.type, H5S_ALL, H5S_ALL, H5P_DEFAULT, data.data());
#endif
             type.close();

        }


        template<typename T>
        void writeSlices(std::vector<T> data, const std::vector<hsize_t>& subdims, const std::vector<hsize_t>& offsets)
        {

            auto mNDimensions = subdims.size();
            std::vector<hsize_t> strides(mNDimensions,1);
            std::vector<hsize_t> blocks(mNDimensions,1);

            auto memspace_id = H5Screate_simple (mNDimensions, subdims.data(), NULL);
            auto dataspace_id = H5Dget_space (mId);

            H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, offsets.data(),
                                               strides.data(), subdims.data(), blocks.data());
            HDF5Type<T> type;
            #ifndef NOMPI
                hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
                //H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
                H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);
                H5Dwrite(mId, type.type, memspace_id,
                      dataspace_id, plist_id, data.data());
                H5Pclose(plist_id);
            #else
                H5Dwrite(mId, type.type, memspace_id,
                      dataspace_id, H5P_DEFAULT, data.data());
            #endif

              H5Sclose(dataspace_id);
              H5Sclose(memspace_id);
              type.close();

        }



        template<typename T>
        void writeSlices(std::vector<T> data, const std::vector<ptrdiff_t>& subdims, const std::vector<ptrdiff_t>& offsets)
        {
            std::vector<hsize_t> tmp1, tmp2;
            for(size_t i = 0; i< subdims.size(); ++i)
            {
                tmp1.emplace_back(subdims[i]);
                tmp2.emplace_back(offsets[i]);
            }
            writeSlices(data, tmp1, tmp2);
        }

        template<typename T>
        void writeSlices(std::vector<T> data, const std::vector<size_t>& subdims, const std::vector<size_t>& offsets)
        {
            std::vector<hsize_t> tmp1, tmp2;
            for(size_t i = 0; i< subdims.size(); ++i)
            {
                tmp1.emplace_back(subdims[i]);
                tmp2.emplace_back(offsets[i]);
            }
            writeSlices(data, tmp1, tmp2);
        }

        template<typename T>
        void writeElement(T data, const std::vector<hsize_t>& offsets)
        {

            auto mNDimensions = offsets.size();
            std::vector<hsize_t> strides(mNDimensions,1);
            std::vector<hsize_t> blocks(mNDimensions,1);
            std::vector<hsize_t> subdims(mNDimensions,1);

            auto memspace_id = H5Screate_simple (mNDimensions, subdims.data(), NULL);
            auto dataspace_id = H5Dget_space (mId);

            auto err1 = H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, offsets.data(),
                                 strides.data(), subdims.data(), blocks.data());
           HDF5Type<T> type;

            #ifndef NOMPI
               hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
                //H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
                auto err3 = H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);
                auto err2 = H5Dwrite(mId, type.type, memspace_id,
                      dataspace_id, plist_id, data);
                H5Pclose(plist_id);
                H5Eclose_stack(err3);

            #else
              auto err2 = H5Dwrite(mId, type.type, memspace_id,
                      dataspace_id, H5P_DEFAULT, data);
            #endif


              H5Sclose(dataspace_id);
              H5Sclose(memspace_id);
              H5Eclose_stack(err1);
              H5Eclose_stack(err2);
              type.close();
        }



        template<typename T>
        void readSlices(std::vector<T>& data, const std::vector<hsize_t>& subdims, const std::vector<hsize_t>& offsets)
        {

            auto mNDimensions = subdims.size();
            std::vector<hsize_t> strides(mNDimensions,1);
            std::vector<hsize_t> blocks(mNDimensions,1);

            auto memspace_id = H5Screate_simple (mNDimensions, subdims.data(), NULL);
            auto dataspace_id = H5Dget_space (mId);
            H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, offsets.data(),
                                               strides.data(), subdims.data(), blocks.data());
            HDF5Type<T> type;

            #ifndef NOMPI
                hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
                //H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
                H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);

                H5Dread(mId, type.type, memspace_id,
                      dataspace_id, plist_id, data.data());
                H5Pclose(plist_id);
            #else
                H5Dread(mId, type.type, memspace_id,
                      dataspace_id, H5P_DEFAULT, data.data());
            #endif
              H5Sclose(dataspace_id);
              H5Sclose(memspace_id);
              type.close();

        }

        template<typename T>
        void readElement(T data, const std::vector<hsize_t>& offsets)
        {

            auto mNDimensions = offsets.size();
            std::vector<hsize_t> strides(mNDimensions,1);
            std::vector<hsize_t> blocks(mNDimensions,1);
            std::vector<hsize_t> subdims(mNDimensions,1);


            auto memspace_id = H5Screate_simple (mNDimensions, subdims.data(), NULL);
            auto dataspace_id = H5Dget_space (mId);
            H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, offsets.data(),
                                 strides.data(), subdims.data(), blocks.data());
            HDF5Type<T> type;

#ifndef NOMPI
            hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
                //H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
                H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);
                H5Dread(mId, type.type, memspace_id,
                      dataspace_id, plist_id, data);
                H5Pclose(plist_id);
#else
            H5Dread(mId, type.type, memspace_id,
                    dataspace_id, H5P_DEFAULT, data);
#endif
              H5Sclose(dataspace_id);
              H5Sclose(memspace_id);
              type.close();

        }


    private:

        // Put all member variables and private methods here. These may change arbitrarily.

        bool alreadyClosed;

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/IO/HDF5/helpers/hdf5dataset_test.h"
#endif
#endif

#endif
