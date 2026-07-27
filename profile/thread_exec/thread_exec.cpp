#include <mpi.h>

#include <omp.h>
#include <stdio.h>

#include <mutex>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  // output openmp thread id and mpi rank, lock the output to avoid interleaving
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::mutex mtx;
  int nthreads, tid;

// Begin of parallel region
#pragma omp parallel private(nthreads, tid)
  {
    // Getting thread number
    tid = omp_get_thread_num();
    printf("Hello from MPI rank %d, thread = %d\n", rank, tid);

    // barrier
#pragma omp barrier
    if (tid == 0) {
      // Only master thread does this
      nthreads = omp_get_num_threads();
      printf("\nNumber of threads on MPI rank %d is %d\n", rank, nthreads);
    }
  }

  MPI_Finalize();
  return 0;
}
