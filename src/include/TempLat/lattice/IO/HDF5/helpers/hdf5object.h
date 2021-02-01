#ifndef TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5OBJECT_H
#define TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5OBJECT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020
#ifdef HDF5


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5attribute.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5type.h"

namespace TempLat {


    /** \brief A class which group different features of hdf5 objects. They can all have attributes from example, and are all defined by an hid_t.
     *
     * 
     * Unit test: make test-hdf5object
     **/

    class HDF5Object {
    public:
        /* Put public methods here. These should change very little over time. */
        HDF5Object() {

        }

        HDF5Object(const hid_t& id) :
                mId(id)
        {

        }

        HDF5Object(hid_t&& id) :
            mId(id)
        {

        }

        operator hid_t& ()
        {
            return mId;
        }

        operator const hid_t& () const
        {
            return mId;
        }

        template<typename T>
        void addAtribute(std::string name, T value) //single value attribute
        {

            hsize_t dim = 1;
            auto dataspace_id = H5Screate_simple(1, &dim, NULL);


            auto attr_id = H5Acreate2 (mId, name.c_str(), HDF5Type<T>().type, dataspace_id,
                                       H5P_DEFAULT, H5P_DEFAULT);

            H5Awrite(attr_id, HDF5Type<T>().type, value);
            H5Aclose(attr_id);

        }

        void addAtribute(std::string name, std::string value) //single value attribute
        {
            addAtribute(name, value.c_str());
        }

        template<typename T>
        void addAtribute(std::string name, std::vector<T> values) //one-d table of value attribute
        {

            hsize_t dim = values.size();
            auto dataspace_id = H5Screate_simple(1, &dim, NULL);


            auto attr_id = H5Acreate2 (mId, name.c_str(), HDF5Type<T>().type, dataspace_id,
                                       H5P_DEFAULT, H5P_DEFAULT);


            H5Awrite(attr_id, HDF5Type<T>().type, values.data());
            H5Aclose(attr_id);
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

    protected:
        hid_t mId;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/IO/HDF5/helpers/hdf5object_test.h"
#endif
#endif

#endif
