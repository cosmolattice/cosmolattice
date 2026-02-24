#ifndef M2PHI2_AXIONC_H // Usual macro guard to prevent multiple inclusion
#define M2PHI2_AXIONC_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Nicolas Loayza, Ander Urio, Adrien Florio, Year: 2025

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
    static constexpr size_t NU1Flds = 1;
    static constexpr size_t NSU2Doublet = 0;
    static constexpr size_t NSU2Flds = 0;
    static constexpr size_t NPotTerms = 1;

    // Coupling managers:  they deal with the different couplings between the gauge fields and real(axionic)/complex
    // scalars/SU2 doublets
    //  --> If a type of interaction is not present, comment the corresponding line

    using ScalarU1AxionCouplings = TempLat::CouplingsManager<NScalars, NU1Flds, true>; // activates axion coupling U(1)-scalar

    // typedef TempLat::CouplingsManager<NSU2Doublet, NU1Flds, true> SU2DoubletU1Couplings; // activates coupling
    // U(1)-doublet
    //	typedef TempLat::CouplingsManager<NSU2Doublet, NSU2Flds,true> SU2DoubletSU2Couplings; // activates coupling
    // SU(2)-doublet
  };

#define MODELNAME U1Axion
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
    double m2, kappa, fluctuationRatio;
    // Here are the declaration of the model specific parameters. They are 'private'
    // to force you using them only within your model and not outside.

  public:
    static constexpr size_t NDim = Model<MODELNAME>::NDim;

    MODELNAME(ParameterParser &parser, RunParameters<double> &runPar, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox)
        : // Constructor of our model.
          Model<MODELNAME>(parser, runPar.getLatParams(), toolBox, runPar.dt,
                           STRINGIFY(MODELLABEL)) // MODELLABEL is defined in the cmake.
    {

      /////////
      // Initial homogeneous components of the fields
      // (read from parameters file, or specified here if not)
      /////////

      fldS0 = parser.get<double, 1>("initial_amplitudes");
      piS0 = parser.get<double, 1>("initial_momenta");

      /////////
      // Parameters of the model (read from parameters file)
      /////////
      // --> Comment: Gauge couplings are specified in the parameters file (e.g. gU1s, gSU2s), and do not need to be
      // defined here

      kappa = parser.get<double>("kappa");

      m2 = parser.get<double>("m2");

      /////////
      // Rescaling for program variables
      /////////

      alpha = 0;
      fStar = 1.0;
      omegaStar = 1.0;

      InverseAxionLambda = kappa * MPl; // * fStar/2.435e18;

      fluctuationRatio = parser.get<double>("fluctuationRatio", 1e-4);

      setInitialPotentialAndMassesFromPotential();
    }

    /////////
    // Program potential (add as many functions as terms are in the potential)
    /////////
    // --> Comment: These functions may depend on the norm of the complex scalar/doublets,
    //      which are introduced as "norm(fldCS(0_c))" and "norm(fldSU2Doublet(0_c))" respectively.

    auto potentialTerms(Tag<0>) // Term 0: Quartic potential of the complex scalar
    {
      return 0.5 * m2 * pow<2>(fldS(0_c));
    }

    std::string extraInfoFn(int verbosity)
    {
      auto extraInfo = Model<MODELNAME>::extraInfoFn(verbosity);
      if (verbosity > 0) {
        extraInfo += "kappa_" + FloatToString::format(kappa) + "_m2_" + FloatToString::format(m2) + "_";
      }
      return extraInfo;
    }

    /////////
    // Derivatives of the program potential with respect fields
    /////////

    auto potDeriv(Tag<0>) // Derivative with respect complex scalar norm
    {
      return m2 * fldS(0_c);
    }

    /////////
    //  Second derivatives of the program potential with respect fields
    /////////

    auto potDeriv2(Tag<0>) const // 2nd derivative with respect complex scalar norm
    {
      return m2;
    }

    auto getFluctuationRatio(FieldsNumbering::piS) const { return fluctuationRatio; }

    auto getFluctuationRatio(FieldsNumbering::fldS) const { return fluctuationRatio; }

    auto getFluctuationRatio(FieldsNumbering::fldU1) const { return fluctuationRatio; }

    auto getFluctuationRatio(FieldsNumbering::piU1) const { return fluctuationRatio; }
  };
} // namespace TempLat

#endif // AXION_H
