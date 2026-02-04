#ifndef TEMPLAT_LATTICE_IO_HDF5_FILELOADERHDF5_H
#define TEMPLAT_LATTICE_IO_HDF5_FILELOADERHDF5_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#ifdef HDF5

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5file.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/parameters/parameterparser.h"

#include "TempLat/parallel/device.h"
#include "TempLat/parallel/device_memory.h"

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
    // Put public methods here. These should change very little over time.
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

      if (dim == toolBox->mNDimensions - 1) // Last dimension, saved as a full rod.
      {
        // look at index 0 in the last dimension. The next nGrid[last dimension] points are stored continuously.
        coords.emplace_back(0);

        // for hdf5, tell it we want to store a sub array of size (1,1,1...,nGrid[last dimension]).
        std::vector<hsize_t> subdims(toolBox->mNDimensions, 1);
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

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif

#endif
