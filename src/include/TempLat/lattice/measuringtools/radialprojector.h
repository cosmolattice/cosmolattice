#ifndef TEMPLAT_LATTICE_MEASUREMENTS_RADIALPROJECTOR_H
#define TEMPLAT_LATTICE_MEASUREMENTS_RADIALPROJECTOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <algorithm>
#include <cstddef>

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionresult.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsinglequantity.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialbincomputer.h"
#include "TempLat/lattice/algebra/operators/squareroot.h"

#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"

namespace TempLat
{
  /** @brief A class which projects any N-D lattice on its positive-definite radial coordinate. In other
   *  words, integrating out all the angular dimensions.
   *  When in Fourier-space, this routine takes into account what the redundancies are.
   *  Only the unique values are counted. Real-valued entries get weight 0.5
   *  (they only contribute one float value out of the two for each complex value),
   *  and of the hermitian pairs, only the positivePartner is taken into account.
   *
   *  See `RadialProjectionResult` for a description of the result type.
   *
   * Unit test: ctest -R test-radialprojector
   **/

  template <typename T> class RadialProjector
  {
  public:
    using vType = typename GetGetReturnType<T>::type;
    using sType = typename GetFloatType<vType>::type;

    static constexpr bool isComplexValued = GetGetReturnType<T>::isComplex;
    using floatType = typename RadialProjectionResult<sType>::floatType;
    using resultType = RadialProjectionResult<sType>;

    static constexpr size_t NDim = GetNDim::get<T>();

    RadialProjector(const T &instance, SpaceStateType spaceType, device::memory::host_ptr<MemoryToolBox<NDim>> pToolBox,
                    bool pUseCentralBinValues)
        : mSpaceType(spaceType), mInstance(instance), mToolBox(pToolBox), mUseBinCentralValues(pUseCentralBinValues),
          mLayout((mSpaceType == SpaceStateType::Fourier) ? mToolBox->mLayouts.getFourierSpaceLayout()
                                                          : mToolBox->mLayouts.getConfigSpaceLayout())
    {
    }

    /** @brief Measure the projection of your n-dimensional lattice onto the
     *  radial direction. Only supports linear binning, so if you want to
     *  transform to e.g. logarithmic binning, you will simply have a pass
     *  a large number of linear bins here, and do the logarithmic binning yourself.
     *  Default is nGrid ^ (nDimensions - 1).
     */
    RadialProjectionResult<sType> measure(ptrdiff_t nLinearBins = -1, sType customRange = -1, bool excludeOrigin = true)
    {
      if (nLinearBins < 0) {
        ptrdiff_t nGrid = mLayout.getGlobalSizes()[0];
        nLinearBins = std::pow(nGrid, std::max((size_t)1, NDim - 1));
      }

      RadialProjectionResult<sType> baseWorkSpace(nLinearBins, mUseBinCentralValues,
                                                  mSpaceType == SpaceStateType::Fourier);

      sType minValue = excludeOrigin ? 1.0 : 0.0;

      RadialProjectionResult<sType> myResult =
          mSpaceType == SpaceStateType::Configuration
              ? computeConfigurationSpace(makeBinComputer(nLinearBins, minValue, customRange), baseWorkSpace,
                                          excludeOrigin)
              : computeFourierSpace(makeBinComputer(nLinearBins, minValue, customRange), baseWorkSpace, excludeOrigin);

      myResult.finalize(mToolBox->mGroup.getBaseComm());

      return myResult;
    }

  private:
    const SpaceStateType mSpaceType;
    T mInstance;
    device::memory::host_ptr<MemoryToolBox<NDim>> mToolBox;
    bool mUseBinCentralValues;
    LayoutStruct<NDim> mLayout;

  public:
    template <typename BINCOMPUTETYPE>
    RadialProjectionResult<sType> computeConfigurationSpace(BINCOMPUTETYPE binComputer,
                                                            RadialProjectionResult<sType> baseWorkSpace,
                                                            bool excludeOrigin)
    {
      confirmGetterSpace();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx)
      {
        // Get the global coordinates of this index.
        device::IdxArray<NDim> global_coords;
        device::apply([&](auto &&...args) { mLayout.putSpatialLocationFromMemoryIndexInto(global_coords, args...); },
                      idx);

        // Check if we are at the origin.
        bool isAtOrigin = true;
        for (auto &&it : global_coords)
          if (it != 0) {
            isAtOrigin = false;
            break;
          }
        if (excludeOrigin && isAtOrigin) [[unlikely]]
          return;

        // get the radius
        sType r{};
        for (size_t i = 0; i < NDim; ++i)
          r += global_coords[i] * global_coords[i];
        r = device::sqrt(r);

        // Map the radius to a bin
        const ptrdiff_t bin = binComputer(r);

        // Add the bin contribution to the workspace.
        device::apply([&](auto &&...args) { baseWorkSpace.add_device(bin, DoEval::eval(mInstance, args...), r, 1.); },
                      idx);
      };

      device::iteration::foreach ("RadialProjectorConfiguration", mLayout, functor);

      baseWorkSpace.pull();
      binComputer.setCentralBinBounds(baseWorkSpace.getCentralBinBounds());
      return baseWorkSpace;
    }

