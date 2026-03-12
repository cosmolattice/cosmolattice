#ifndef ON_2D_H // Usual macro guard to prevent multiple inclusion
#define ON_2D_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/cosmointerface.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"

// Include cosmointerface to have access to all of the library.

namespace TempLat
{
  /////////
  // Model name and number of fields
  /////////

  // In the following class, we define the defining parameters of your model:
  // number of fields of each species and the type of tinteractions.

  struct ModelPars : public TempLat::DefaultModelPars {
    static constexpr size_t NScalars = 2;
    // In our phi4 example, we only want 2 scalar fields.
    static constexpr size_t NPotTerms = 1;
    // Our potential naturaly splits into two terms: the inflaton potential
    // and the interaction with the daughter field.
    static constexpr size_t NDim = 2;

    // All the numbers of fields are 0 by default, so we need only
    // to specify that we want two scalar fields.
    // See the model with gauge fields to have an example of how to turn
    // them on and specify interactions.
  };

#define MODELNAME ON_2D
  // Here we define the name of the model. This should match the name of your file.

  template <class R> using Model = MakeModel(R, ModelPars);
  // In this line, we define an appropriate generic model, with the correct
  // number of fields, ready to be customized.
  // If you are curious about what this is doing, the macro is defined in
  // the "CosmoInterface/abstractmodel.h" file.

  class MODELNAME : public Model<MODELNAME>
  // Declaration of our model. It inherits from the generic model defined above.
  {
    //...
  private:
    double m2, lambda;
    double A, omega;
    double highCut, lowCut;
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
    static constexpr size_t N = Model<MODELNAME>::Ns;
    using Model<MODELNAME>::t;

    Field<typename Model<MODELNAME>::FloatType, NDim> DriveProfile;

    MODELNAME(ParameterParser &parser, RunParameters<double> &runPar,
              device::memory::host_ptr<MemoryToolBox<NDim>> toolBox)
        : // Constructor of our model.
          Model<MODELNAME>(parser, runPar.getLatParams(), toolBox, runPar.dt,
                           STRINGIFY(MODELLABEL)) // MODELLABEL is defined in the cmake.
          ,
          DriveProfile("DriveProfile", toolBox, runPar.getLatParams())
    {
      /////////
      // Independent parameters of the model (read from parameters file)
      /////////

      lambda = parser.get<double>("lambda");
      //  We start by initializing our model paramteters. We read them from the
      // input file/command line.  Effectively, by calling 'par.get<double>("lambda")'
      // we declare a new parameter which needs to be in the input data.  Its name is
      // "lambda" and we specify it is a 'double'.

      m2 = parser.get<double>("m2");
      A = parser.get<double>("A");
      omega = parser.get<double>("omega");

      highCut = parser.get<double>("highCut");
      lowCut = parser.get<double>("lowCut");

      /////////
      // Initial homogeneous components of the fields
      // (read from parameters file, or specified here if not)
      /////////

      // We assume all scalar fields have identical initial amplitudes and momenta
      for (size_t i = 0; i < N; ++i) {
        fldS0[i] = parser.get<double>("initial_amplitudes");
        piS0[i] = parser.get<double>("initial_momenta", 0.);
      }

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
      fStar = fldS0[0];
      omegaStar = sqrt(m2) + sqrt(0.5 * lambda) * fStar;
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

      FourierSite<NDim> ntilde(toolBox);
      // Fourier lattice site, see eq.(57) of arXiv:2006.15122v2
      auto k = ntilde.norm() * runPar.kIR;
      DriveProfile.inFourierSpace() = heaviside(highCut - k) * heaviside(k - lowCut);
    }

    /////////
    // Program potential (add as many functions as terms are in the potential)
    /////////

    auto potentialTerms(Tag<0>) const // Potential energy of the fields
    //
    // Now we need to define the physics of the model. We start by defining the potential.
    // We need to specify as  many potential  terms as we specified in the ModelParams,
    // here 2. Then for every potential terms, we define a function
    //' auto potentialTerms(Tag<N>)'  with N =0,...,NPot -1. The type 'Tag<N>' simply allows
    // to define different function with the same name. The 'auto' keyword lets the compiler
    // figure out on itself what is the actual return type of the function.
    {
      return Total(j, 0, N - 1, m2 * pow<2>(fldS(j))) +
             Total(i, 0, N - 1, Total(j, 0, N - 1, 0.25 * lambda * pow<2>(fldS(i)) * pow<2>(fldS(j))));
      // Driving term is not in here, as the potential should reflect the true potential energy of the fields only.

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

    template <int D> auto potDeriv(Tag<D> tagD) // Derivative with respect to the D-th scalar field.
    // In exactly the same fashion, we  need to define one derivative of the potential
    // per scalar field (N in this case).  The integer in Tag<D> tells you the field with
    // respect to which you are defining the derivative of the potential of.
    {
      return m2 * fldS(tagD) + Total(j, 0, N - 1, 0.25 * lambda * 2 * fldS(tagD) * pow<2>(fldS(j))) +
             0.25 * lambda * 2 * fldS(tagD) * pow<2>(fldS(tagD)) // missing part of the phi^4 self-interaction (4) - 2
             + A * sin(omega * t) * DriveProfile;                // Add Driving here
    }

    /////////
    //  Second derivatives of the program potential with respect fields
    // (add one function for each field)
    /////////

    template <int D> auto potDeriv2(Tag<D> tagD) // Second derivative with respect D-th scalar field.
    // Finally, for the purpose of initializing the masses, the user needs to define
    // in the same fashion the second derivatives of the potential
    // (put 'return 0' if you are not using this feature).
    {
      return m2 + Total(j, 0, N - 1, 0.25 * lambda * 2 * pow<2>(fldS(j))) +
             0.25 * lambda * 10 * pow<2>(fldS(tagD)) // missing part of the phi^4 self-interaction (4 * 3) - 2
             + ZeroType();                           // Driving here
    }
  };
} // namespace TempLat

#endif // ON_2D_H
