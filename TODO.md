# Open issues

- There are problems with 1D fourier transforms in `test-normgradientsquare`
- There are problems with 2D fourier transforms in `test-latticelaplacian`
- There should be something in place to make sure that running 1D sims with MPI throws.
- In the new algebra, some of the scalar algebra is deactivated to not conflict with SU2 with !HasSU2Get. This is backward, SU2 should deactivate the scalar algebra. What makes it tricky is that the scalar algebra does not have any unique function anymore, just eval. Need to think about this.
- Big parts of the code examples (in the docs) are now slightly broken, as we have changed quite a lot. Needs manual fixing. AF: They now all compile on CPU.
- ParaFaFT memory size does not seem to be correctly applied - needs to be fixed.
