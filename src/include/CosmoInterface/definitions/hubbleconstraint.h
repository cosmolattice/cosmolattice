#ifndef COSMOINTERFACE_HELPERS_HUBBLELAWS_H
#define COSMOINTERFACE_HELPERS_HUBBLELAWS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/lattice/algebra/operators/operators.h"
#include "CosmoInterface/definitions/energies.h"
#include "TempLat/util/templatvector.h"

namespace TempLat {


    /** \brief A class that computes the hubble law in order to monitor "energy conservation"
     *
     * 
     **/

    class HubbleConstraint {
    public:
        /* Put public methods here. These should change very little over time. */
        HubbleConstraint() {

        }

		// This function computes the degree of conservation of the 1st Friedmann equation (H^2 \propto \rho).
        template<class Model>
        static auto get(Model& model)
        {
            auto rho = Energies::rho(model);

            auto LHS = pow<2>(model.aDotI);  // left hand side
            auto RHS = pow(model.aI, 2 * model.alpha + 2) / 3. * pow<2>(model.fStar / Model::MPl) * rho;  // right hand side

            // Returns a vector, whose 0 component is the relative degree of conservation.
            return make_templatvector((LHS - RHS) / (LHS + RHS), LHS, RHS);
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/definitions/hubbleconstraint_test.h"
#endif


#endif
