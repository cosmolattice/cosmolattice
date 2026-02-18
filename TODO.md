# Open issues

- There are problems with 1D fourier transforms in `test-normgradientsquare`
- There are problems with 2D fourier transforms in `test-latticelaplacian`
- Correctly implement PostGet/PreGet for other things than just the bare field.
- Offline compilation must work flawlessly
- Make sure HIP works.
- Get rid of the JumpsHolder
- Remove all explicit Kokkos:: and replace with device:: analogues.
- I'm pretty sure confirmSpace is currently not working. 
- Expression trees do not need to be copyable on GPU anymore - so remove the additional code that made it possible.