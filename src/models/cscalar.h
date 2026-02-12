#ifndef CSCALAR_H // Usual macro guard to prevent multiple inclusion
#define CSCALAR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel
   Valkenburg. Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco
// Torrenti,  Year: 2020

#include "CosmoInterface/cosmointerface.h"

// Include cosmointerface to have access to all of the library.

namespace TempLat
{
  /////////
  // Model name and number of fields
  /////////

  struct ModelPars : public TempLat::DefaultModelPars {
    static constexpr size_t NCScalars = 1;
    static constexpr size_t NPotTerms = 1;
  };

#define MODELNAME cscalar
  // Here we define the name of the model. This should match the name of your
  // file.

  template <class R> using Model = MakeModel(R, ModelPars);
  // In this line, we define an appropriate generic model, with the correct
  // number of fields, ready to be customized.
  // If you are curious about what this is doing, the macro is defined in
  // the "CosmoInterface/abstractmodel.h" file.

  class MODELNAME : public Model<MODELNAME>
  {
  private:
    double m2;
    // Here are the declaration of the model specific parameters. They are
    // 'private' to force you using them only within your model and not outside.

    // Some parameters which are declared in the class "Model" and which are
    // useful (they are all 'public'):

    // fldS0, piS0 : arrays which should contain the initial homogeneous values of
    //               the scalar fields
    //
    // alpha, fStar, omegaStar : time and field rescaling to go to program units.
    //
    // fldS : The actual object which contains the scalar fields.

  public:
    static constexpr size_t NDim = Model<MODELNAME>::NDim;

    MODELNAME(ParameterParser &parser, RunParameters<double> &runPar, auto toolBox)
        : Model<MODELNAME>(parser, runPar.getLatParams(), toolBox, runPar.dt,
                           STRINGIFY(MODELLABEL)) // MODELLABEL is defined in the cmake.
    {
      /////////
      // Independent parameters of the model (read from parameters file)
      /////////

      m2 = parser.get<double>("m2");

      const double normCmplx0 = parser.get<double>("initial_amplitudes");
      const double normPiCmplx0 = parser.get<double>("initial_momenta", 0);

      fldCS0(0_c) = Complexify(normCmplx0 / sqrt(2.0), normCmplx0 / sqrt(2.0));
      piCS0(0_c) = Complexify(normPiCmplx0 / sqrt(2.0), normPiCmplx0 / sqrt(2.0));

      /////////
      // Rescaling for program variables
      /////////

      alpha = 1.;
      fStar = 1.;
      omegaStar = 1.;

      setInitialPotentialAndMassesFromPotential();
    }

    /////////
    // Program potential (add as many functions as terms are in the potential)
    /////////

    auto potentialTerms(Tag<0>) const // Inflaton potential energy
    {
      return 0.5 * m2 * norm2(fldCS(0_c));
    }
    auto potDerivNormCS(Tag<0>) const { return m2 * norm(fldCS(0_c)); }
    auto potDeriv2NormCS(Tag<0>) const { return m2; }
  };
} // namespace TempLat

#endif // CSCALAR_H
