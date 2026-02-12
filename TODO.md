# Open issues

- There are problems with 1D fourier transforms in `test-normgradientsquare`
- There are problems with 2D fourier transforms in `test-latticelaplacian`
- Change the way kokkos is downloaded to ExternalProjec_add
- Corretly implement PostGet/PreGet for other things than just the bare field.

- Write device::string, which is never copied onto the device. Can be a stupid container that allocates a single string on the heap.