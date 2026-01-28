#ifndef TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTPLANHOLDER_H
#define TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTPLANHOLDER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2026

#ifndef NOFFT
#ifndef NOMPI
#ifndef NOPARAFAFT
#include <parafaft_r2c.hpp>
#endif
#endif
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"
#include "TempLat/lattice/memory/memoryblock.h"

#include <memory>
#include <complex>

namespace TempLat
{

  MakeException(ParafaftPlanHolderException);
  MakeException(ParafaftCompiledWithoutSinglePrecisionSupport);

  /** \brief Plan holder for parafaft FFT transforms.
   *
   * This class holds a parafaft::ParaFaFT_R2C object and implements the
   * PlanInterface<T> methods for r2c and c2r transforms.
   *
   * Note: Parafaft only supports double precision currently.
   *
   * Unit test: make test-parafaftplanholder
   **/

  template <size_t NDim, typename T> class ParafaftPlanHolder : public FFTPlanInterface<NDim, T>
  {
  public:
    using Complex = std::complex<double>;

#ifndef NOMPI
#ifndef NOPARAFAFT
    /**
     * @brief Constructor - takes ownership of the parafaft object.
     *
     * We store the MPICartesianGroup to keep the MPI communicator alive.
     */
    ParafaftPlanHolder(MPICartesianGroup group, std::shared_ptr<parafaft::ParaFaFT_R2C<NDim>> parafaftObj)
        : mGroup(group), mParafaft(parafaftObj)
    {
    }
#endif
#endif

    virtual ~ParafaftPlanHolder()
    {
      say << "ParafaftPlanHolder is correctly being destructed.\n";
      // Parafaft cleans up internally via its destructor
    }

    /**
     * @brief Complex-to-real backward transform (in-place).
     *
     * Parafaft's backward() copies input to internal buffers before processing,
     * so using the same memory for both is safe.
     */
    virtual void c2r(MemoryBlock<NDim, T> &mBlock) override { execute_c2r(mBlock); }

    /**
     * @brief Real-to-complex forward transform (in-place).
     *
     * Parafaft's forward() copies input to internal buffers before processing,
     * so using the same memory for both is safe.
     */
    virtual void r2c(MemoryBlock<NDim, T> &mBlock) override { execute_r2c(mBlock); }

  private:
#ifndef NOMPI
#ifndef NOPARAFAFT
    // Keep group alive for MPI communicator lifetime
    MPICartesianGroup mGroup;

    // Shared pointer to parafaft object
    std::shared_ptr<parafaft::ParaFaFT_R2C<NDim>> mParafaft;

#endif
#endif

    // Double precision implementation using in-place padded buffer API
    template <typename S = T>
      requires std::is_same<S, double>::value
    void execute_r2c(MemoryBlock<NDim, S> &mBlock)
    {
#ifndef NOMPI
#ifndef NOPARAFAFT
      // Parafaft's forward_in_place accepts padded buffers directly
      // Buffer layout: [N0_local][N1_local][2*(N/2+1)] - matches CosmoLattice
      auto block_view = mBlock.getRawHostView();
      mParafaft->forward_in_place(block_view.data());
      mBlock.pushHostView();
#endif
#endif
    }

    // Double precision implementation using in-place padded buffer API
    template <typename S = T>
      requires std::is_same<S, double>::value
    void execute_c2r(MemoryBlock<NDim, S> &mBlock)
    {
#ifndef NOMPI
#ifndef NOPARAFAFT
      // Parafaft's backward_in_place accepts padded buffers directly
      // Buffer layout: [N0_local][N1_local][2*(N/2+1)] - matches CosmoLattice
      auto block_view = mBlock.getRawHostView();
      mParafaft->backward_in_place(block_view.data());
      mBlock.pushHostView();
#endif
#endif
    }

    // Float precision - not supported
#ifndef NOFFTFLOAT
    template <typename S = T>
    typename std::enable_if<std::is_same<S, float>::value, void>::type execute_r2c(MemoryBlock<S> &mBlock)
    {
      throw ParafaftCompiledWithoutSinglePrecisionSupport("Parafaft does not support single precision.");
    }

    template <typename S = T>
    typename std::enable_if<std::is_same<S, float>::value, void>::type execute_c2r(MemoryBlock<S> &mBlock)
    {
      throw ParafaftCompiledWithoutSinglePrecisionSupport("Parafaft does not support single precision.");
    }
#endif

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
