#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_SCALEFACTORKERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_SCALEFACTORKERNELS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/definitions/energies.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/operators.h"

namespace TempLat {


    /** \brief A class that computes the kernel for the scale factor equation.
     *
     * 
     * Unit test: make test-scalefactorkernels
     **/

    class ScaleFactorKernels {
    public:
        /* Put public methods here. These should change very little over time. */
        ScaleFactorKernels() {

        }

        template <class Model>
        static auto get(Model& model)
        {
            using T = decltype(model.aI);

			// The kernel of the scale factor is sourced by the different volume-averaged energy densities:			
            T Eks = 0;
            T Egs = 0;
            T Ekcs = 0;
            T Egcs = 0;
            T EkSU2Dbl = 0;
            T EgSU2Dbl = 0;
            T EmagU1 = 0;
            T EelU1 = 0;
            T EmagSU2 = 0;
            T EelSU2 = 0;

            if(model.Ns > 0){ 
                Eks = Energies::kineticS(model);  //  scalar singlet, kinetic energy
                Egs = Energies::gradientS(model);  //  scalar singlet, gradient energy
            }
            if(model.NCs > 0){
                Ekcs = Energies::kineticCS(model);  // complex scalar, kinetic energy
                Egcs = Energies::gradientCS(model);  // complex scalar, gradient energy
            }
            if(model.NSU2Doublet > 0){
                EkSU2Dbl = Energies::kineticSU2Doublet(model);  // SU2 doublet, kinetic energy
                EgSU2Dbl = Energies::gradientSU2Doublet(model);  // SU2 doublet, gradient energy
            }
            if(model.NU1 > 0){
                EelU1 = Energies::electricU1(model);  // U1 gauge fields, electric energy
                EmagU1 = Energies::magneticU1(model);  // U1 gauge fields, magnetic energy
            }
            if(model.NSU2 > 0){
                EelSU2 = Energies::electricSU2(model);  // SU2 gauge fields, electric energy
                EmagSU2 = Energies::magneticSU2(model);  // SU2 gauge fields, magnetic energy
            }

			// Returns kernel for the scale factor:
            return pow(model.aI, 2 * model.alpha + 1) / 3.0 * pow<2>(model.fStar / Model::MPl) *
                   ((model.alpha - 2) * (Eks + Ekcs + EkSU2Dbl) + model.alpha * (Egs + Egcs + EgSU2Dbl) +
                    (model.alpha - 1) * (EelU1 + EmagU1 + EelSU2 + EmagSU2) + (model.alpha + 1) * model.potAvI);
        }
		
		template <class Model>
        static auto getSI(Model& model)
        {
            using T = decltype(model.aI);

			// The kernel of the scale factor is sourced by the different volume-averaged energy densities:			
            T Eks = 0;
            T Egs = 0;
            T Ekcs = 0;
            T Egcs = 0;
            T EkSU2Dbl = 0;
            T EgSU2Dbl = 0;
            T EmagU1 = 0;
            T EelU1 = 0;
            T EmagSU2 = 0;
            T EelSU2 = 0;

            if(model.Ns > 0){ 
                Eks = Energies::kineticSSI(model);  //  scalar singlet, kinetic energy
                Egs = Energies::gradientSSI(model);  //  scalar singlet, gradient energy
            }
            if(model.NCs > 0){
                Ekcs = Energies::kineticCSSI(model);  // complex scalar, kinetic energy
                Egcs = Energies::gradientCSSI(model);  // complex scalar, gradient energy
            }
            if(model.NSU2Doublet > 0){
                EkSU2Dbl = Energies::kineticSU2DoubletSI(model);  // SU2 doublet, kinetic energy
                EgSU2Dbl = Energies::gradientSU2DoubletSI(model);  // SU2 doublet, gradient energy
            }
            if(model.NU1 > 0){
                EelU1 = Energies::electricU1SI(model);  // U1 gauge fields, electric energy
                EmagU1 = Energies::magneticU1SI(model);  // U1 gauge fields, magnetic energy
            }
            if(model.NSU2 > 0){
                EelSU2 = Energies::electricSU2SI(model);  // SU2 gauge fields, electric energy
                EmagSU2 = Energies::magneticSU2SI(model);  // SU2 gauge fields, magnetic energy
            }

			// Returns kernel for the scale factor:
            return pow(model.aI, 2 * model.alpha + 1) / 3.0 * pow<2>(model.fStar / Model::MPl) *
                   ((model.alpha - 2) * (Eks + Ekcs + EkSU2Dbl) + model.alpha * (Egs + Egcs + EgSU2Dbl) +
                    (model.alpha - 1) * (EelU1 + EmagU1 + EelSU2 + EmagSU2) + (model.alpha + 1) * model.potAvSI);
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
#include "CosmoInterface/evolvers/kernels/scalefactorkernels_test.h"
#endif


#endif
