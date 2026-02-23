#ifndef TEMPLAT_PARALLEL_MPI_MPITAGS_H
#define TEMPLAT_PARALLEL_MPI_MPITAGS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

namespace TempLat
{
  /* we are going to abuse __LINE__ as an looper like value, simply so we get unique values for all tags. */
  namespace MPITags
  {
    static constexpr int waitInLine = __LINE__;
    static constexpr int remapPrepareTransose = __LINE__;
    static constexpr int remapAllToAll = __LINE__;
    static constexpr int gatherAsVector = __LINE__;
    static constexpr int rangeExchangeSend = __LINE__;
    static constexpr int dataShiftGhostCells = __LINE__;
  }; // namespace MPITags

  /** @brief A class which just compiles.
   * Unit test: ctest -R test-mpitags
   **/
  class MPITagsJustCompile
  {
  public:
    // Put public methods here. These should change very little over time.
    MPITagsJustCompile() = default;

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };
} // namespace TempLat

#endif
