#ifndef TEMPLAT_LATTICE_ALGEBRA_RANDOM_PHASEBUNCHDAVIES_H
#define TEMPLAT_LATTICE_ALGEBRA_RANDOM_PHASEBUNCHDAVIES_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Ander Urio,  Year: 2025

#include <complex>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/coordinates/dimensioncountrecorder.h"

//The template we use to properly set the e^{ik/aH} and the -i*e^{i/kaH} phase of the gauge field and electric field BD solution

//It does this process taking into account the hermiticity conditions of the R2C case of FFTW

namespace TempLat {

    template <size_t NDim, typename T, bool gauge>
    class PhaseHelper : public DimensionCountRecorder<NDim> {
    public:
        using ToolboxPtr = device::memory::host_ptr<MemoryToolBox<NDim>>;

        PhaseHelper(ToolboxPtr pToolBox, T kIR, T aI) : 
        DimensionCountRecorder<NDim>(SpaceStateType::undefined),
        mToolBox(pToolBox),
        mkIR(kIR),
        maI(aI)
        {
            this->confirmSpace(mToolBox->mLayouts.getFourierSpaceLayout(),
                               SpaceStateType::Fourier);
        }

        std::complex<T> get(const int& i) const {
            return get(mToolBox->getCoordFourier(i));
        }

        std::complex<T> get(const std::array<ptrdiff_t, NDim>& coord) const {
        std::array<ptrdiff_t, NDim> hermitianPartner;
        auto hermitianType = this->getCurrentLayout()
            .getHermitianPartners()
            .putHermitianPartner(coord, hermitianPartner);

        T ki = mkIR * sqrt( coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2] );

        std::array<T,2> pair;
        if constexpr (gauge) {
            pair = {std::cos(ki / maI), std::sin(ki / maI)};
        } else {
            pair = {std::sin(ki / maI), -std::cos(ki / maI)};
        }

        return std::complex<T>(
          (hermitianType == HermitianRedundancy::none || hermitianType == HermitianRedundancy::positivePartner || hermitianType == HermitianRedundancy::negativePartner)
                ? pair[0] : T(1),
          (hermitianType == HermitianRedundancy::none || hermitianType == HermitianRedundancy::positivePartner)
                ? pair[1] : 
          (hermitianType == HermitianRedundancy::negativePartner ? -pair[1] : T(0))
        );
    }

    DEVICE_FORCEINLINE_FUNCTION
    std::complex<T> eval(const ptrdiff_t& i, const ptrdiff_t& j, const ptrdiff_t& k) const {
       return get(std::array<ptrdiff_t, NDim>{i, j, k});
    }  

    private:
        ToolboxPtr mToolBox;
        T mkIR;
        T maI;
    };

    template <size_t NDim, typename T>
    using BDPhasePi2A = PhaseHelper<NDim, T, true>;

    template <size_t NDim, typename T>
    using BDPhasePi2E = PhaseHelper<NDim, T, false>;


    class PhaseTester{
        public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/random/phase_test.h"
#endif


#endif
