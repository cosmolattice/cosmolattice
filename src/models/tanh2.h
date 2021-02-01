#ifndef TANH2_H //Usual macro guard to prevent multiple inclusion
#define TANH2_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/cosmointerface.h"

//Include cosmointerface to have access to all of the library.

namespace TempLat
{
    /////////
    // Model name and number of fields
    /////////

    // In the following class, we define the defining parameters of your model:
    //number of fields of each species and the type of interactions.

    struct ModelPars : public TempLat::DefaultModelPars {
        static constexpr size_t NScalars = 2;
        // In our phi4 example, we only want 2 scalar fields.
        static constexpr size_t NPotTerms = 2;
        // Our potential naturaly splits into two terms: the inflaton potential
        // and the interaction with the daughter field.

        // All the numbers of fields are 0 by default, so we need only
        // to specify that we want two scalar fields.
        // See the model with gauge fields to have an example of how to turn
        // them on and specify interactions.
    };

  #define MODELNAME tanh2
  // Here we define the name of the model. This should match the name of your file.

  template<class R>
  using Model = MakeModel(R, ModelPars);
  // In this line, we define an appropriate generic model, with the correct
  // number of fields, ready to be customized.
  // If you are curious about what this is doing, the macro is defined in
  // the "CosmoInterface/abstractmodel.h" file.

  class MODELNAME : public Model<MODELNAME>
  //Declaration of our model. It inherits from the generic model defined above.
  {
 //...
private:

	double M, Lambda4, phii, omega, q, g;
// Here are the declaration of the model specific parameters. They are 'private'
// to force you using them only within your model and not outside.

// Some parameters which are declared in the class "Model" and which are useful (they are all 'public'):

// fldS0, piS0 : arrays which should contain the initial homogeneous values of
//               the scalar fields
//
// alpha, fStar, omegaStar : time and field rescaling to go to program units.
//
// fldS : The actual object which contains the scalar fields.
  
  public:

    MODELNAME(ParameterParser& parser, RunParameters<double>& runPar, std::shared_ptr<MemoryToolBox> toolBox): //Constructor of our model.
    Model<MODELNAME>(parser,runPar.getLatParams(), toolBox, runPar.dt, STRINGIFY(MODELLABEL)) //MODELLABEL is defined in the cmake.
    {
      /////////
      // Independent parameters of the model and initial homogeneous components of the fields
      /////////
      
        M = parser.get<double>("M");
        Lambda4 = parser.get<double>("Lambda4");
        q = parser.get<double>("q");
        
        fldS0 = parser.get<double, 2>("initial_amplitudes");
        piS0 = parser.get<double, 2>("initial_momenta", {0, 0});
        
        phii = fldS0[0];
        g = sqrt(q)*omega/phii;
        omega = sqrt(Lambda4  / pow<2>(M)); 

        
        /////////
        // Rescaling for program variables
        /////////
        
         alpha = 0;
   		 fStar = fldS0[0];
   		 omegaStar = omega;
   		 
   		 // We now need to specify the rescaling from physical units to program units.
        // This consists of the  time rescaling exponent alpha, the field rescaling fStar
        // and the velocity rescaling omegaStar.
        // See the paper for more information on how to fix them.
        

   		 setInitialPotentialAndMassesFromPotential();
   		 // Here we call this function to compute the value of the potential on the homogeneous
        // initial condition  (useful to set the initial Hubble rate). We also compute
        // in this function the masses from the second derivative of the potential
        // evaluated on the homogeneous initial conditions. If you want to do something else,
        // uncomment the next section and do whatever suits your needs.

        /*
          masses2S = {..., ...};
          setInitialPotentialFromPotential();
         */ 
   		
   	}
   		 
   /////////
   // Program potential (add as many functions as terms are in the potential)
   /////////  
   
    auto potentialTerms(Tag<0>) // Inflaton potential energy
    {
      	return pow<2>(M/phii) * pow<2>(tanh(fldS(0_c)*phii/M)) / 2;
    }
    auto potentialTerms(Tag<1>) // Interaction energy
    {
        return 0.5 * q * fldS(0_c) * fldS(0_c) * fldS(1_c) * fldS(1_c);
    }
    
    
   /////////
   // Derivatives of the program potential with respect fields (add one function for each field)
   ///////// 

    auto potDeriv(Tag<0> f0) // Derivative with respect inflaton
    {
        return   2 * (M/phii) * pow<2>(tanh(fldS(0_c)*phii/M))/sinh(2*fldS(0_c)*phii/M) + q * fldS(0_c) * fldS(1_c) * fldS(1_c);
    }

    auto potDeriv(Tag<1> f1)  // Derivative with respect daughter field
    {
        return  q * fldS(1_c) * fldS(0_c) * fldS(0_c);
    }
    
   /////////
   //  Second derivatives of the program potential with respect fields (add one function for each field)
   /////////

    auto potDeriv2(Tag<0> f0) // Second derivative with respect inflaton
    {
        return  4  * (2 - cosh(2*fldS(0_c)*phii/M) ) * pow<2>(tanh(fldS(0_c)*phii/M)) / pow<2>(sinh(2*fldS(0_c)*phii/M)) +  q * fldS(1_c) * fldS(1_c) ;
    }

    auto potDeriv2(Tag<1> f1) // Second derivative with respect daughter field
    {
        return  q * fldS(0_c) * fldS(0_c) ;
    }

    };
}

#endif //TANH2_H


