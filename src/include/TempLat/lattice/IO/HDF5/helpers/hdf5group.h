#ifndef TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5GROUP_H
#define TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5GROUP_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#ifdef HAVE_HDF5

#include "TempLat/lattice/IO/HDF5/helpers/hdf5object.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5type.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5dataset.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5timeseries.h"
#include "TempLat/util/stdatomictype.h"

namespace TempLat
{

  /** @brief A class which overloads hdf5 groups.
   *
   *
   * Unit test: ctest -R test-hdf5group
   **/

  herr_t print_dataset_name(hid_t group_id, const char *name, const H5L_info_t *info, void *op_data)
  {
    (void)info;
    (void)op_data;

    H5O_info_t obj_info;
    if (H5Oget_info_by_name3(group_id, name, &obj_info, 0, H5P_DEFAULT) < 0) {
      fprintf(stderr, "Failed to get info for %s\n", name);
      return -1;
    }

    if (obj_info.type == H5O_TYPE_DATASET) {
      printf("Dataset: %s\n", name);
    } else if (obj_info.type == H5O_TYPE_GROUP) {
      printf("Group: %s\n", name);
    } else {
      printf("Other object: %s\n", name);
    }

    return 0;
  }

  class HDF5Group : public HDF5Object
  {
  public:
    // Put public methods here. These should change very little over time.
    HDF5Group() = default;

    HDF5Group(const hid_t &id) : HDF5Object(id), alreadyClosed(false) {}

    HDF5Group(hid_t &&id) : HDF5Object(std::move(id)), alreadyClosed(false) {}

    ~HDF5Group() { this->close(); }

    void close()
    {
      if (!alreadyClosed) {
        H5Gclose(mId);
        alreadyClosed = true;
      }
    }

    template <typename T, typename C>
      requires requires(C c) {
        c.size();
        c[0];
      }
    HDF5Dataset createDataset(std::string name, const C &_dims)
    {
      std::vector<hsize_t> dims(_dims.size());
      for (size_t i = 0; i < _dims.size(); ++i)
        dims[i] = static_cast<hsize_t>(_dims[i]);

      auto dataspace_id = H5Screate_simple(dims.size(), dims.data(), NULL);

      HDF5Dataset ret(
          H5Dcreate2(mId, name.c_str(), HDF5Type<T>().type, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT));

      H5Sclose(dataspace_id);

      return ret;
    }

    template <typename T>
    auto createTimeSeries(std::string name, std::vector<hsize_t> dims, std::vector<hsize_t> chunks)
    {
      std::vector<hsize_t> maxdims;
      maxdims.emplace_back(H5S_UNLIMITED);
      for (size_t i = 1; i < dims.size(); ++i) {
        maxdims.emplace_back(dims[i]);
      }

      auto dataspace_id = H5Screate_simple(dims.size(), dims.data(), maxdims.data());

      hid_t prop = H5Pcreate(H5P_DATASET_CREATE);
      H5Pset_chunk(prop, chunks.size(), chunks.data());
      H5Pset_fill_time(prop, H5D_FILL_TIME_NEVER);

      HDF5TimeSeries<T> ret(H5Dcreate2(mId, name.c_str(), HDF5Type<typename std_atomic_type<T>::type>().type,
                                        dataspace_id, H5P_DEFAULT, prop, H5P_DEFAULT));

      H5Pclose(prop);
      H5Sclose(dataspace_id);

      return ret;
    }

    auto reopenDataset(std::string name) { return H5Dopen2(mId, name.c_str(), H5P_DEFAULT); }

    HDF5Group getGroup(std::string gn = "/") { return {H5Gopen(mId, gn.c_str(), H5P_DEFAULT)}; }

    HDF5Group createGroup(std::string gn)
    {
      return {H5Gcreate(mId, gn.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT)};
    }

    HDF5Group createOrOpenGroup(std::string gn)
    {
      auto status = H5Lexists(mId, gn.c_str(), H5P_DEFAULT);
      if (status > 0)
        return getGroup(gn);
      else
        return createGroup(gn);
    }

  private:
    bool alreadyClosed;
    // Put all member variables and private methods here. These may change arbitrarily.
  };
} // namespace TempLat

#endif

#endif
