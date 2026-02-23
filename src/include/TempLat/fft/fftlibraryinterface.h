#ifndef TEMPLAT_FFT_FFTLIBRARYINTERFACE_H
#define TEMPLAT_FFT_FFTLIBRARYINTERFACE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/memory/memorylayouts/fftlayoutstruct.h"

#include "TempLat/lattice/memory/memoryblock.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"
namespace TempLat
{

  /** @brief Yes, another nested class interface: for the session guard. Your constructor should take care of
   * your_library_init(), your destructor should take care of your_library_cleanup(). These are the session-wide
   * initialization / finalization calls.
   */
  class FFTSessionGuard
  {
  public:
    virtual ~FFTSessionGuard() = default;
  };

  /** @brief Yes, for once a nested class. The interface for your to-be-implemented FFT plan, but forward and
   * backward. Complex-to-real and real-to-complex.
   *
   *  Your implementation of FFTPlanInterface must take care of freeing the plan(s) upon destruction. So you should use
   * shared_ptr's!
   */
  template <size_t NDim, typename T> class FFTPlanInterface
  {
  public:
    /* virtual desctructor can not be abstract: https://stackoverflow.com/a/13444839/2295722 */
    virtual ~FFTPlanInterface() = default;
    virtual void c2r(MemoryBlock<NDim, T> &mBlock) = 0;
    virtual void r2c(MemoryBlock<NDim, T> &mBlock) = 0;
  };

  /** @brief A pure abstract class (interface!) which defines the methods that you must implement for your new fft
   *library to play well with us.
   *
   * Unit test: ctest -R test-fftlibraryinterface
   **/
  template <size_t NDim> class FFTLibraryInterface
  {
  public:
    // Put public methods here. These should change very little over time.
    FFTLibraryInterface() = default;
    virtual ~FFTLibraryInterface() = default;

    /** @brief Return the number of dimensions over which the MPI processes can be distributed, given a problem of
     * dimension nDimensions. FFTW can only divide in 1 dimension: the leading dimension. ParaFaFT can divide an N
     * dimensional setup over the leading N - 1 dimensions.
     *
     * The calling function needs to know, because the user may switch between layouts after compilation.
     */
    virtual ptrdiff_t getMaximumNumberOfDimensionsToDivide(ptrdiff_t nDimensions) = 0;

    /** @brief The lattice objects expect an *unnormalized* FFT, such as the one FFTW gives:
     *  applying once forward and then backward should return the input values multiplied by
     *  nGridPoints^nDimensions, i.e. the total number of values in the problem.
     *  If your library does not respect that, return the factor by which we must multiply the output
     *  values to obtain the unnormalized FFT. Return default IntrinsicScales (exact (double) 1)
     *  in order not to waste time, if your library does it like FFTW.
     */
    virtual IntrinsicScales getIntrinsicRescaleToGetUnnormalizedFFT(ptrdiff_t nGridPoints) = 0;

    /** @brief given an actual setup, return the description of the subarray of the global problem that this process
     * holds. */
    virtual FFTLayoutStruct<NDim> computeLocalSizes(MPICartesianGroup group, device::IdxArray<NDim> nGridPoints,
                                                    bool forbidTransposition = false) = 0;

    /** @brief If your library has different levels of patience for the planning phase, set it here.
     */
    virtual void setPlannerPatience(int level) = 0;

    /** @brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
     * shared_ptr's. Since we use virtual methods here, we cannot use templates. Only one type of dynamic typing allowed
     * by C++, either runtime (virtual) or compile time (template).
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, float>> getPlans_float(const MPICartesianGroup &group,
                                                                          const FFTLayoutStruct<NDim> &layout) = 0;
    /** @brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
     * shared_ptr's.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, double>> getPlans_double(const MPICartesianGroup &group,
                                                                            const FFTLayoutStruct<NDim> &layout) = 0;
  };

} // namespace TempLat

#endif
