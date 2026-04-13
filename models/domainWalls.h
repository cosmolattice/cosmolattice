#ifndef DOMAINWALLS_H // Usual macro guard to prevent multiple inclusion
#define DOMAINWALLS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/cosmointerface.h"

// Include cosmointerface to have access to all of the library.

namespace TempLat
{
  /////////
  // Model name and number of fields
  /////////

  // In the following class, we define the defining parameters of your model:
  // number of fields of each species and the type of interactions.

  struct ModelPars : public TempLat::DefaultModelPars {
    static constexpr size_t NScalars = 1;
    static constexpr size_t NCScalars = 0;
    static constexpr size_t NU1Flds = 0;
    static constexpr size_t NSU2Doublet = 0;
    static constexpr size_t NSU2Flds = 0;
    static constexpr size_t NPotTerms = 1;

    using NumberType = double;

    // Coupling managers:  they deal with the different couplings between the gauge fields and complex scalars/SU2
    // doublets
    //  --> If a type of interaction is not present, comment the corresponding line
    // typedef TempLat::CouplingsManager<NCScalars, NU1Flds, true> CsU1Couplings; // activates coupling U(1)-complex scalar
    // typedef TempLat::CouplingsManager<NSU2Doublet, NU1Flds, true> SU2DoubletU1Couplings;// activates coupling U(1)-doublet
    // typedef TempLat::CouplingsManager<NSU2Doublet, NSU2Flds,true> SU2DoubletSU2Couplings; // activates coupling SU(2)-doublet
  };

#define MODELNAME domainWalls
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
    FloatType lambda, vev, q, g;
    // Here are the declaration of the model specific parameters. They are 'private'
    // to force you using them only within your model and not outside.

  public:
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
      fldCS0(0_c) = Complexify(0., 0.);
      piCS0(0_c) = Complexify(0., 0.);

      /////////
      // Parameters of the model (read from parameters file)
      /////////
      // --> Comment: Gauge couplings are specified in the parameters file (e.g. gU1s, gSU2s), and do not need to be
      // defined here

      lambda = parser.get<FloatType>("lambda", 1.);
      vev = parser.get<FloatType>("lambda", 1.);
      q = parser.get<FloatType>("qbias", 0.);

      /////////
      // Rescaling for program variables
      /////////

      alpha = 1;
      fStar = vev;
      omegaStar = sqrt(lambda) * vev;
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

    /////////
    // Program potential (add as many functions as terms are in the potential)
    /////////
    // --> Comment: These functions may depend on the norm of the complex scalar/doublets,
    //      which are introduced as "norm(fldCS(0_c))" and "norm(fldSU2Doublet(0_c))" respectively.

    auto potentialTerms(Tag<0>) // Term 0: Quartic potential of the complex scalar
    {
      return (*this).fatteningfactor * FloatType(0.25) * pow<2>(pow<2>(fldS(0_c)) - FloatType(1.)) + g * pow<3>(fldS(0_c))   ;
    }

    /////////
    // Derivatives of the program potential with respect fields
    /////////

    auto potDerivNormCS(Tag<0>) // Derivative with respect complex scalar norm
    {
      return (*this).fatteningfactor * fldS(0_c) * (pow<2>(fldS(0_c)) - FloatType(1.)) + FloatType(3.) * g * pow<2>(fldS(0_c))   ;
    }

    /////////
    //  Second derivatives of the program potential with respect fields
    /////////

    auto potDeriv2NormCS(Tag<0>) // 2nd derivative with respect complex scalar norm
    {
      return (*this).fatteningfactor * (FloatType(3.) *  pow<2>(fldS(0_c)) - FloatType(1.)) + FloatType(6.) * g * fldS(0_c)   ;
    }
  };
} // namespace TempLat

#endif // LPHI4U1_H
