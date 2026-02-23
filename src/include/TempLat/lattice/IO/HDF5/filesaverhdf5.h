#ifndef TEMPLAT_LATTICE_IO_HDF5_FILESAVERHDF5_H
#define TEMPLAT_LATTICE_IO_HDF5_FILESAVERHDF5_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#ifdef HAVE_HDF5

#include <cstring>
#include <sstream>
#include <vector>
#include "TempLat/util/prettytostring.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/parameters/parameterparser.h"

#include "TempLat/parallel/device.h"
#include "TempLat/parallel/device_iteration.h"

#include "TempLat/lattice/IO/HDF5/helpers/hdf5file.h"

namespace TempLat
{
  MakeException(StringIsTooLong);

  /** @brief A class which implements saving in pure HDF5.
   *
   *
   * Unit test: ctest -R test-filesaverhdf5
   **/
  class FileSaverHDF5
  {
  public:
    /* Put public methods here. These should change very little over time. */
    FileSaverHDF5() = default;

    void open(std::string fn) { mFile.open(fn); }

    void create(std::string fn, FileMode flag = Overwrite) { mFile.create(fn, flag); }

    void close() { mFile.close(); }
    void reset() { this->close(); }

    void save(ParameterParser &r)
    { // Conceptually, may be better as attributes? But nightmare to save vector of strings, did nt manage to do it in a
      // finite amount of time.
      std::ostringstream oss;

      std::vector<std::string> parStr;
      std::string tmp;
      for (auto x : r.getParams()) {
        if (x.second != "inf") {
          parStr.emplace_back(x.first + "=" + x.second);
        }
      }

      // Create a flat buffer of fixed-size strings
      std::vector<char> stringData(parStr.size() * HDF5TypeConstant::FixedSizeStringLength, 0);
      for (size_t i = 0; i < parStr.size(); ++i) {
        if (parStr[i].size() > HDF5TypeConstant::FixedSizeStringLength)
          throw StringIsTooLong("Well, that's a bit embarassing. One of your parameters contains too many characters "
                                "(the total string should be smaller than " +
                                std::to_string(HDF5TypeConstant::FixedSizeStringLength) +
                                " char by default, for our hdf5). If you managed to make HDF5 with variable string "
                                "length, please let us know! If you just want to change the hardcoded number, look in "
                                "the file TempLat/lattice/IO/HDF5/helpers/hdf5type.h .");
        std::strncpy(&stringData[i * HDF5TypeConstant::FixedSizeStringLength], parStr[i].c_str(),
                     HDF5TypeConstant::FixedSizeStringLength - 1);
      }

      // Create dataset and write directly with H5Dwrite using proper string type
      mDataset = mFile.createDataset<const char *>("Parameters", std::vector<hsize_t>(1, parStr.size()));
      HDF5Type<const char *> strtype;
      H5Dwrite(mDataset, strtype.type, H5S_ALL, H5S_ALL, H5P_DEFAULT, stringData.data());
      strtype.close();
      mDataset.close();
    }

    void save_attr(ParameterParser &r)
    { // Conceptually, may be better as attributes? But nightmare to save vector of strings, did nt manage to do it in a
      // finite amount of time.
      std::ostringstream oss;

      std::vector<std::string> parStr;
      std::string tmp;
      mDataset = mFile.createDataset<const char *>("Parameters", std::vector<hsize_t>(1, parStr.size()));
      for (auto x : r.getParams()) {
        if (x.second != "inf") {
          // parStr.emplace_back(x.first + "=" + x.second);
          mDataset.addAtribute(x.first, x.second);
        }
      }
      /* for(size_t i = 0; i < parStr.size(); ++i){
           if(parStr[i].size() > HDF5TypeConstant::FixedSizeStringLength) throw StringIsTooLong("Well, that's a bit
       embarassing. One of your parameters contains too many characters (the total string should be smaller than
       "+std::to_string(HDF5TypeConstant::FixedSizeStringLength)+" char by default, for our hdf5). If you managed to
       make HDF5 with variable string length, please let us know! If you just want to change the hardcoded number, look
       in the file TempLat/lattice/IO/HDF5/helpers/hdf5type.h .");
           mDataset.writeElement(parStr[i].c_str(),std::vector<hsize_t>(1,i));
       }*/
      mDataset.close();
    }