  public:
    template <typename BINCOMPUTETYPE>
    RadialProjectionResult<sType> computeFourierSpace(BINCOMPUTETYPE binComputer,
                                                      RadialProjectionResult<sType> baseWorkSpace, bool excludeOrigin)
    {
      confirmGetterSpace();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx)
      {
        // Get the global coordinates of this index.
        device::IdxArray<NDim> global_coords;
        device::apply([&](auto &&...args) { mLayout.putSpatialLocationFromMemoryIndexInto(global_coords, args...); },
                      idx);

        // Check if we are at the origin.
        bool isAtOrigin = true;
        for (auto &&it : global_coords)
          if (it != 0) {
            isAtOrigin = false;
            break;
          }
        if (excludeOrigin && isAtOrigin) [[unlikely]]
          return;

        const HermitianRedundancy quality = mLayout.getHermitianPartners().qualify(global_coords);
        if (quality != HermitianRedundancy::negativePartner) {
          // get the radius
          sType r{};
          for (size_t i = 0; i < NDim; ++i)
            r += global_coords[i] * global_coords[i];
          r = device::sqrt(r);

          // Map the radius to a bin
          const ptrdiff_t bin = binComputer(r);

          // don't over-weight the real-valued entries: only one float value, only half the weight.
          floatType weight = quality == HermitianRedundancy::realValued ? 0.5 : 1;

          // Add the bin contribution to the workspace.
          device::apply(
              [&](auto &&...args) { baseWorkSpace.add_device(bin, DoEval::eval(mInstance, args...), r, weight); }, idx);
        }
      };
      device::iteration::foreach ("RadialProjectorFourier", mLayout, functor);
      baseWorkSpace.pull();
      binComputer.setCentralBinBounds(baseWorkSpace.getCentralBinBounds());

      return baseWorkSpace;
    }

  private:
    void confirmGetterSpace()
    {
      ConfirmSpace::apply(mInstance, mLayout, mSpaceType);
      GhostsHunter::apply(mInstance);
    }

    /** @brief Creates the lambda that maps the IterationCoordinates to a bin. */
    inline auto makeBinComputer(ptrdiff_t nLinearBins, sType minValue, sType customRange = -1)
    {
      auto rMax = customRange < 0 ? mLayout.getMaxRadius() : customRange;

      return RadialBinComputer(minValue, rMax, nLinearBins);
    }
  };

  template <size_t NDim, typename T>
  RadialProjector<T> projectRadially(T instance, SpaceStateType spaceType,
                                     device::memory::host_ptr<MemoryToolBox<NDim>> pToolBox, bool useBinCentralValues)
  {
    return RadialProjector<T>(instance, spaceType, pToolBox, useBinCentralValues);
  }

  template <typename T> RadialProjector<T> projectRadially(T instance, bool useBinCentralValues = false)
  {
    return projectRadially(instance, SpaceStateType::Configuration, GetToolBox::get(instance), useBinCentralValues);
  }

  template <typename T> RadialProjector<T> projectRadiallyFourier(T instance, bool useBinCentralValues = false)
  {
    return projectRadially(instance, SpaceStateType::Fourier, GetToolBox::get(instance), useBinCentralValues);
  }

} // namespace TempLat

#endif
