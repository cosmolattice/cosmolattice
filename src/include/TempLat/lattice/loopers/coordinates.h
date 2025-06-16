#ifndef TEMPLAT_LATTICE_ITERATORS_COORDINATES_H
#define TEMPLAT_LATTICE_ITERATORS_COORDINATES_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019. Jorge Baeza-Ballesteros, Year: 2023

#include "TempLat/lattice/loopers/coordinatesinitializer.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  /** \brief A class which
   * holds offsets and vector coordinates.
   * Used to cache the coordinate access and have loopers in arbitrary dimensions.
   *
   * Unit test: make test-coordinates
   **/
  template <size_t NDim> class Coordinates
  {
  public:
    /* Put public methods here. These should change very little over time. */
    Coordinates(const JumpsHolder<NDim> &jumpsHolder, const LayoutStruct<NDim> &layoutStruct)
        : mLayouts(layoutStruct), mJumps(jumpsHolder), mIStart(0), jumps(mJumps.getJumpsInMemoryOrder()), vect{}
    {
      std::cout << mOffsets << std::endl;
      CoordinatesInitializer mCI(jumpsHolder, layoutStruct);
      mCI.initCoords(1, mOffsets, mIStart, mIEnd);
      // auto sizes = mJumps.getSizesInMemory();
      mOffsets.shrink_to_fit();
      std::cout << mOffsets << std::endl;
    }

    ptrdiff_t operator()(ptrdiff_t i) const
    {
      std::cout << mOffsets << std::endl;
#ifdef CHECKBOUNDS
      if (i < 0 || i >= (ptrdiff_t)mOffsets.size()) {
        throw std::out_of_range("Coordinates::operator() out of bounds: " + std::to_string(i) + " not in [0, " +
                                std::to_string(mOffsets.size()) + ")");
      }
#endif
      std::cout << "Coordinates::operator() called with i = " << i << ", size is " << mOffsets.size() << std::endl;
      return mOffsets[i];
    }

    ptrdiff_t &operator()(ptrdiff_t i)
    {
      std::cout << "mIStart: " << mIStart << ", mIEnd: " << mIEnd << std::endl;
      std::cout << "jumps: ";
      for (const auto &jump : jumps) {
        std::cout << jump << " ";
      }
      std::cout << std::endl;
      std::cout << "vect: ";
      for (const auto &v : vect) {
        std::cout << v << " ";
      }
      std::cout << std::endl;

      std::cout << mOffsets << std::endl;
#ifdef CHECKBOUNDS
      if (i < 0 || i >= (ptrdiff_t)mOffsets.size()) {
        throw std::out_of_range("Coordinates::operator() out of bounds: " + std::to_string(i) + " not in [0, " +
                                std::to_string(mOffsets.size()) + ")");
      }
#endif
      std::cout << "Coordinates::operator() called with i = " << i << ", size is " << mOffsets.size() << std::endl;
      return mOffsets[i];
    }

    std::vector<ptrdiff_t> getVec(ptrdiff_t i) { return getVecOffset(mOffsets[i]); }

    std::array<ptrdiff_t, NDim> getVecOffset(ptrdiff_t i) //(JBB 2023)
    {
      std::cout << "mIStart: " << mIStart << ", mIEnd: " << mIEnd << std::endl;
      std::cout << "jumps: ";
      for (const auto &jump : jumps) {
        std::cout << jump << " ";
      }
      std::cout << std::endl;
      std::cout << "vect: ";
      for (const auto &v : vect) {
        std::cout << v << " ";
      }
      std::cout << std::endl;

      auto offset = i - mJumps.toOrigin();
      for (size_t j = 0; j < NDim; j++) {
        mLayouts.putSpatialLocationFromMemoryIndexInto(offset / jumps[j], j, vect);
        std::cout << "layout[" << j << "]: ";
        for (size_t k = 0; k < NDim; k++) {
          std::cout << vect[k] << " ";
        }
        std::cout << std::endl;
        offset = offset % jumps[j];
      }
      std::cout << "getVecOffset: " << i << " -> ";
      for (size_t j = 0; j < NDim; j++) {
        std::cout << vect[j] << " ";
      }
      std::cout << std::endl;
      return vect;
    }

    ptrdiff_t getStart() const { return mIStart; }
    ptrdiff_t getEnd() const { return mIEnd; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    LayoutStruct<NDim> mLayouts;
    JumpsHolder<NDim> mJumps;

    std::vector<ptrdiff_t> mOffsets;
    ptrdiff_t mIStart;
    ptrdiff_t mIEnd;
    std::array<ptrdiff_t, NDim> jumps, vect;

    template <size_t _NDim> friend class Looper;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/loopers/coordinates_test.h"
#endif

#endif
