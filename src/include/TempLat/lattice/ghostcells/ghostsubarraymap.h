#ifndef TEMPLAT_LATTICE_MANIPULATION_GHOSTSUBARRAYMAP_H
#define TEMPLAT_LATTICE_MANIPULATION_GHOSTSUBARRAYMAP_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/ghostcells/ghostsubarray.h"

namespace TempLat
{
  /** @brief A class which keeps track of all subarray types for a given
   *  layout, which in turn is described by the LayoutStruct. The map is
   *  for datatypes and dimensions. getSubArray<T>(dimension) is the
   *  only necessary public method.
   *
   * Unit test: ctest -R test-ghostsubarraymap
   **/
  template <size_t NDim> class GhostSubarrayMap
  {
  public:
    // Put public methods here. These should change very little over time.
    GhostSubarrayMap(LayoutStruct<NDim> layout, device::Idx ghostDepth) : mLayout(layout), mGhostDepth(ghostDepth) {}

    template <typename T> GhostSubarray<NDim> getSubArray(ptrdiff_t dimension) { return pGetSubArray<T>(dimension); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    LayoutStruct<NDim> mLayout;
    device::Idx mGhostDepth;
    std::map<MPI_Datatype, std::vector<GhostSubarray<NDim>>> mSubArrays;

    template <typename T> GhostSubarray<NDim> pGetSubArray(ptrdiff_t dimension)
    {
      MPI_Datatype dataType = MPITypeSelect<T>();
      if (mSubArrays.count(dataType) < 1) {
        std::vector<GhostSubarray<NDim>> arraySet;
        for (size_t idimension = 0; idimension < NDim; ++idimension) {
          arraySet.emplace_back(mLayout, idimension, mGhostDepth, dataType);
        }
        mSubArrays[dataType] = arraySet;
      }
      return mSubArrays[dataType][dimension];
    }
  };

} // namespace TempLat

#endif
