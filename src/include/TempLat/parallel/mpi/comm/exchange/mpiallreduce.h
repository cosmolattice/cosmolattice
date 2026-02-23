#ifndef TEMPLAT_PARALLEL_MPI_COMM_EXCHANGE_MPIALLREDUCE_H
#define TEMPLAT_PARALLEL_MPI_COMM_EXCHANGE_MPIALLREDUCE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <vector>
#include <array>

#include "TempLat/parallel/devices/kokkos/kokkos.h"
#include "TempLat/util/exception.h"
#include "TempLat/util/log/saycomplete.h"
#include "TempLat/parallel/mpi/mpitypeconstants.h"
#include "TempLat/parallel/device.h"

namespace TempLat
{
  MakeException(MPIAllReduceException);

  /** @brief A class which calls MPI_Allreduce, meaning that all
   *   processes submit their value and receive the combined value of
   *   all processes, given an operation to combine the values.
   *
   * Unit test: ctest -R test-mpiallreduce
   **/
  class MPIAllReduce
  {
  public:
    // Put public methods here. These should change very little over time.
    MPIAllReduce(MPI_Comm comm) : mComm(comm) {}

#ifdef HAVE_MPI
    /** @brief Makes a copy of your value, applies the reduction
     *  in-place on the copied value, returns the resulting value.
     */
    template <typename T>
      requires requires { MPITypeSelect<T>(); }
    T Allreduce(const T &value, MPI_Op operation, ptrdiff_t size = 1, int *error = nullptr)
    {
      T copyValue = value;
      int myerror = MPI_SUCCESS != MPI_Allreduce(MPI_IN_PLACE, &copyValue, size, MPITypeSelect<T>(), operation, mComm);
      if (error) {
        *error = myerror;
      } else {
        combineAllError(myerror);
      }
      return copyValue;
    }

    /** @brief The vector / array versions perform the reduction in-
     *  place, you loose your original array.
     */
    template <typename T>
    std::vector<T> &Allreduce(std::vector<T> *value, MPI_Op operation, ptrdiff_t size = 0, int *error = nullptr)
    {
      int myerror = MPI_SUCCESS !=
                    MPI_Allreduce(MPI_IN_PLACE, value->data(), value->size(), MPITypeSelect<T>(), operation, mComm);
      if (error) {
        *error = myerror;
      } else {
        combineAllError(myerror);
      }
      return *value;
    }

    /** @brief The vector / array versions perform the reduction in-
     *  place, you loose your original array.
     */
    template <typename View>
      requires requires(View view) {
        typename View::value_type;
        typename View::size_type;
        view.data();
        MPITypeSelect<typename View::value_type>();
      }
    View Allreduce(View value, MPI_Op operation, ptrdiff_t size = 0, int *error = nullptr)
    {
      using T = typename View::value_type;
      int myerror =
          MPI_SUCCESS != MPI_Allreduce(MPI_IN_PLACE, value.data(), value.size(), MPITypeSelect<T>(), operation, mComm);
      if (error) {
        *error = myerror;
      } else {
        combineAllError(myerror);
      }
      return value;
    }

    /** @brief The vector / array versions perform the reduction in-
     *  place, you loose your original array.
     */
    template <typename T, size_t N>
    std::vector<std::array<T, N>> &Allreduce(std::vector<std::array<T, N>> *value, MPI_Op operation, ptrdiff_t size = N,
                                             int *error = nullptr)
    {
      int myerror = MPI_SUCCESS !=
                    MPI_Allreduce(MPI_IN_PLACE, value->data(), value->size() * N, MPITypeSelect<T>(), operation, mComm);
      if (error) {
        *error = myerror;
      } else {
        combineAllError(myerror);
      }
      return *value;
    }

    /** @brief The vector / array versions perform the reduction in-
     *  place, you loose your original array.
     */
    template <typename T, size_t N>
    std::array<T, N> &Allreduce(std::array<T, N> *value, MPI_Op operation, ptrdiff_t size = N, int *error = nullptr)
    {
      int myerror = MPI_SUCCESS != MPI_Allreduce(MPI_IN_PLACE, value->data(), N, MPITypeSelect<T>(), operation, mComm);
      if (error) {
        *error = myerror;
      } else {
        combineAllError(myerror);
      }
      return *value;
    }

    /** @brief Treat complex values as array of size 2.
     */
    template <typename T>
    complex<T> Allreduce(complex<T> value, MPI_Op operation, ptrdiff_t size = 2, int *error = nullptr)
    {
      std::array<double, 2u> realView{{value.real(), value.imag()}};
      Allreduce(&realView, operation);
      return complex<T>(realView[0], realView[1]);
    }

#else
    /** @brief Dummy's for the MPI-less compilation. */
    template <typename T> const T &Allreduce(const T &whatever, MPI_Op operation, ptrdiff_t size = 1, int *error = nullptr)
    {
      return whatever;
    }

    /** @brief Dummy's for the MPI-less compilation. */
    template <typename T> T &Allreduce(T *whatever, MPI_Op operation, int *error = nullptr) { return *whatever; }
#endif
    template <typename T> T computeAllSum(const T &value, size_t size = 1, int *error = nullptr)
    {
      return Allreduce(value, MPI_SUM, size, error);
    }

    template <typename T> T computeAllOr(const T &value, size_t size = 1, int *error = nullptr)
    {
      return Allreduce(value, MPI_LOR, size, error);
    }

    template <typename T> T computeAllAnd(const T &value, size_t size = 1, int *error = nullptr)
    {
      return Allreduce(value, MPI_LAND, size, error);
    }

    template <typename T> T computeAllMin(const T &value, size_t size = 1, int *error = nullptr)
    {
      return Allreduce(value, MPI_MIN, size, error);
    }

    template <typename T> T computeAllMax(const T &value, size_t size = 1, int *error = nullptr)
    {
      return Allreduce(value, MPI_MAX, size, error);
    }

    int combineAllError(int error)
    {
      int nextError;
      computeAllOr(error, 1, &nextError);
      int result = (error || nextError);
      if (result) throw MPIAllReduceException("Received error from MPI after call to MPI_Allreduce.");
      return result;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    MPI_Comm mComm;

    void useMCommToAvoidWarningUnused()
    {
      if (!mComm) {
        say << "Hello world.\n This function is never called.\n";
      }
    }
  };
} // namespace TempLat

#endif
