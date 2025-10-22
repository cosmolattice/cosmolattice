#ifndef TEMPLAT_LATTICE_IO_HDF5_FILESAVERHDF5_H
#define TEMPLAT_LATTICE_IO_HDF5_FILESAVERHDF5_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#ifdef HDF5

#include "TempLat/util/prettytostring.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/parameters/parameterparser.h"

#include "TempLat/parallel/device.h"

#include "TempLat/lattice/IO/HDF5/helpers/hdf5file.h"

namespace TempLat
{

  /** \brief A class which implements saving in pure HDF5.
   *
   *
   * Unit test: make test-filesaverhdf5
   **/

  MakeException(StringIsTooLong);

  class FileSaverHDF5
  {
  public:
    // Put public methods here. These should change very little over time.
    FileSaverHDF5() { std::cout << "FileSaverHDF5: Called constructor."; }

    void open(std::string fn)
    {
      std::cout << "FileSaverHDF5: Opened file " << fn << std::endl;
      // mFile.open(fn);
    }

    void create(std::string fn)
    {
      std::cout << "FileSaverHDF5: Created file " << fn << std::endl;
      // mFile.create(fn);
    }

    void close()
    {
      std::cout << "FileSaverHDF5: Closed file." << std::endl;
      // mFile.close();
    }
    void reset()
    {
      std::cout << "FileSaverHDF5: Resetting file." << std::endl;
      this->close();
    }

    void save(ParameterParser &r)
    { // Conceptually, may be better as attributes? But nightmare to save vector of strings, did nt manage to do it in a
      // finite amount of time.
      /*
      std::ostringstream oss;

      std::vector<std::string> parStr;
      std::string tmp;
      for (auto x : r.getParams()) {
        if (x.second != "inf") {
          parStr.emplace_back(x.first + "=" + x.second);
        }
      }
      mDataset = mFile.createDataset<const char *>("Parameters", std::vector<hsize_t>(1, parStr.size()));
      for (size_t i = 0; i < parStr.size(); ++i) {
        if (parStr[i].size() > HDF5TypeConstant::FixedSizeStringLength)
          throw StringIsTooLong("Well, that's a bit embarassing. One of your parameters contains too many characters "
                                "(the total string should be smaller than " +
                                std::to_string(HDF5TypeConstant::FixedSizeStringLength) +
                                " char by default, for our hdf5). If you managed to make HDF5 with variable string "
                                "length, please let us know! If you just want to change the hardcoded number, look in "
                                "the file TempLat/lattice/IO/HDF5/helpers/hdf5type.h .");
        mDataset.writeElement(parStr[i].c_str(), std::vector<hsize_t>(1, i));
      }
      mDataset.close();
*/
    }

    template <typename R> void save(R r)
    { // used to store an entity directly to a dataset, using it's own name.
      /*
      typedef typename GetGetReturnType<R>::type vType;
      ConfirmSpace::apply(r, r.getToolBox()->mLayouts.getConfigSpaceLayout(), SpaceStateType::Configuration);
      GhostsHunter::apply(r);
      mDataset = mFile.createDataset<vType>(GetString::get(r), r.getToolBox()->mNGridPointsVec);
      saveDim(r, 0, {});
      mDataset.close();
*/
    }

    template <typename R, typename T> void save(T t, R r, std::string name)
    { // used to store an entity in a time series. The name is the one of the group, data set labelled by t.
      /*
      typedef typename GetGetReturnType<R>::type vType;
      ConfirmSpace::apply(r, r.getToolBox()->mLayouts.getConfigSpaceLayout(), SpaceStateType::Configuration);
      GhostsHunter::apply(r);
      mDataset = mFile.createOrOpenGroup(name).createDataset<vType>(PrettyToString::get(t, 10),
                                                                    r.getToolBox()->mNGridPointsVec);
      saveDim(r, 0, {});
      mDataset.close();
      */
    }

    template <typename R> void save(R t, std::string name)
    { // used to store a number. The name is the one of the dataset which contains this number.
      /*
      typedef typename GetGetReturnType<R>::type vType;
      mDataset = mFile.createDataset<vType>(name, std::vector<hsize_t>(1, 1));
      mDataset.writeElement(&t, std::vector<hsize_t>(1, 0));
      mDataset.close();
*/
    }

    // To save our fields, we use the fact that the last dimension is not parallelised.
    // We iterate over the first N-1 dimensions, and for each of these we save the whole
    // last dimension to file.
    template <typename R> void saveDim(R r, int dim, std::vector<ptrdiff_t> coords)
    {
      /*
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
        device::array<ptrdiff_t, NDim> memoryPos;
        for (size_t i = 0; i < coords.size(); ++i)
          memoryPos[i] = coords[i];
        // Then, overwrite memoryPos with the actual memory indices.
        device::apply([&](auto... idx) { mLayout.putMemoryIndexFromSpatialLocationInto(memoryPos, idx...); },
                      memoryPos);
        // To get the subview, we make another copy with one dimension less.
        device::array<ptrdiff_t, NDim - 1> subMemoryPos;
        for (size_t i = 0; i < NDim - 1; ++i)
          subMemoryPos[i] = memoryPos[i];

        // If the input is a field, we can copy directly from memory
        if constexpr (requires(R _r) { _r.getView(); }) {
          // And apply this to get the subview, with the last dimension as a range starting from memoryPos[dim] (which
          // is nGhosts) to memoryPos[dim]+nGrid[dim].
          auto subview = device::apply(
              [&](const auto &...args) {
                return Kokkos::subview(r.getView(), args...,
                                       std::pair<ptrdiff_t, ptrdiff_t>(memoryPos[dim], memoryPos[dim] + subdims[dim]));
              },
              subMemoryPos);

          // Finally, we can copy this subview to host and write it to the selected hyperslab in the dataset.
          std::vector<vType> sdata(toolBox->mNGridPointsVec[dim]);
          device::memory::copyDeviceToHost(subview, sdata.data());
          mDataset.writeSlices(sdata, subdims, offsets);
        } else {
          // Otherwise, we use GetEval to get the data point by point.
          Kokkos::View<vType *, Kokkos::DefaultExecutionSpace> device_buf("buffer", toolBox->mNGridPointsVec[dim]);
          auto functor = DEVICE_CLASS_LAMBDA(size_t i)
          {
            device::apply([&](const auto &...idx) { device_buf(i - memoryPos[dim]) = GetEval::getEval(r, idx..., i); },
                          subMemoryPos);
          };
          Kokkos::parallel_for("SaveDimBufferFilling",
                               Kokkos::RangePolicy(memoryPos[dim], memoryPos[dim] + subdims[dim]), functor);

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
*/
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    HDF5File mFile;
    // TODO: This is totally insane, but if this is uncommented, the data becomes just a set of nans.
    // No need to call anything, it's just straight nans. I have no clue why or how.
    // Problem for future me.
    // HDF5Dataset mDataset;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif // HDF5

#endif
