#ifndef COSMOINTERFACE_HELPERS_POTENTIAL_H
#define COSMOINTERFACE_HELPERS_POTENTIAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/complexalgebra/complexalgebra.h"
#include "TempLat/lattice/algebra/su2algebra/su2algebra.h"

namespace TempLat {

    /** \brief A class which manipulates the potential and the its derivatives.
     *
     * 
     **/

    class Potential {
    public:
        /* Put public methods here. These should change very little over time. */
        Potential() = delete;
    private:
    
    	// This function converts, for complex scalars and SU(2) doublets, first derivatives wrt to norm to first derivatives wrt the components.
    	// Used e.g. to set the initial fluctuations of the components.
        template<class NormDeriv, class Fld, int I>
        static inline auto derivComponentFromNorm(NormDeriv nd, Fld fld, Tag<I> index) 
        {
            return nd / norm(fld) * fld(index);
        }
	    	
    	// This function converts, for complex scalars and SU(2) doublets, second derivatives wrt to norm to second derivatives wrt the components. 
    	// Used e.g. to set the initial fluctuations of the components.
        template<class NormDeriv, class NormDeriv2, class Fld, int I>
        static inline auto deriv2ComponentFromNorm(NormDeriv nd, NormDeriv2 nd2, Fld fld, Tag<I> index) 
        {
            return 1.0 / norm2(fld) * nd2 * pow<2>(fld(index)) + nd / norm(fld) * (1 - pow<2>(fld(index))/norm2(fld));
        }

    public:

		// Total potential energy (sum of all its terms)
        template<class Model>
        static inline auto potential(Model& pM)
        {
            return Total(i,0,Model::NPotTerms-1,pM.potentialTerms(i));
        }
        
		// First derivative of the potential wrt scalar field n.
        template<class Model, int N>
        static inline auto derivS(Model& model, Tag<N> n)
        {
            return model.potDeriv(n); // calls function in model file
        }
        
        // Creates an array with the different first derivatives wrt all scalar fields. 
        // IfElse is to avoid compilation errors when no scalars are specified.
        template<class Model>
        static inline auto derivS(Model& model) 
        {
            return IfElse((Model::Ns>0),MakeArray(i,0,Model::Ns-1, model.potDeriv(i)), ZeroType());
        }
        
		template<class Model>
        static inline auto derivSSI(Model& model) 
        {
            return IfElse((Model::Ns>0),MakeArray(i,0,Model::Ns-1, derivSSI(i)), ZeroType());
        }
		
		
		// Second derivative of the potential wrt scalar field n. 
        template<class Model, int N>
        static inline auto deriv2S(Model& model, Tag<N> n) 
        {
            return model.potDeriv2(n);
        }

		// Returns a complex number composed by the first derivatives of the potential wrt components 0 and 1 of the complex scalar "n".
        template<class Model, int N>
        static inline auto derivCS(Model& model, Tag<N> n) 
        {
            return  Complexify(
                    derivComponentFromNorm(model.potDerivNormCS(n), model.fldCS(n), 0_c),
                    derivComponentFromNorm(model.potDerivNormCS(n), model.fldCS(n), 1_c)
                    );
        }

		// Returns a complex number composed by the second derivatives of the potential wrt components 0 and 1 of the complex scalar "n".
        template<class Model, int N>
        static inline auto deriv2CS(Model& model, Tag<N> n) 
        {
            return Complexify(
                    deriv2ComponentFromNorm(model.potDerivNormCS(n), model.potDeriv2NormCS(n),model.fldCS(n), 0_c),
                    deriv2ComponentFromNorm(model.potDerivNormCS(n), model.potDeriv2NormCS(n),model.fldCS(n), 1_c)
                    );
        }

		// Returns a doublet composed by the first derivatives of the potential wrt the components of the SU2 doublet "n".   
        template<class Model, int N>
        static inline auto derivSU2Doublet(Model& model, Tag<N> n) 
        {
            return  MakeSU2Doublet(a,
                    derivComponentFromNorm(model.potDerivNormSU2Doublet(n), model.fldSU2Doublet(n), a)
            );
        }
        
        // Returns a doublet composed by the second derivatives of the potential wrt the components of the SU2 doublet "n".
        template<class Model, int N>
        static inline auto deriv2SU2Doublet(Model& model, Tag<N> n) 
        {
            return MakeSU2Doublet(a,
                    deriv2ComponentFromNorm(model.potDerivNormSU2Doublet(n), model.potDeriv2NormSU2Doublet(n),model.fldSU2Doublet(n), a)
            );
        }


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/definitions/potential_test.h"
#endif


#endif
