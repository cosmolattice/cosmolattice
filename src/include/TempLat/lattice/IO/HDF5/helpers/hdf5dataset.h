#ifndef TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5DATASET_H
#define TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5DATASET_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#ifdef HAVE_HDF5

#include <vector>
#include <stdexcept>
#include "TempLat/lattice/IO/HDF5/helpers/hdf5type.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5object.h"
#include "TempLat/util/stdatomictype.h"

namespace TempLat
{

  /** @brief A class which overloads hdf5 dataset and implement relevant write method.
   *
   *
   * Unit test: ctest -R test-hdf5dataset
   **/
  class HDF5Dataset : public HDF5Object
  {
  public:
    // Put public methods here. These should change very little over time.
    HDF5Dataset() : alreadyClosed(false) {}

    HDF5Dataset(const hid_t &id) : HDF5Object(id), alreadyClosed(false) {}

    HDF5Dataset(hid_t &&id) : HDF5Object(std::move(id)), alreadyClosed(false) {}
    ~HDF5Dataset() = default;

    void close()
    {
      if (!alreadyClosed) {
        H5Dclose(mId);
        alreadyClosed = true;
      }
    }

    template <typename T> void write(std::vector<T> data)
    {
      HDF5Type<T> type;
#ifdef HAVE_MPI
      hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
      // H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
      H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);
      H5Dwrite(mId, type.type, H5S_ALL, H5S_ALL, plist_id, data.data());
      H5Pclose(plist_id);
#else
      H5Dwrite(mId, type.type, H5S_ALL, H5S_ALL, H5P_DEFAULT, data.data());
#endif
      type.close();
    }

    template <typename T, typename C>
      requires requires(C c) {
        c.size();
        c[0];
      }
    void writeSlices(std::vector<T> data, const C &_subdims, const C &_offsets, bool doIWrite = true)
    {
      if (_subdims.size() != _offsets.size())
        throw std::runtime_error("In HDF5Dataset::writeSlices, subdims and offsets must have the same size");

      std::vector<hsize_t> subdims(_subdims.size());
      std::vector<hsize_t> offsets(_offsets.size());

      for (size_t i = 0; i < _subdims.size(); ++i) {
        subdims[i] = static_cast<hsize_t>(_subdims[i]);
        offsets[i] = static_cast<hsize_t>(_offsets[i]);
      }

      auto mNDimensions = subdims.size();
      std::vector<hsize_t> strides(mNDimensions, 1);
      std::vector<hsize_t> blocks(mNDimensions, 1);

      auto memspace_id = H5Screate_simple(mNDimensions, subdims.data(), NULL);
      auto dataspace_id = H5Dget_space(mId);

      if (doIWrite) {
        H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offsets.data(), strides.data(), subdims.data(),
                            blocks.data());
      } else {
        H5Sselect_none(dataspace_id);
      }
      HDF5Type<typename std_atomic_type<T>::type> type;
#ifdef HAVE_MPI
      hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
      // H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
      H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);
      if (doIWrite) H5Dwrite(mId, type.type, memspace_id, dataspace_id, plist_id, data.data());
      H5Pclose(plist_id);
#else
      H5Dwrite(mId, type.type, memspace_id, dataspace_id, H5P_DEFAULT, data.data());
#endif

      H5Sclose(dataspace_id);
      H5Sclose(memspace_id);
      type.close();
    }

    template <typename T, typename C>
      requires requires(C c) {
        c.size();
        c[0];
      }
    void writeElement(T data, const C &_offsets)
    {
      std::vector<hsize_t> offsets(_offsets.size());
      for (size_t i = 0; i < _offsets.size(); ++i)
        offsets[i] = static_cast<hsize_t>(_offsets[i]);

      auto mNDimensions = offsets.size();
      std::vector<hsize_t> strides(mNDimensions, 1);
      std::vector<hsize_t> blocks(mNDimensions, 1);
      std::vector<hsize_t> subdims(mNDimensions, 1);

      auto memspace_id = H5Screate_simple(mNDimensions, subdims.data(), NULL);
      auto dataspace_id = H5Dget_space(mId);

      auto err1 = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offsets.data(), strides.data(), subdims.data(),
                                      blocks.data());
      HDF5Type<T> type;

