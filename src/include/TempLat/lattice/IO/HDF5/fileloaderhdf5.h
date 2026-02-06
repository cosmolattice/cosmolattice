#ifndef TEMPLAT_LATTICE_IO_HDF5_FILELOADERHDF5_H
#define TEMPLAT_LATTICE_IO_HDF5_FILELOADERHDF5_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#ifdef HDF5

#include <cstring>
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5file.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/parameters/parameterparser.h"

namespace TempLat
{

  /** \brief A class which implements loading in pure HDF5.
   *
   *
   * Unit test: make test-fileloaderhdf5
   **/

  class FileLoaderHDF5
  {
  public:
    /* Put public methods here. These should change very little over time. */
    FileLoaderHDF5() {}

    void open(std::string fn) { mFile.open(fn); }
    void close() { mFile.close(); }
    void reset() { this->close(); }

    void load(ParameterParser &par)
    {

      mDataset = mFile.openDataset("Parameters");

      std::vector<std::string> parStr;
      char tmp[HDF5TypeConstant::FixedSizeStringLength];

      hid_t dspace = H5Dget_space(mDataset); // get number of parameters stored.
      const int ndims = H5Sget_simple_extent_ndims(dspace);
      hsize_t dims[ndims];
      H5Sget_simple_extent_dims(dspace, dims, NULL);
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
#ifndef NOMPI
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
#ifndef NOMPI
      H5Pset_dxpl_mpio(plist, H5FD_MPIO_INDEPENDENT);
#endif
      H5Dread(dataset, H5T_NATIVE_DOUBLE, memspace, filespace, plist, &value);

      H5Pclose(plist);
      H5Sclose(memspace);
      H5Sclose(filespace);
      H5Dclose(dataset);
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

      auto starts = toolBox->mLayouts.getConfigSpaceStarts(); // Local mpi offset.
      auto sizes = toolBox->mLayouts.getConfigSpaceSizes();   // Local mpi sizes.

      if (dim == toolBox->mNDimensions - 1) // Last dimension, saved as a full rod.
      {
        coords.emplace_back(
            0); // look at index 0 in the last dimension. The next nGrid[last dimension] points are stored continuously.
        ptrdiff_t offset = r.getJumps().getTotalOffsetFromSpatialCoordinates(coords);

        std::vector<hsize_t> subdims(
            toolBox->mNDimensions,
            1); // for hdf5, tell it we want to store a sub array of size (1,1,1...,nGrid[last dimension]).
        subdims.back() = toolBox->mNGridPointsVec[dim];
        std::vector<hsize_t> offsets; // at position (i,j,k,...,0) in the global lattice file.
        for (size_t i = 0; i < coords.size(); ++i)
          offsets.emplace_back(coords[i]);
        offsets.back() = 0;

        typedef typename GetGetReturnType<R>::type vType;

        std::vector<vType> rdata(toolBox->mNGridPointsVec[dim]);
        mDataset.readSlices(rdata, subdims, offsets);

        for (size_t i = 0; i < rdata.size(); ++i) {
          r.getSet(offset + i) = rdata[i];
        }

      } else {
        for (int i = 0; i < sizes[dim]; ++i) { // Recursive call to loop over an arbitrary number of dimensions.
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

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/IO/HDF5/fileloaderhdf5_test.h"
#endif

#endif

#endif
