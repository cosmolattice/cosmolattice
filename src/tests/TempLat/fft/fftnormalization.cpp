
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/fft/fftnormalization.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/fft/fftmpidomainsplit.h"

namespace TempLat
{

  struct FFTNormalizationTester {
    static void Test(TDDAssertion &tdd)
    {
      device::IdxArray<3> nGrid;
      for (size_t i = 0; i < 3; ++i)
        nGrid[i] = 256;

      const double baseNorm = std::pow(nGrid[0], 3);

      auto split = FFTMPIDomainSplit<3>::makeDomainDecomposition(MPICommReference().size(), 3);
      FFTLibrarySelector<3> ffter(MPICartesianGroup(3, split), nGrid);

      FFTLayoutStruct layout = ffter.getLayout();
      auto iScales = layout.getIntrinsicScales();

      FFTNormalization<3> normalizer(layout);

      const ptrdiff_t iEnd = layout.getMinimalMemorySize();

      MemoryBlock<3, double> mem(iEnd);
      auto host_mem_view = mem.getRawHostView();
      auto mem_view = mem.getRawView();

      auto zeroMemory = DEVICE_LAMBDA(const device::IdxArray<1> &i) { mem_view[i[0]] = i[0]; };

      auto &&doTest = [&](auto expectedNormC2R, auto expectedNormR2C) {
        device::iteration::foreach ("InitMem", device::IdxArray<1>{0}, device::IdxArray<1>{(device::Idx)iEnd},
                                    zeroMemory);
        device::iteration::fence();

        normalizer.c2r(mem, 1. / iScales.c2r);
        mem.flagHostMirrorOutdated();

        bool c2rAllgood = true;
        mem.pullHostView();
        for (ptrdiff_t i = 0; i < iEnd; ++i) {
          c2rAllgood = c2rAllgood && AlmostEqual(host_mem_view[i], expectedNormC2R * i / iScales.c2r);
          if (!c2rAllgood) {
            sayShort << "at " << i << ": " << host_mem_view[i] << " != " << expectedNormC2R * i / iScales.c2r << "\n";
            break;
          }
        }
        tdd.verify(c2rAllgood);

        normalizer.r2c(mem, 1 / iScales.r2c);
        mem.flagHostMirrorOutdated();

        bool r2cAllgood = true;
        mem.pullHostView();
        for (ptrdiff_t i = 0; i < iEnd; ++i) {
          r2cAllgood = r2cAllgood && AlmostEqual(host_mem_view[i], expectedNormR2C * i / iScales.r2c);
          if (!r2cAllgood) {
            sayShort << "at " << i << ": " << host_mem_view[i] << " != " << expectedNormR2C * i / iScales.r2c << "\n";
            break;
          }
        }

        tdd.verify(r2cAllgood);
      };

      sayShort << "Testing Config normalization...\n";
      normalizer.setToConfigType();
      doTest(1. / baseNorm, 1. / baseNorm);

      sayShort << "Testing Fourier normalization...\n";
      normalizer.setToFourierType();
      doTest(1., 1. / baseNorm);

      sayShort << "Testing Mixed normalization...\n";
      normalizer.setToMixedType();
      doTest(1. / std::sqrt(baseNorm), 1. / baseNorm);
    }
  };

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FFTNormalizationTester> test;
}
