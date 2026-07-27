#ifndef TEMPLAT_LATTICE_ALGEBRA_RANDOM_PHASEBUNCHDAVIES_H
#define TEMPLAT_LATTICE_ALGEBRA_RANDOM_PHASEBUNCHDAVIES_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Ander Urio,  Year: 2025

#include <complex>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/coordinates/dimensioncountrecorder.h"
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"

// The template we use to properly set the e^{ik/aH} and the -i*e^{i/kaH} phase of the gauge field and electric field BD
// solution

// It does this process taking into account the hermiticity conditions of the R2C case of FFTW

namespace TempLat
{

  template <typename T, size_t NDim, bool gauge> class BDPhaseHelper : public DimensionCountRecorder<NDim>
  {
  public:
    using ToolboxPtr = device::memory::host_ptr<MemoryToolBox<NDim>>;

    BDPhaseHelper(ToolboxPtr pToolBox, T kIR, T aI)
        : DimensionCountRecorder<NDim>(SpaceStateType::Fourier), mLayout(pToolBox->mLayouts.getFourierSpaceLayout()),
          mkIR(kIR), maI(aI)
    {
    }

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION complex<T> eval(const IDX &...idx) const
    {
      device::IdxArray<NDim> coord;
      mLayout.putSpatialLocationFromMemoryIndexInto(coord, idx...);

      device::IdxArray<NDim> hermitianPartner;
      const auto hermitianType =
          this->getCurrentLayout().getHermitianPartners().putHermitianPartner(coord, hermitianPartner);

      T ki2 = 0;
      for (size_t d = 0; d < NDim; ++d)
        ki2 += coord[d] * coord[d];
      const T ki = mkIR * sqrt(ki2);

      device::array<T, 2> pair;
      if constexpr (gauge) {
        pair = {cos(ki / maI), sin(ki / maI)};
      } else {
        pair = {sin(ki / maI), -cos(ki / maI)};
      }

      return complex<T>(
          (hermitianType == HermitianRedundancy::none || hermitianType == HermitianRedundancy::positivePartner ||
           hermitianType == HermitianRedundancy::negativePartner)
              ? pair[0]
              : T(1),
          (hermitianType == HermitianRedundancy::none || hermitianType == HermitianRedundancy::positivePartner)
              ? pair[1]
              : (hermitianType == HermitianRedundancy::negativePartner ? -pair[1] : T(0)));
    }

  private:
    LayoutStruct<NDim> mLayout;
    T mkIR;
    T maI;
  };

  template <typename T, size_t NDim> using BDPhasePi2A = BDPhaseHelper<T, NDim, true>;

  template <typename T, size_t NDim> using BDPhasePi2E = BDPhaseHelper<T, NDim, false>;
} // namespace TempLat

#endif
