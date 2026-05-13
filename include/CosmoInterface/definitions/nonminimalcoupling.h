#ifndef COSMOINTERFACE_DEFINITIONS_NONMINIMALCOUPLING_H
#define COSMOINTERFACE_DEFINITIONS_NONMINIMALCOUPLING_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

#include "CosmoInterface/definitions/averages.h"
#include "CosmoInterface/definitions/energies.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/util/templatvector.h"

namespace TempLat {

    class NonMinimalCoupling {
    public:
        NonMinimalCoupling() = delete;

        template<class Model>
        static auto F(Model& model) {
            auto sum = Total(i, 0, Model::Ns - 1,
              IfElse(Model::NonMinimalCouplings::couples(i, Tag<0>()),
                (6.0 * model.xis(i, 0_c) - 1.0) * model.xis(i, 0_c) * pow<2>(model.fStar / model.MPl) * model.fld2AvSI_i(i),
                ZeroType()
            ));
            return 1.0 / (1.0 + sum);
        }

        template<class Model>
        static auto R(Model& model) {
            Averages::setAllAverages(model);

            auto norm = pow<2>(model.fStar / Model::MPl) * NonMinimalCoupling::F(model);
            auto Eks = (Model::Ns > 0 ? Energies::kineticS(model) : 0);
            auto Ekcs = (Model::NCs > 0 ? Energies::kineticCS(model) : 0);
            auto EkSU2Dbl = (Model::NSU2Doublet > 0 ? Energies::kineticSU2Doublet(model) : 0);
            auto Egs = (Model::Ns > 0 ? Energies::gradientS(model) : 0);
            auto Egcs = (Model::NCs > 0 ? Energies::gradientCS(model) : 0);
            auto EgSU2Dbl = (Model::NSU2Doublet > 0 ? Energies::gradientSU2Doublet(model) : 0);
            auto EelU1 = (Model::NU1 > 0 ? Energies::electricU1(model) : 0);
            auto EmagU1 = (Model::NU1 > 0 ? Energies::magneticU1(model) : 0);
            auto EelSU2 = (Model::NSU2 > 0 ? Energies::electricSU2(model) : 0);
            auto EmagSU2 = (Model::NSU2 > 0 ? Energies::magneticSU2(model) : 0);

            auto rhoMinimal = Eks + Ekcs + EkSU2Dbl + Egs + Egcs + EgSU2Dbl + EelU1 + EmagU1 + EelSU2 + EmagSU2 +
                              model.potAvI;
            auto pMinimal = Eks + Ekcs + EkSU2Dbl - (1.0 / 3.0) * (Egs + Egcs + EgSU2Dbl) +
                            (1.0 / 3.0) * (EelU1 + EmagU1 + EelSU2 + EmagSU2) - model.potAvI;
            auto minimalContribution = rhoMinimal - 3.0 * pMinimal;
            auto nonMinimalContribution = Total(i, 0, Model::Ns - 1,
                IfElse(Model::NonMinimalCouplings::couples(i, Tag<0>()),
                  12 * model.xis(i, 0_c) * (model.pi2AvSI_i(i) - model.grad2AvSI_i(i)) - 6.0 * model.xis(i, 0_c) * model.fldVpAvSI(i),
                  ZeroType())
            );
            return norm * (minimalContribution + nonMinimalContribution);
        }
    };

} /* TempLat */

#endif
