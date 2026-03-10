#ifndef COSMOINTERFACE_DEFINITIONS_NONMINIMALCOUPLING_H
#define COSMOINTERFACE_DEFINITIONS_NONMINIMALCOUPLING_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

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
            auto norm = pow<2>(model.fStar / Model::MPl) * NonMinimalCoupling::F(model);
            auto minimalContribution = Energies::rhoMinimal(model) - 3.0 * Energies::pMinimal(model);
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
