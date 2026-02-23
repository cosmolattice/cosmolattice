#ifndef TEMPLAT_LATTICE_IO_HDF5_FILELOADERHDF5_H
#define TEMPLAT_LATTICE_IO_HDF5_FILELOADERHDF5_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#ifdef HAVE_HDF5

#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>
#include "TempLat/lattice/IO/HDF5/helpers/hdf5file.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/parameters/parameterparser.h"

#include "TempLat/parallel/device.h"
#include "TempLat/parallel/device_memory.h"

namespace TempLat
{

  /** @brief A class which implements loading in pure HDF5.
   *
   *
   * Unit test: ctest -R test-fileloaderhdf5
   **/
  class FileLoaderHDF5
  {
  public:
    // Put public methods here. These should change very little over time.

    FileLoaderHDF5() = default;

    void open(std::string fn) { mFile.open(fn); }
    void close() { mFile.close(); }
    void reset() { this->close(); }

    void load(ParameterParser &par)
    {
      mDataset = mFile.openDataset("Parameters");

      std::vector<std::string> parStr;
      char tmp[HDF5TypeConstant::FixedSizeStringLength];

      // get number of parameters stored.
      hid_t dspace = H5Dget_space(mDataset);
      const int ndims = H5Sget_simple_extent_ndims(dspace);
      std::vector<hsize_t> dims(ndims);
      H5Sget_simple_extent_dims(dspace, dims.data(), NULL);
      auto nElements = dims[0];

      for (size_t i = 0; i < nElements; ++i) {
        mDataset.readElement(tmp, std::vector<hsize_t>(1, i));
        parStr.emplace_back(tmp);
      }
      H5Sclose(dspace);
      par.addFromVector(parStr);
      mDataset.close();
    }

    template <typename R> void load(R &t, std::string name)
    { // used to store a number. The name is the one of the dataset which contains this number.
      mDataset = mFile.openDataset(name);
      mDataset.readElement(&t, std::vector<hsize_t>(1, 0));
      mDataset.close();
    }

    /**
     * @brief Load a double scalar from a named dataset
     * @param value The value to load into
     * @param name Dataset name
     *
     * This overload explicitly handles double
     */
    void load(double &value, const std::string &name)
    {
      // Use "/" prefix for root group (matching openDataset pattern)
      std::string fullName = "/" + name;
      auto dataset = H5Dopen2(mFile.getHandle(), fullName.c_str(), H5P_DEFAULT);

      H5Dread(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &value);

      H5Dclose(dataset);
    }

    /**
     * @brief Load a string from a named dataset
     * @param str String to load into
     * @param name Dataset name
     *
     * Uses a large fixed-size buffer (8KB) to accommodate RNG states (~5KB)
     */
    void load(std::string &str, const std::string &name)
    {
      constexpr size_t LargeStringLength = 16384; // 16KB for combined RNG states

      std::vector<char> buffer(LargeStringLength, 0);

      // Use "/" prefix for root group (matching openDataset pattern)
      std::string fullName = "/" + name;
      auto dataset = H5Dopen2(mFile.getHandle(), fullName.c_str(), H5P_DEFAULT);
      auto dtype = H5Dget_type(dataset);
      size_t typeSize = H5Tget_size(dtype);

      // Create memory type matching the file type size
      auto memtype = H5Tcopy(H5T_C_S1);
      H5Tset_size(memtype, typeSize);

      H5Dread(dataset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, buffer.data());

      H5Tclose(memtype);
      H5Tclose(dtype);
      H5Dclose(dataset);

      str = std::string(buffer.data());
    }

