#ifndef COSMOINTERFACE_ENERGIES_H
#define COSMOINTERFACE_ENERGIES_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/evolvers/evolvertype.h"
#include "CosmoInterface/definitions/averages.h"
#include "TempLat/lattice/algebra/operators/operators.h"

namespace TempLat {


    /** \brief A class which computes the energy when working with our verlet-type algorithms. Assumes that the averages have already been computed.
     *
     * 
     **/

    class Energies {
    public:
        /* Put public methods here. These should change very little over time. */
        Energies() {

        }
        
        // The following functions compute the different volume-averaged contributions to the energy density.
        // The required field functionals are computed in fieldfunctionals.h, and then passed to these functions (via the "fldf" parameter) to rescale them by the appropriate factors.
        
        // EXAMPLE: The kinetic energy of a given field can be called as "Energies::kineticS(model,FieldFunctionals::pi2S(model,i))" (see energiesmeasurer.h for other examples)
        // In this case, FieldFunctionals::pi2S gives <pi^2>, with pi=a^(3-alpha)*f' the momenta, and f' the time-derivative. Then, Energies::kineticS multiplies this by (1/2)*a^(-6) to obtain the energy.

        template<class Model, class T>  
        static inline auto kineticS(Model& model, T fldf) {  // scalar singlet: kinetic energy
            return 0.5 * fldf* pow<-6>(model.aI);
        }

        template<class Model, class T>  
        static inline auto gradientS(Model& model, T fldf) {  // scalar singlet: gradient energy
            return 0.5 * fldf * pow<-2>(model.aI);
        }
		
        template<class Model, class T>  
        static inline auto kineticCS(Model& model, T fldf) {  // complex scalar: kinetic energy
            return  fldf * pow<-6>(model.aI);
        }

        template<class Model, class T>
        static inline auto gradientCS(Model& model, T fldf) {   // complex scalar: gradient energy
            return  fldf * pow<-2>(model.aI);
        }
		
        template<class Model, class T>
        static inline auto kineticSU2Doublet(Model& model, T fldf) {  // SU(2) doublet: kinetic energy
            return  fldf * pow<-6>(model.aI);
        }

        template<class Model, class T>
        static inline auto gradientSU2Doublet(Model& model, T fldf) {   // SU(2) doublet: gradient energy
            return fldf * pow<-2>(model.aI);
        }
		
        template<class Model, class T>    // U(1) gauge field: electric energy
        static inline auto electricU1(Model& model, T fldf) {  //This expects field already rescaled by the coupling constant. That ways, works like scalar, for different gauge fields.
            return 0.5 * pow<2>( model.omegaStar / model.fStar) * pow<-4>(model.aI) * fldf;
        }

        template<class Model, class T>
        static inline auto magneticU1(Model& model, T fldf) {   // U(1) gauge field: magnetic energy
            return 0.5 * pow<2>( model.omegaStar / model.fStar)  * pow<-4>(model.aI) * fldf;
        }
		
        template<class Model, class T>   // SU(2) gauge field: electric energy
        static inline auto electricSU2(Model& model, T fldf)
        {
            return  0.5 * pow<2>( model.omegaStar / model.fStar)  * pow<-4>(model.aI) * fldf;
        }

        template<class Model, class T>    // SU(2) gauge field: magnetic energy
        static inline auto magneticSU2(Model& model, T fldf)
        {
            return  0.5 * pow<2>( model.omegaStar / model.fStar)  * pow<-4>(model.aI) * fldf;
        }
        
        // The below functions are the same with fldf=model.pi2AvI,model.grad2AvI...
        template<class Model>
        static inline auto kineticS(Model& model) {
            return kineticS(model, model.pi2AvI);
        }

        template<class Model>
        static inline auto gradientS(Model& model) {
            return gradientS(model, model.grad2AvI);
        }
        
         template<class Model>
        static inline auto kineticSSI(Model& model) {
            return kineticS(model, model.pi2AvSI);
        }

