#ifndef DEFECTSONGLOBAL_H // Usual macro guard to prevent multiple inclusion
#define DEFECTSONGLOBAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros,  Year: 2026

#include "CosmoInterface/cosmointerface.h"

// Include cosmointerface to have access to all of the library.

namespace TempLat
{
  /////////
  // Model name and number of fields
  /////////

  // In the following class, we define the defining parameters of your model:
  // number of fields of each species and the type of interactions.

  // @label:model_pars
  struct ModelPars : public TempLat::DefaultModelPars {
    static constexpr size_t NScalars = 1;
    static constexpr size_t NPotTerms = 1;
    static constexpr bool DefectsModel = true;

    using FloatType = double;
  };
  // @endlabel

#define MODELNAME defects_ONGlobal
  // Here we define the name of the model. This should match the name of your file.

  template <class R> using Model = MakeModel(R, ModelPars);
  // In this line, we define an appropriate generic model, with the correct
  // number of fields, ready to be customized.
  // If you are curious about what this is doing, the macro is defined in
  // the "CosmoInterface/abstractmodel.h" file.

  class MODELNAME : public Model<MODELNAME>
  // Declaration of our model. It inherits from the generic model defined above.
  {
  private:
    FloatType lambda, vev, q;
    // Here are the declaration of the model specific parameters. They are 'private'
    // to force you using them only within your model and not outside.

  public:
    FloatType g;
    static constexpr size_t NDim = Model<MODELNAME>::NDim;

    MODELNAME(ParameterParser &parser, RunParameters<FloatType> &runPar,
              device::memory::host_ptr<MemoryToolBox<NDim>> toolBox)
        : // Constructor of our model.
          Model<MODELNAME>(parser, runPar.getLatParams(), toolBox, runPar.dt,
                           STRINGIFY(MODELLABEL)) // MODELLABEL is defined in the cmake.
    {
      /////////
      // Initial homogeneous components of the fields
      // (read from parameters file, or specified here if not)
      /////////

      // COMPLEX SCALAR NORM: set to zero to have a non-biased model
      ForLoop(i, 0, ModelPars::NScalars - 1, fldS0(i) = FloatType(0); piS0(i) = FloatType(0););
      /////////
      // Parameters of the model (read from parameters file)
      /////////
      // --> Comment: Gauge couplings are specified in the parameters file (e.g. gU1s, gSU2s), and do not need to be
      // defined here

      lambda = parser.get<FloatType>("lambda", 1.);
      vev = parser.get<FloatType>("vev", 1.);
      q = parser.get<FloatType>("qbias", 0.);
      if (!AlmostEqual(q, 0.0, runParameterTolerance) && ModelPars::NScalars != 1)
        throw(RunParametersInconsistent(
            "The bias potential is only implemented for domain wall simulations. Please, set qbias to zero or remove "
            "it from the parameter file to run a simulation with NScalars > 1."));

      /////////
      // Rescaling for program variables
      /////////

      // @label:program_variables
      alpha = 1;
      fStar = vev;
      omegaStar = sqrt(lambda) * vev;
      // @endlabel
      g = q / lambda;
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
    }

    // @label:model_potential
    /////////
    // Program potential (add as many functions as terms are in the potential)
    /////////
    // --> Comment: These functions may depend on the norm of the complex scalar/doublets,
    //      which are introduced as "norm(fldCS(0_c))" and "norm(fldSU2Doublet(0_c))" respectively.
    auto potentialTerms(Tag<0>) // Term 0: Quartic potential of the complex scalar
    {
      return resolutionPreservingFactor * FloatType(0.25) *
                 pow<2>(Total(i, 0, ModelPars::NScalars - 1, pow<2>(fldS(i))) - FloatType(1.)) +
             IfElse(ModelPars::NScalars == 1, g * pow<3>(fldS(0_c)), ZeroType());
    }

    /////////
    // Derivatives of the program potential with respect fields
    /////////
    template <int M> auto potDeriv(Tag<M> m) // Derivative with respect complex scalar norm
    {
      return resolutionPreservingFactor * fldS(m) *
                 (Total(i, 0, ModelPars::NScalars - 1, pow<2>(fldS(i))) - FloatType(1.)) +
             IfElse(ModelPars::NScalars == 1, 3 * g * pow<2>(fldS(0_c)), ZeroType());
    }
    // @endlabel

    /////////
    //  Second derivatives of the program potential with respect fields
    /////////
    template <int M> auto potDeriv2(Tag<M> m) // 2nd derivative with respect complex scalar norm
    {
      return resolutionPreservingFactor *
                 (Total(i, 0, ModelPars::NScalars - 1, pow<2>(fldS(i))) + FloatType(2.) * pow<2>(fldS(m))) +
             IfElse(ModelPars::NScalars == 1, 6 * g * fldS(0_c), ZeroType());
    }
  };
} // namespace TempLat

#endif // LPHI4U1_H