    /**
     * @brief Load per-rank string data from a shared dataset (parallel HDF5 safe)
     * @param str String to load into
     * @param name Dataset name (shared across all ranks)
     * @param mpiRank This rank's index
     *
     * Reads from a dataset of size [nRanks] where each rank reads its element.
     */
    void loadPerRank(std::string &str, const std::string &name, int mpiRank)
    {
      constexpr size_t LargeStringLength = 16384;

      std::vector<char> buffer(LargeStringLength, 0);

      std::string fullName = "/" + name;
      auto dataset = H5Dopen2(mFile.getHandle(), fullName.c_str(), H5P_DEFAULT);
      auto dtype = H5Dget_type(dataset);
      size_t typeSize = H5Tget_size(dtype);

      // Create memory type matching file type size
      auto memtype = H5Tcopy(H5T_C_S1);
      H5Tset_size(memtype, typeSize);

      // Select hyperslab for this rank's element
      auto filespace = H5Dget_space(dataset);
      hsize_t start[1] = {static_cast<hsize_t>(mpiRank)};
      hsize_t count[1] = {1};
      H5Sselect_hyperslab(filespace, H5S_SELECT_SET, start, nullptr, count, nullptr);

      // Memory space for single element
      hsize_t memDims[1] = {1};
      auto memspace = H5Screate_simple(1, memDims, nullptr);

      // Read with independent I/O
      auto plist = H5Pcreate(H5P_DATASET_XFER);
#ifdef HAVE_MPI
      H5Pset_dxpl_mpio(plist, H5FD_MPIO_INDEPENDENT);
#endif
      H5Dread(dataset, memtype, memspace, filespace, plist, buffer.data());

      H5Pclose(plist);
      H5Sclose(memspace);
      H5Sclose(filespace);
      H5Tclose(memtype);
      H5Tclose(dtype);
      H5Dclose(dataset);

      str = std::string(buffer.data());
    }

    /**
     * @brief Load per-rank double data from a shared dataset (parallel HDF5 safe)
     * @param value The value to load into
     * @param name Dataset name (shared across all ranks)
     * @param mpiRank This rank's index
     */
    void loadPerRank(double &value, const std::string &name, int mpiRank)
    {
      std::string fullName = "/" + name;
      auto dataset = H5Dopen2(mFile.getHandle(), fullName.c_str(), H5P_DEFAULT);

      // Select hyperslab for this rank's element
      auto filespace = H5Dget_space(dataset);
      hsize_t start[1] = {static_cast<hsize_t>(mpiRank)};
      hsize_t count[1] = {1};
      H5Sselect_hyperslab(filespace, H5S_SELECT_SET, start, nullptr, count, nullptr);

      // Memory space for single element
      hsize_t memDims[1] = {1};
      auto memspace = H5Screate_simple(1, memDims, nullptr);

      // Read with independent I/O
      auto plist = H5Pcreate(H5P_DATASET_XFER);
#ifdef HAVE_MPI
      H5Pset_dxpl_mpio(plist, H5FD_MPIO_INDEPENDENT);
#endif
      H5Dread(dataset, H5T_NATIVE_DOUBLE, memspace, filespace, plist, &value);

      H5Pclose(plist);
      H5Sclose(memspace);
      H5Sclose(filespace);
      H5Dclose(dataset);
    }

    /**
     * @brief Load RNG state from binary uint64_t array and reconstruct text string
     * @param textState Output: reconstructed text state for loadState()
     * @param name Dataset name
     * @param mpiRank This rank's index
     */
    void loadRNGStateBinary(std::string &textState, const std::string &name, int mpiRank)
    {
      std::string fullName = "/" + name;
      auto dataset = H5Dopen2(mFile.getHandle(), fullName.c_str(), H5P_DEFAULT);
      auto filespace = H5Dget_space(dataset);

      hsize_t dims[2];
      H5Sget_simple_extent_dims(filespace, dims, nullptr);
      size_t stateSize = dims[1];

      hsize_t start[2] = {static_cast<hsize_t>(mpiRank), 0};
      hsize_t count[2] = {1, stateSize};
      H5Sselect_hyperslab(filespace, H5S_SELECT_SET, start, nullptr, count, nullptr);

      hsize_t memDims[1] = {stateSize};
      auto memspace = H5Screate_simple(1, memDims, nullptr);
      std::vector<uint64_t> binaryState(stateSize);

      auto plist = H5Pcreate(H5P_DATASET_XFER);
#ifdef HAVE_MPI
      H5Pset_dxpl_mpio(plist, H5FD_MPIO_INDEPENDENT);
#endif
      H5Dread(dataset, H5T_NATIVE_UINT64, memspace, filespace, plist, binaryState.data());

      H5Pclose(plist);
      H5Sclose(memspace);
      H5Dclose(dataset);
      H5Sclose(filespace);

      std::ostringstream oss;

      if (stateSize == 313) {
        for (size_t i = 0; i < stateSize; ++i) {
          if (i > 0) oss << " ";
          oss << binaryState[i];
        }
      } else if (stateSize == 316) {
        for (size_t i = 0; i < 313; ++i) {
          if (i > 0) oss << " ";
          oss << binaryState[i];
        }
        oss << "\n";
        oss << binaryState[313] << " " << binaryState[314] << " ";
        if (binaryState[314]) {
          double cachedValue;
          std::memcpy(&cachedValue, &binaryState[315], sizeof(double));
          oss << std::setprecision(17) << cachedValue;
        }
      } else {
        for (size_t i = 0; i < stateSize; ++i) {
          if (i > 0) oss << " ";
          oss << binaryState[i];
        }
      }

      textState = oss.str();
    }

