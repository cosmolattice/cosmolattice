#ifndef NMCLPHI4_H
#define NMCLPHI4_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

#include "CosmoInterface/cosmointerface.h"

namespace TempLat
{
    // @label:model_pars
    struct ModelPars : public TempLat::DefaultModelPars {
        static constexpr size_t NScalars = 2;
        static constexpr size_t NPotTerms = 1;

        // Non-minimal coupling: the second scalar (index 1) couples non-minimally to gravity. The first scalar (index 0) only couples minimally to gravity.
        typedef CouplingsManager<NScalars, 1, false, true> NonMinimalCouplings;
    };
    // @endlabel

  // @label:model_name
  #define MODELNAME NMC_lphi4
  // @endlabel

  // @label:make_model
  template<class R>
  using Model = MakeModel(R, ModelPars);
  // @endlabel

  // @label:class_declaration
  class MODELNAME : public Model<MODELNAME>
  {
  private:
  // @endlabel
    // @label:private_members
    double lambda;
    // @endlabel

  public:
    static constexpr size_t NDim = Model<MODELNAME>::NDim;

    // @label:constructor_params
    MODELNAME(ParameterParser& parser, RunParameters<double>& runPar,
              device::memory::host_ptr<MemoryToolBox<NDim>> toolBox)
        : Model<MODELNAME>(parser, runPar.getLatParams(), toolBox, runPar.dt, STRINGIFY(MODELLABEL))
    {
        lambda = parser.get<double>("lambda");
    // @endlabel

        // @label:initial_conditions
        fldS0 = parser.get<double, 2>("initial_amplitudes");
        piS0 = parser.get<double, 2>("initial_momenta", {0, 0});
        // @endlabel

        // @label:rescaling
        alpha = 1;
        fStar = fldS0[0];
        omegaStar = sqrt(lambda) * fStar;
        // @endlabel

        // @label:masses_setup
        setInitialPotentialAndMassesFromPotential();
        // @endlabel
    }

    // @label:potential_terms
    auto potentialTerms(Tag<0>)
    {
        return 0.25 * pow<4>(fldS(0_c));
    }

    // @endlabel

    // @label:potential_derivs
    auto potDeriv(Tag<0>)
    {
        return pow<3>(fldS(0_c));
    }
    auto potDeriv(Tag<1>)
    {
        return ZeroType();
    }
    // @endlabel

    // @label:potential_second_derivs
    auto potDeriv2(Tag<0>)
    {
        return 3 * pow<2>(fldS(0_c));
    }
    auto potDeriv2(Tag<1>)
    {
        return ZeroType();
    }
    // @endlabel

  };
}

#endif
