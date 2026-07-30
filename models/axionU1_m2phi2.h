#ifndef AXIONU1_M2PHI2_H  //Usual macro guard to prevent multiple inclusion
#define AXIONU1_M2PHI2_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */


#include "CosmoInterface/cosmointerface.h"

//Include cosmointerface to have access to all of the library.

namespace TempLat
{
    /////////
    // Model name and number of fields
    /////////

	 // @label:model_pars
	struct ModelPars : public TempLat::DefaultModelPars{
    	static constexpr size_t NScalars = 1;
    	static constexpr size_t NU1Flds = 1;
    	static constexpr size_t NPotTerms = 1;

    	using FloatType = double;

		// Axion-gauge coupling: index 0 scalar (the only one) couples to the index 0 abelian gauge (the only one)
		typedef TempLat::CouplingsManager<NScalars, NU1Flds,true> ScalarU1AxionCouplings; 

	};
	// @endlabel

	  // @label:model_name
	#define MODELNAME axionU1_m2phi2
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
        FloatType mass; //axion mass for the specific quadratic potential of this model file
      // @endlabel
		

      public:
        static constexpr size_t NDim = Model<MODELNAME>::NDim;

		// @label:constructor_params
    	MODELNAME(ParameterParser& parser, RunParameters<FloatType>& runPar, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox): //Constructor of our model.
    	Model<MODELNAME>(parser,runPar.getLatParams(), toolBox, runPar.dt, STRINGIFY(MODELLABEL)) //MODELLABEL is defined in the cmake.
    	{
		 // @endlabel
   

			// @label:initial_conditions
				fldS0 = parser.get<FloatType, 1>("initial_amplitudes");
				piS0 = parser.get<FloatType, 1>("initial_momenta");
			// @endlabel

      
		// @label:read_model_specific_param
		mass = parser.get<FloatType>("Mass");
		// @endlabel
		
		// @label:rescaling
        alpha = 0;
        fStar = fldS0(0_c);
        omegaStar = mass;
		// @endlabel

    
		// @label:masses_setup
        setInitialPotentialAndMassesFromPotential(); //not used for this model, but written here for CL to properly work in general
		 // @endlabel

        }

   	 // @label:potential_terms
        auto potentialTerms(Tag<0>) // Term 0: Quadratic potential of the scalar singlet
    	{
      		return  FloatType(0.5) * pow<2>(fldS(0_c));
    	}
		// @endlabel

		// @label:potential_derivs
        auto potDeriv(Tag<0>) // Derivative with respect to the scalar singlet
    	{
    		return fldS(0_c);
    	}
		 // @endlabel

		// @label:potential_second_derivs
    	auto potDeriv2(Tag<0>) // 2nd derivative with respect to the scalar singlet
    	{
      		return  OneType() ;
    	}
		// @endlabel

    };
}

#endif  //M2PHI2_AXIONU1_H
