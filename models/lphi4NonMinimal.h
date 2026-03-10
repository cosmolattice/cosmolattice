#ifndef LPHI4NONMINIMAL_H
#define LPHI4NONMINIMAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

#include "CosmoInterface/cosmointerface.h"

namespace TempLat
{
    struct ModelPars : public TempLat::DefaultModelPars {
        static constexpr size_t NScalars = 2;
        static constexpr size_t NPotTerms = 2;

        // Non-minimal coupling: only the second scalar (index 1) couples to gravity
        typedef CouplingsManager<NScalars, 1, false, true> NonMinimalCouplings;
    };

  #define MODELNAME lphi4NonMinimal

  template<class R>
  using Model = MakeModel(R, ModelPars);

  class MODELNAME : public Model<MODELNAME>
  {
  private:
    double g, lambda, q;

  public:
    static constexpr size_t NDim = Model<MODELNAME>::NDim;

    MODELNAME(ParameterParser& parser, RunParameters<double>& runPar,
              device::memory::host_ptr<MemoryToolBox<NDim>> toolBox)
        : Model<MODELNAME>(parser, runPar.getLatParams(), toolBox, runPar.dt, STRINGIFY(MODELLABEL))
    {
        lambda = parser.get<double>("lambda");
        q = parser.get<double>("q");
        g = sqrt(q * lambda);

        fldS0 = parser.get<double, 2>("initial_amplitudes");
        piS0 = parser.get<double, 2>("initial_momenta", {0, 0});

        alpha = 1;
        fStar = fldS0[0];
        omegaStar = sqrt(lambda) * fStar;

        setInitialPotentialAndMassesFromPotential();
    }

    auto potentialTerms(Tag<0>)
    {
        return 0.25 * pow<4>(fldS(0_c));
    }

    auto potentialTerms(Tag<1>)
    {
        return 0.5 * q * pow<2>(fldS(0_c) * fldS(1_c));
    }

    auto potDeriv(Tag<0>)
    {
        return pow<3>(fldS(0_c)) + q * fldS(0_c) * pow<2>(fldS(1_c));
    }

    auto potDeriv(Tag<1>)
    {
        return q * fldS(1_c) * pow<2>(fldS(0_c));
    }

    auto potDeriv2(Tag<0>)
    {
        return 3 * pow<2>(fldS(0_c)) + q * pow<2>(fldS(1_c));
    }

    auto potDeriv2(Tag<1>)
    {
        return q * pow<2>(fldS(0_c));
    }
  };
}

#endif