        template<class Model>
        static inline auto gradientSSI(Model& model) {
            return gradientS(model, model.grad2AvSI);
        }
		
        template<class Model>
        static inline auto kineticCS(Model& model) {
            return kineticCS(model, model.CSpi2AvI);
        }

        template<class Model>
        static inline auto gradientCS(Model& model) {
            return gradientCS(model, model.CSgrad2AvI);
        }
		
		 template<class Model>
        static inline auto kineticCSSI(Model& model) {
            return kineticCS(model, model.CSpi2AvSI);
        }

        template<class Model>
        static inline auto gradientCSSI(Model& model) {
            return gradientCS(model, model.CSgrad2AvSI);
        }
		
        template<class Model>
        static inline auto kineticSU2Doublet(Model& model) {
            return kineticSU2Doublet(model, model.SU2DblPi2AvI);
        }

        template<class Model>
        static inline auto gradientSU2Doublet(Model& model) {
            return gradientSU2Doublet(model, model.SU2DblGrad2AvI);
        }
		
		template<class Model>
        static inline auto kineticSU2DoubletSI(Model& model) {
            return kineticSU2Doublet(model, model.SU2DblPi2AvSI);
        }

        template<class Model>
        static inline auto gradientSU2DoubletSI(Model& model) {
            return gradientSU2Doublet(model, model.SU2DblGrad2AvSI);
        }
		
        template<class Model>
        static inline auto electricU1(Model& model) {
            return electricU1(model, model.U1pi2AvI);
        }

        template<class Model>
        static inline auto magneticU1(Model& model) {
            return magneticU1(model, model.U1Mag2AvI);
        }
        
        template<class Model>
        static inline auto electricU1SI(Model& model) {
            return electricU1(model, model.U1pi2AvSI);
        }

        template<class Model>
        static inline auto magneticU1SI(Model& model) {
            return magneticU1(model, model.U1Mag2AvSI);
        }

        template<class Model>
        static inline auto electricSU2(Model& model)
        {
            return electricSU2(model, model.SU2pi2AvI);
        }

        template<class Model>
        static inline auto magneticSU2(Model& model)
        {
            return   magneticSU2(model, model.SU2Mag2AvI);
        }
		
		template<class Model>
        static inline auto electricSU2SI(Model& model)
        {
            return electricSU2(model, model.SU2pi2AvSI);
        }

        template<class Model>
        static inline auto magneticSU2SI(Model& model)
        {
            return   magneticSU2(model, model.SU2Mag2AvSI);
        }
		
        template <class Model>
        static inline auto rho(Model& model)  // Total energy density (sum of all contributions)
        {
            Averages::setAllAverages(model);
            auto Eks = (model.Ns > 0 ?  kineticS(model) : 0);
            auto Ekcs =  (model.NCs > 0 ? kineticCS(model) : 0);
            auto EkSU2Dbl =  (model.NSU2Doublet > 0 ? kineticSU2Doublet(model) : 0);
            auto Egs =  (model.Ns > 0 ? gradientS(model) : 0 );
            auto Egcs =  (model.NCs > 0 ? gradientCS(model) : 0 );
            auto EgSU2Dbl =  (model.NSU2Doublet > 0 ? gradientSU2Doublet(model) : 0 );
            auto EelU1 =  (model.NU1 > 0 ? electricU1(model) : 0);
            auto EmagU1 =  (model.NU1 > 0 ? magneticU1(model) : 0);
            auto EelSU2 =  (model.NSU2 > 0 ? electricSU2(model) : 0);
            auto EmagSU2 =  (model.NSU2 > 0 ? magneticSU2(model) : 0);

            return  (
                    Eks + Ekcs + EkSU2Dbl
                    + Egs + Egcs + EgSU2Dbl
                    + EelU1 + EmagU1
                    + EelSU2 + EmagSU2
                    + model.potAvI);   // we also include the potential energy to the sum
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
#include "CosmoInterface/definitions/energies_test.h"
#endif


#endif
