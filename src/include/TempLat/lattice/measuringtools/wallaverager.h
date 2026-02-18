#ifndef TEMPLAT_LATTICE_MEASUREMENTS_WALLAVERAGER_H
#define TEMPLAT_LATTICE_MEASUREMENTS_WALLAVERAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2025

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"

namespace TempLat
{

  /** @brief A class which computes the average value of a getter over the first n-1 coordinates, returns a vector.
   *
   *
   * Unit test: ctest -R test-averager
   **/
  template <typename T> class WallAverager
  {
  public:
    using vType = typename GetGetReturnType<T>::type;

    // Put public methods here. These should change very little over time.
    WallAverager(const T &pT, SpaceStateType spaceType) : mT(pT), mSpaceType(spaceType)
    {
      for (ptrdiff_t i = 0; i < mT.getToolBox()->mNDimensions; ++i) {
        mWorkspace.emplace_back(std::vector<vType>(mT.getToolBox()->mNGridPointsVec[i], 0));
      }
    }

    operator vType() { return compute(); }

    void compute()
    {
      if (mSpaceType == SpaceStateType::Fourier)
        throw AveragerWrongSpace("Wall averager works only in configuration space.");

      computeConfigurationSpace();

      // mT.getToolBox()->mGroup.getBaseComm().Allreduce(mWorkspace.data(), MPI_SUM); // Perform the reduction. All
      // processes have a N*d array (in the isotropic case). it is allreduced here.
      for (ptrdiff_t i = 0; i < mT.getToolBox()->mNDimensions; ++i) {
        mT.getToolBox()->mGroup.getBaseComm().Allreduce(
            &mWorkspace[i], MPI_SUM); // Perform the reduction. All processes have a N*d array (in the isotropic case).
                                      // it is allreduced here.
      }

      std::vector<ptrdiff_t> codim(
          mT.getToolBox()->mNDimensions,
          1); // For each dimension, compute the size of the space that is average over (N^(d-1) for isotropic lattices)
      for (ptrdiff_t t = 0; t < mT.getToolBox()->mNDimensions; ++t) {
        for (ptrdiff_t j = 0; j < mT.getToolBox()->mNDimensions; ++j) {
          if (j != t) codim[t] *= mT.getToolBox()->mNGridPointsVec[j];
        }
      }

      for (ptrdiff_t t = 0; t < mT.getToolBox()->mNDimensions; ++t) { // Normalize the average
        for (ptrdiff_t j = 0; j < mT.getToolBox()->mNGridPointsVec[t]; ++j) {
          mWorkspace[t][j] /= codim[t];
        }
      }
    }

    void computeConfigurationSpace()
    {
      // TODO : THIS IS STILL CL1.0
      ptrdiff_t i = 0;
      auto &it = mT.getToolBox()->itX();

      // std::vector<size_t> coord (mT.getToolBox()->mNDimensions, 0);

      AveragerHelper<vType, false>::onBeforeAverageConfiguration(mT, mSpaceType);

      for (it.begin(); it.end(); ++it) {
        i = it();
        auto coord = mT.getToolBox()->getCoordConfiguration0N(i);
        DoEval::eval(mT, i);

        for (ptrdiff_t t = 0; t < mT.getToolBox()->mNDimensions; ++t) {
          mWorkspace[t][coord[t]] += DoEval::eval(mT, i); // Average over the orthogonal directions.
        }
      }
    }

    std::string toString() const { return "<" + GetString::get(mT) + ">_walls"; }

    /** For measurement objects. */
    inline auto getToolBox() const { return GetToolBox::get(mT); }

    auto getWall(size_t dim) const { return mWorkspace[dim]; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    T mT;
    SpaceStateType mSpaceType;
    std::vector<std::vector<vType>> mWorkspace;
  };

  template <typename T> auto wallAverager(T instance, SpaceStateType spaceType = SpaceStateType::Configuration)
  {
    return WallAverager<T>(instance, spaceType);
  }

  auto wallAverager(ZeroType a) { return 0; }

  struct WallAveragerTester {
#ifdef TEMPLATTEST
  public:
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