    template <typename R> void save(R r)
    { // used to store an entity directly to a dataset, using it's own name.
      using vType = GetGetReturnType<R>::type;
      ConfirmSpace::apply(r, r.getToolBox()->mLayouts.getConfigSpaceLayout(), SpaceStateType::Configuration);
      GhostsHunter::apply(r);
      mDataset = mFile.createDataset<vType>(GetString::get(r), r.getToolBox()->mNGridPointsVec);
      saveDim(r, 0, {});
      mDataset.close();
    }

    template <typename R, typename T> void save(T t, R r, std::string name)
    { // used to store an entity in a time series. The name is the one of the group, data set labelled by t.
      using vType = GetGetReturnType<R>::type;
      ConfirmSpace::apply(r, r.getToolBox()->mLayouts.getConfigSpaceLayout(), SpaceStateType::Configuration);
      GhostsHunter::apply(r);
      mDataset = mFile.createOrOpenGroup(name).createDataset<vType>(PrettyToString::get(t, 10),
                                                                    r.getToolBox()->mNGridPointsVec);
      saveDim(r, 0, {});
      mDataset.close();
    }

    template <typename R> void save(R t, std::string name)
    { // used to store a number. The name is the one of the dataset which contains this number.
      using vType = GetGetReturnType<R>::type;
      mDataset = mFile.createDataset<vType>(name, std::vector<hsize_t>(1, 1));
      mDataset.writeElement(&t, std::vector<hsize_t>(1, 0));
      mDataset.close();
    }

    /**
     * @brief Save a double scalar to a named dataset
     * @param value The value to save
     * @param name Dataset name
     *
     * This overload explicitly handles double without GetGetReturnType
     */
    void save(double value, const std::string &name)
    {
      // Use "/" prefix for root group (matching openDataset pattern)
      std::string fullName = "/" + name;
      hsize_t dims[1] = {1};
      auto dataspace = H5Screate_simple(1, dims, nullptr);
      auto dataset = H5Dcreate2(mFile.getHandle(), fullName.c_str(), H5T_NATIVE_DOUBLE, dataspace, H5P_DEFAULT,
                                H5P_DEFAULT, H5P_DEFAULT);

      H5Dwrite(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &value);

      H5Dclose(dataset);
      H5Sclose(dataspace);
    }

