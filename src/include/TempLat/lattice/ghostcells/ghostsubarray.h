#ifndef TEMPLAT_LATTICE_MANIPULATION_GHOSTSUBARRAY_H
#define TEMPLAT_LATTICE_MANIPULATION_GHOSTSUBARRAY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/parallel/mpi/mpitypeconstants.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"

namespace TempLat
{
  /** @brief A class which holds an MPI_Datatype which is a subarray of
   * the lattice, created by MPI_Type_create_subarray. The subarray
   * is the ghost padding of the lattice, in one single dimension.
   *
   * Unit test: ctest -R test-ghostsubarray
   **/
  template <size_t NDim> class GhostSubarray
  {
  public:
    // Put public methods here. These should change very little over time.

    /** @brief Constructor
     *  @param layout The layout of the memory.
     *  @param dimension The dimension in which to create the array.
     *  @param depth The number of ghost steps at either side of the array.
     *  @param atomType the MPI_Datatype corresponding to the elements which are laid out in the layout.
     */
    GhostSubarray(LayoutStruct<NDim> layout, device::Idx dimension, device::Idx depth, MPI_Datatype atomType)
        : mLayout(layout),
#ifdef HAVE_MPI
          mDimension(dimension), mDepth(depth), mAtomType(atomType),
#endif
          mSubarrayMPIDataType(std::make_shared<MPI_Datatype>())
    {
#ifdef HAVE_MPI

      /* size of our bare owned memory */
      std::vector<int> ownedSizes(mLayout.getSizesInMemory().begin(), mLayout.getSizesInMemory().end());

      /* size of our bare owned memory + ghost depth */
      for (auto &&it : ownedSizes)
        it += 2 * mDepth;

      /* size of our the slice of our total memory with thickness depth */
      std::vector<int> subarraySizes(ownedSizes.begin(), ownedSizes.end());
      /* change full volume to a slice */
      subarraySizes[mDimension] = mDepth;

      std::vector<int> starts(NDim, 0);
      MPI_Type_create_subarray(NDim, ownedSizes.data(), subarraySizes.data(), starts.data(), MPI_ORDER_C, mAtomType,
                               mSubarrayMPIDataType.get());

      MPI_Type_commit(mSubarrayMPIDataType.get());
#endif
    }

    ~GhostSubarray()
    {
#ifdef HAVE_MPI
      if (mSubarrayMPIDataType.use_count() < 2) {
        MPI_Type_free(mSubarrayMPIDataType.get());
      }
#endif
    }

    operator MPI_Datatype() const { return *mSubarrayMPIDataType; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    LayoutStruct<NDim> mLayout;
#ifdef HAVE_MPI
    device::Idx mDimension;
    device::Idx mDepth;
    MPI_Datatype mAtomType;
#endif
    std::shared_ptr<MPI_Datatype> mSubarrayMPIDataType;
  };

} // namespace TempLat

#endif
