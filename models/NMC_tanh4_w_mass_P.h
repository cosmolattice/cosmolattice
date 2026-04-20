#ifndef NMCTANH4_w_mass_H //Usual macro guard to prevent multiple inclusion
#define NMCTANH4_w_mass_H

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
    // number of fields of each species and the type of tinteractions.

    struct ModelPars : public TempLat::DefaultModelPars {
        static constexpr size_t NScalars = 2;
        // In this example, we only want 2 scalar fields.
        static constexpr size_t NPotTerms = 2;
        // Only the inflaton has a potential



        // All the numbers of fields are 0 by default, so we need only
        // to specify that we want two scalar fields.
        // See the model with gauge fields to have an example of how to turn
        // them on and specify interactions.

        typedef CouplingsManager<NScalars, 1, false, true> NonMinimalCouplings;   // Non-minimal coupling to gravity of scalars, only the second scalar.


    };

  #define MODELNAME NMC_tanh4_w_mass_P
  // Here we define the name of the model. This should match the name of your file.

  template<class R>
  using Model = MakeModel(R, ModelPars);
  // In this line, we define an appropriate generic model, with the correct
  // number of fields, ready to be customized.
  // If you are curious about what this is doing, the macro is defined in
  // the "CosmoInterface/abstractmodel.h" file.

  class MODELNAME : public Model<MODELNAME>
  // Declaration of our model. It inherits from the generic model defined above.
  {
 //...
private:

  double lambdaNMC, mNMC, Hstar, V0, M;
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
    static constexpr size_t NDim = Model<MODELNAME>::NDim;

    MODELNAME(ParameterParser& parser, RunParameters<double>& runPar,
              device::memory::host_ptr<MemoryToolBox<NDim>> toolBox)
        : Model<MODELNAME>(parser, runPar.getLatParams(), toolBox, runPar.dt, STRINGIFY(MODELLABEL)) //MODELLABEL is defined in the cmake.
    {
      /////////
      // Independent parameters of the model (read from parameters file)
      /////////
      Hstar = parser.get<double>("Hstar"); //Hubble rate 14 efolds before the end of inflation, begin of linear analysis in GeV
	  V0 = parser.get<double>("V0"); //Lambda4/(4 * mp^2 * Hstar^2)
	  M = parser.get<double>("M"); //M in units of mp
      lambdaNMC = parser.get<double>("lambdaNMC"); //Quartic Coupling of the NMC field
	  mNMC = parser.get<double>("mNMC"); //Mass of the NMC field in units of Hstar.
	  
      //  We start by initializing our model paramteters. We read them from the
      // input file/command line.  Effectively, by calling 'par.get<double>("lambda")'
      // we declare a new parameter which needs to be in the input data.  Its name is
      // "lambda" and we specify it is a 'double'.

        /////////
        // Initial homogeneous components of the fields
        // (read from parameters file, or specified here if not)
        /////////

        fldS0 = parser.get<double, 2>("initial_amplitudes");
        piS0 = parser.get<double, 2>("initial_momenta", {0, 0});
        // Then, we need to specify the initial homogeneous
        // value of our fields. We read them again from the input file. The int '2' means
        // that we actually expect two values and that we will get an array of
        // double of size two.
        // Contrary to the "initial_amplitudes" parameter and the others above,
        //, the "initial_momenta" is an optional parameter. It can still be specified through
        //  command line or input file as initial_momenta=value1 value2 ... valueNs,
        // but it can also be omitted, as we specified a default value of '{0, 0}'.


        /////////
        // Rescaling for program variables
        /////////
        alpha = 1;
        fStar = Constants::MPl;
        omegaStar = Hstar;
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
    //
    // Now we need to define the physics of the model. We start by defining the potential.
    // We need to specify as  many potential  terms as we specified in the ModelParams,
    // here 2. Then for every potential terms, we define a function
    //' auto potentialTerms(Tag<N>)'  with N =0,...,NPot -1. The type 'Tag<N>' simply allows
    // to define different function with the same name. The 'auto' keyword lets the compiler
    // figure out on itself what is the actual return type of the function.
    {
        return V0 * pow<4>(tanh(fldS(0_c) / M));
		//return 0.0;
		//return 0.25 * pow<4>(fldS(0_c));
        // Some notations.  The scalar fields are stored in a collection called 'fldS'.
        // The scalar fields are labelled  from 0 to Ns-1. The field say number 1 is
        // accessed through the syntax 'fldS(0_c)'. The function 'pow<N>(x)'. Works with the
        // known-at-compile-time integer N and compute the expression x*...*x N times.
        // If you don't know the integer at compile time or you don't have an integer,
        // use the more usual syntax pow(x, N).
        // These 'pow' functions are just one example of the many algebraic functions which
        // can be applied to our fields,  see the manual for an exhaustive list
        // and what to do if you want to implement a new one.
    }
	
    auto potentialTerms(Tag<1>) // NMC field potential
    {
        return 0.5 * pow<2>(mNMC) * pow<2>(fldS(1_c)) + 0.25 * pow<2>(Constants::MPl/Hstar) * lambdaNMC * pow<4>(fldS(1_c));
    }


    // Advanced note (ignore if you are satisfied with the above) :
    // - The 'auto' return type is important because the object returned is
    // not say an array containing  the value of the expression but the expression itself, which can and will be
    // evaluate later on. The type of the  expression itself depends on the expression and can be intricated. See
    // manual for more  details.
    // - The syntax 0_c is equivalent to Tag<0>(),
    // i.e. creating  an object of type 0. This operator '_c' is a modern C++ user-defined type literal,
    // taken from Boost and located in fcn/util/rangeiteration/tagliteral.h .



   /////////
   // Derivatives of the program potential with respect fields
   // (add one function for each field).
   /////////

    auto potDeriv(Tag<0>) // Derivative with respect to the inflaton.
    // In exactly the same fashion, we  need to define one derivative of the potential
    // per scalar field (2 in this case).  The integer in Tag<0> tells you the field with
    // respect to which you are defining the derivative of the potential of.
    {
		return   V0 * 4.0 * (1./M) * pow<3>(tanh(fldS(0_c)/M)) / pow<2>(cosh(fldS(0_c)/M));
	  //return pow<3>(fldS(0_c));
    }
	
    auto potDeriv(Tag<1>) // NMC field potential 1st deriv
    {
        return pow<2>(mNMC) * fldS(1_c) + pow<2>(Constants::MPl/Hstar) * lambdaNMC * pow<3>(fldS(1_c));
    }

    /////////
   //  Second derivatives of the program potential with respect fields
   // (add one function for each field)
   /////////

    auto potDeriv2(Tag<0>) // Second derivative with respect inflaton
    // Finally, for the purpose of initializing the masses, the user needs to define
    // in the same fashion the second derivatives of the potential
    // (put 'return 0' if you are not using this feature).
    {
	  return  V0 * 4.0 * pow<2>(1./M) * (4.0-cosh(2.0*fldS(0_c)/M)) * pow<2>(tanh(fldS(0_c)/M)) / pow<4>(cosh(fldS(0_c)/M));
	  //return 3.0 * pow<2>(fldS(0_c));
    }
	
    auto potDeriv2(Tag<1>) // NMC field potential 2nd deriv
    {
        return pow<2>(mNMC) + 3.0 * pow<2>(Constants::MPl/Hstar) * lambdaNMC * pow<2>(fldS(1_c));
    }

    };
}

#endif //NMCTANH4_w_mass_H