    /**
     * @brief Save a string to a named dataset
     * @param str The string to save
     * @param name Dataset name
     *
     * Uses a large fixed-size buffer (8KB) to accommodate RNG states (~5KB)
     */
    void save(const std::string &str, const std::string &name)
    {
      constexpr size_t LargeStringLength = 16384; // 16KB for combined RNG states

      if (str.size() >= LargeStringLength) {
        throw StringIsTooLong("String too long for HDF5 dataset '" + name + "': " + std::to_string(str.size()) +
                              " chars (max " + std::to_string(LargeStringLength) + ")");
      }

      std::vector<char> buffer(LargeStringLength, 0);
      std::strncpy(buffer.data(), str.c_str(), LargeStringLength - 1);

      // Create custom string type with larger size
      auto memtype = H5Tcopy(H5T_C_S1);
      H5Tset_size(memtype, LargeStringLength);

      // Create dataspace and dataset (use "/" prefix for root group)
      std::string fullName = "/" + name;
      hsize_t dims[1] = {1};
      auto dataspace = H5Screate_simple(1, dims, nullptr);
      auto dataset =
          H5Dcreate2(mFile.getHandle(), fullName.c_str(), memtype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

      H5Dwrite(dataset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, buffer.data());

      H5Dclose(dataset);
      H5Sclose(dataspace);
      H5Tclose(memtype);
    }

    /**
     * @brief Save per-rank string data to a shared dataset (parallel HDF5 safe)
     * @param str The string to save
     * @param name Dataset name (shared across all ranks)
     * @param mpiRank This rank's index
     * @param nRanks Total number of MPI ranks
     *
     * Creates a dataset of size [nRanks] where each rank writes to its element.
     * This is parallel HDF5 safe - all ranks participate in collective operations.
     */
    void savePerRank(const std::string &str, const std::string &name, int mpiRank, int nRanks)
    {
      constexpr size_t LargeStringLength = 16384;

      if (str.size() >= LargeStringLength) {
        throw StringIsTooLong("String too long for HDF5 dataset '" + name + "': " + std::to_string(str.size()) +
                              " chars (max " + std::to_string(LargeStringLength) + ")");
      }

      std::vector<char> buffer(LargeStringLength, 0);
      std::strncpy(buffer.data(), str.c_str(), LargeStringLength - 1);

      // Create custom string type
      auto memtype = H5Tcopy(H5T_C_S1);
      H5Tset_size(memtype, LargeStringLength);

      // Create dataspace with nRanks elements (collective operation)
      std::string fullName = "/" + name;
      hsize_t dims[1] = {static_cast<hsize_t>(nRanks)};
      auto filespace = H5Screate_simple(1, dims, nullptr);
      auto dataset =
          H5Dcreate2(mFile.getHandle(), fullName.c_str(), memtype, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

      // Select hyperslab for this rank's element
      hsize_t start[1] = {static_cast<hsize_t>(mpiRank)};
      hsize_t count[1] = {1};
      H5Sselect_hyperslab(filespace, H5S_SELECT_SET, start, nullptr, count, nullptr);

      // Memory space for single element
      hsize_t memDims[1] = {1};
      auto memspace = H5Screate_simple(1, memDims, nullptr);

      // Write with independent I/O (each rank writes its own element)
      auto plist = H5Pcreate(H5P_DATASET_XFER);
#ifdef HAVE_MPI
      H5Pset_dxpl_mpio(plist, H5FD_MPIO_INDEPENDENT);
#endif
      H5Dwrite(dataset, memtype, memspace, filespace, plist, buffer.data());

      H5Pclose(plist);
      H5Sclose(memspace);
      H5Dclose(dataset);
      H5Sclose(filespace);
      H5Tclose(memtype);
    }

    /**
     * @brief Save per-rank double data to a shared dataset (parallel HDF5 safe)
     * @param value The value to save
     * @param name Dataset name (shared across all ranks)
     * @param mpiRank This rank's index
     * @param nRanks Total number of MPI ranks
     */
    void savePerRank(double value, const std::string &name, int mpiRank, int nRanks)
    {
      std::string fullName = "/" + name;
      hsize_t dims[1] = {static_cast<hsize_t>(nRanks)};
      auto filespace = H5Screate_simple(1, dims, nullptr);
      auto dataset = H5Dcreate2(mFile.getHandle(), fullName.c_str(), H5T_NATIVE_DOUBLE, filespace, H5P_DEFAULT,
                                H5P_DEFAULT, H5P_DEFAULT);

      // Select hyperslab for this rank's element
      hsize_t start[1] = {static_cast<hsize_t>(mpiRank)};
      hsize_t count[1] = {1};
      H5Sselect_hyperslab(filespace, H5S_SELECT_SET, start, nullptr, count, nullptr);

      // Memory space for single element
      hsize_t memDims[1] = {1};
      auto memspace = H5Screate_simple(1, memDims, nullptr);

      // Write with independent I/O
      auto plist = H5Pcreate(H5P_DATASET_XFER);
#ifdef HAVE_MPI
      H5Pset_dxpl_mpio(plist, H5FD_MPIO_INDEPENDENT);
#endif
      H5Dwrite(dataset, H5T_NATIVE_DOUBLE, memspace, filespace, plist, &value);

      H5Pclose(plist);
      H5Sclose(memspace);
      H5Dclose(dataset);
      H5Sclose(filespace);
    }

    /**
     * @brief Save RNG state as binary uint64_t array (compact storage)
     * @param textState String from RandomUniform::saveState() or RandomGaussian::saveState()
     * @param name Dataset name
     * @param mpiRank This rank's index
     * @param nRanks Total number of MPI ranks
     */
    void saveRNGStateBinary(const std::string &textState, const std::string &name, int mpiRank, int nRanks)
    {
      std::vector<uint64_t> binaryState;
      std::istringstream iss(textState);

      uint64_t val;
      std::string line;
      std::getline(iss, line);
      std::istringstream lineStream(line);
      while (lineStream >> val) {
        binaryState.push_back(val);
      }

      if (std::getline(iss, line) && !line.empty()) {
        std::istringstream gaussianStream(line);
        uint64_t gaussianCounter, hasCache;
        gaussianStream >> gaussianCounter >> hasCache;
        binaryState.push_back(gaussianCounter);
        binaryState.push_back(hasCache);

        if (hasCache) {
          double cachedValue;
          gaussianStream >> cachedValue;
          uint64_t cachedBits;
          std::memcpy(&cachedBits, &cachedValue, sizeof(double));
          binaryState.push_back(cachedBits);
        } else {
          binaryState.push_back(0);
        }
      }

      std::string fullName = "/" + name;
      hsize_t dims[2] = {static_cast<hsize_t>(nRanks), static_cast<hsize_t>(binaryState.size())};
      auto filespace = H5Screate_simple(2, dims, nullptr);
      auto dataset =
          H5Dcreate2(mFile.getHandle(), fullName.c_str(), H5T_NATIVE_UINT64, filespace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

      hsize_t start[2] = {static_cast<hsize_t>(mpiRank), 0};
      hsize_t count[2] = {1, static_cast<hsize_t>(binaryState.size())};
      H5Sselect_hyperslab(filespace, H5S_SELECT_SET, start, nullptr, count, nullptr);

      hsize_t memDims[1] = {static_cast<hsize_t>(binaryState.size())};
      auto memspace = H5Screate_simple(1, memDims, nullptr);

      auto plist = H5Pcreate(H5P_DATASET_XFER);
#ifdef HAVE_MPI
      H5Pset_dxpl_mpio(plist, H5FD_MPIO_INDEPENDENT);
#endif
      H5Dwrite(dataset, H5T_NATIVE_UINT64, memspace, filespace, plist, binaryState.data());

      H5Pclose(plist);
      H5Sclose(memspace);
      H5Dclose(dataset);
      H5Sclose(filespace);
    }

    HDF5Group createOrOpenGroup(const std::string &name) { return mFile.createOrOpenGroup(name); }

    // To save our fields, we use the fact that the last dimension is not parallelised.
    // We iterate over the first N-1 dimensions, and for each of these we save the whole
    // last dimension to file.
    template <typename R> void saveDim(R r, int dim, std::vector<ptrdiff_t> coords)
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
        std::vector<hsize_t> subdims(NDim, 1);
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

        // If the input is a field, we can copy directly from memory
        if constexpr (requires(R _r) { _r.getView(); }) {
          // And apply this to get the subview, with the last dimension as a range starting from memoryPos[dim] (which
          // is nGhosts) to memoryPos[dim]+nGrid[dim].
          auto subview = device::apply(
              [&](const auto &...args) {
                return device::memory::subview(
                    r.getView(), args...,
                    std::pair<ptrdiff_t, ptrdiff_t>(memoryPos[dim], memoryPos[dim] + subdims[dim]));
              },
              subMemoryPos);

          // Finally, we can copy this subview to host and write it to the selected hyperslab in the dataset.
          std::vector<vType> sdata(toolBox->mNGridPointsVec[dim]);
          device::memory::copyDeviceToHost(subview, sdata.data());
          mDataset.writeSlices(sdata, subdims, offsets);
        } else {
          // Otherwise, we get the data point by point.
          device::memory::NDView<1, vType> device_buf("buffer", toolBox->mNGridPointsVec[dim]);
          auto functor = DEVICE_CLASS_LAMBDA(device::IdxArray<1> jdx)
          {
            device::Idx i = jdx[0];
            device::apply([&](const auto &...idx) { device_buf(i - memoryPos[dim]) = DoEval::eval(r, idx..., i); },
                          subMemoryPos);
          };
          device::iteration::foreach<1>("SaveDimBufferFilling", {memoryPos[dim]}, {(device::Idx)subdims[dim]}, functor);

          // Finally, we can copy this subview to host and write it to the selected hyperslab in the dataset.
          std::vector<vType> sdata(toolBox->mNGridPointsVec[dim]);
          device::memory::copyDeviceToHost(device_buf, sdata.data());
          mDataset.writeSlices(sdata, subdims, offsets);
        }
      } else {
        // Recursive call to loop over an arbitrary number of dimensions.
        for (int i = 0; i < sizes[dim]; ++i) {
          std::vector<ptrdiff_t> newCoords(coords);
          newCoords.emplace_back(starts[dim] + i);
          saveDim(r, dim + 1, newCoords);
        }
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    HDF5File mFile;
    HDF5Dataset mDataset;
  };
} // namespace TempLat

#endif // HAVE_HDF5

#endif