#ifdef HAVE_MPI
      hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
      // H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
      auto err3 = H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);
      auto err2 = H5Dwrite(mId, type.type, memspace_id, dataspace_id, plist_id, &data);
      H5Pclose(plist_id);
      H5Eclose_stack(err3);

#else
      auto err2 = H5Dwrite(mId, type.type, memspace_id, dataspace_id, H5P_DEFAULT, &data);
#endif

      H5Sclose(dataspace_id);
      H5Sclose(memspace_id);
      H5Eclose_stack(err1);
      H5Eclose_stack(err2);
      type.close();
    }

    template <typename T, typename C>
      requires requires(C c) {
        c.size();
        c[0];
      }
    void readSlices(std::vector<T> &data, const C &_subdims, const C &_offsets)
    {
      if (_subdims.size() != _offsets.size())
        throw std::runtime_error("In HDF5Dataset::readSlices, subdims and offsets must have the same size");

      std::vector<hsize_t> subdims(_subdims.size());
      std::vector<hsize_t> offsets(_offsets.size());

      for (size_t i = 0; i < _subdims.size(); ++i) {
        subdims[i] = static_cast<hsize_t>(_subdims[i]);
        offsets[i] = static_cast<hsize_t>(_offsets[i]);
      }

      auto mNDimensions = subdims.size();
      std::vector<hsize_t> strides(mNDimensions, 1);
      std::vector<hsize_t> blocks(mNDimensions, 1);

      auto memspace_id = H5Screate_simple(mNDimensions, subdims.data(), NULL);
      auto dataspace_id = H5Dget_space(mId);
      H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offsets.data(), strides.data(), subdims.data(), blocks.data());
      HDF5Type<T> type;

#ifdef HAVE_MPI
      hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
      // H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
      H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);

      H5Dread(mId, type.type, memspace_id, dataspace_id, plist_id, data.data());
      H5Pclose(plist_id);
#else
      H5Dread(mId, type.type, memspace_id, dataspace_id, H5P_DEFAULT, data.data());
#endif
      H5Sclose(dataspace_id);
      H5Sclose(memspace_id);
      type.close();
    }

    template <typename T, typename C>
      requires requires(C c) {
        c.size();
        c[0];
      }
    void readElement(T data, const C &_offsets)
    {
      std::vector<hsize_t> offsets(_offsets.size());
      for (size_t i = 0; i < _offsets.size(); ++i)
        offsets[i] = static_cast<hsize_t>(_offsets[i]);

      auto mNDimensions = offsets.size();
      std::vector<hsize_t> strides(mNDimensions, 1);
      std::vector<hsize_t> blocks(mNDimensions, 1);
      std::vector<hsize_t> subdims(mNDimensions, 1);

      auto memspace_id = H5Screate_simple(mNDimensions, subdims.data(), NULL);
      auto dataspace_id = H5Dget_space(mId);
      H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offsets.data(), strides.data(), subdims.data(), blocks.data());
      HDF5Type<T> type;

#ifdef HAVE_MPI
      hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
      // H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
      H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);
      H5Dread(mId, type.type, memspace_id, dataspace_id, plist_id, data);
      H5Pclose(plist_id);
#else
      H5Dread(mId, type.type, memspace_id, dataspace_id, H5P_DEFAULT, data);
#endif
      H5Sclose(dataspace_id);
      H5Sclose(memspace_id);
      type.close();
    }

    std::vector<hsize_t> getSizes()
    {
      hid_t space_id = H5Dget_space(mId);
      int ndims = H5Sget_simple_extent_ndims(space_id);
      std::vector<hsize_t> dims(ndims);
      H5Sget_simple_extent_dims(space_id, dims.data(), NULL);
      H5Sclose(space_id);
      return dims;
    }

    void reopen(hid_t pId)
    {
      if (alreadyClosed) {
        mId = pId;
        alreadyClosed = false;
      }
    }

  private:
    // Put all member variables and private methods here. These may change arbitrarily.

    bool alreadyClosed;
  };

} // namespace TempLat

#endif

#endif