    template <typename R> void load(R r)
    {
      mDataset = mFile.openDataset(GetString::get(r));
      loadDim(r, 0, {});
      mDataset.close();
    }

    template <typename R> void loadDim(R r, int dim, std::vector<ptrdiff_t> coords)
    {
      auto toolBox = r.getToolBox();

      constexpr size_t NDim = std::decay_t<decltype(*toolBox)>::NDim;

      auto starts = toolBox->mLayouts.getConfigSpaceStarts(); // Local mpi offset.
      auto sizes = toolBox->mLayouts.getConfigSpaceSizes();   // Local mpi sizes.

      const auto mLayout = toolBox->mLayouts.getConfigSpaceLayout();

      if ((size_t)dim == toolBox->NDim - 1) // Last dimension, saved as a full rod.
      {
        // look at index 0 in the last dimension. The next nGrid[last dimension] points are stored continuously.
        coords.emplace_back(0);

        // for hdf5, tell it we want to store a sub array of size (1,1,1...,nGrid[last dimension]).
        std::vector<hsize_t> subdims(toolBox->NDim, 1);
        subdims.back() = toolBox->mNGridPointsVec[dim];

        // at position (i,j,k,...,0) in the global lattice file.
        std::vector<hsize_t> offsets;
        for (size_t i = 0; i < coords.size(); ++i)
          offsets.emplace_back(coords[i]);
        offsets.back() = 0;

        using vType = typename GetGetReturnType<R>::type;

        // We have the coordinate, now we need to convert this to an index in local memory. Let's buffer the coords in a
        // device array to use with putMemoryIndexFromSpatialLocationInto.
        device::IdxArray<NDim> memoryPos;
        for (size_t i = 0; i < coords.size(); ++i)
          memoryPos[i] = coords[i];
        // Then, overwrite memoryPos with the actual memory indices.
        device::apply([&](auto... idx) { mLayout.putMemoryIndexFromSpatialLocationInto(memoryPos, idx...); },
                      memoryPos);
        // To get the subview, we make another copy with one dimension less.
        device::IdxArray<NDim - 1> subMemoryPos;
        for (size_t i = 0; i < NDim - 1; ++i)
          subMemoryPos[i] = memoryPos[i];
        // And apply this to get the subview, with the last dimension as a range starting from memoryPos[dim] (which is
        // nGhosts) to memoryPos[dim]+nGrid[dim].
        auto subview = device::apply(
            [&](const auto &...args) {
              return device::memory::subview(
                  r.getView(), args..., std::pair<ptrdiff_t, ptrdiff_t>(memoryPos[dim], memoryPos[dim] + subdims[dim]));
            },
            subMemoryPos);

        // Now read from file to a temporary buffer on host
        std::vector<vType> rdata(toolBox->mNGridPointsVec[dim]);
        mDataset.readSlices(rdata, subdims, offsets);

        // And copy to device.
        device::memory::copyHostToDevice(rdata.data(), subview);
      } else {
        // Recursive call to loop over an arbitrary number of dimensions.
        for (int i = 0; i < sizes[dim]; ++i) {
          std::vector<ptrdiff_t> newCoords(coords);
          newCoords.emplace_back(starts[dim] + i);
          loadDim(r, dim + 1, newCoords);
        }
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    HDF5File mFile;
    HDF5Dataset mDataset;
  };
} // namespace TempLat

#endif

#endif
