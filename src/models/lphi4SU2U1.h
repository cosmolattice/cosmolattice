#ifndef LPHI4SU2U1_H  //Usual macro guard to prevent multiple inclusion
#define LPHI4SU2U1_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2020

#include "CosmoInterface/cosmointerface.h"

//Include cosmointerface to have access to all of the library.

namespace TempLat
{
/////////
// Model name and number of fields
/////////

// In the following class, we define the defining parameters of your model:
// number of fields of each species and the type of interactions.

    struct ModelPars : public TempLat::DefaultModelPars{
        static constexpr size_t NScalars = 1;
        static constexpr size_t NCScalars = 1;
        static constexpr size_t NU1Flds = 1;
        static constexpr size_t NSU2Doublet = 1;
        static constexpr size_t NSU2Flds = 1;
        static constexpr size_t NPotTerms = 3;

        // Coupling managers:  they deal with the different couplings between
        // the gauge fields and complex scalars/SU2 doublets
        //  --> If a type of interaction is not present, comment the corresponding line
        typedef TempLat::CouplingsManager<NCScalars, NU1Flds,true> CsU1Couplings;
        // activates coupling U(1)-complex scalar
        typedef TempLat::CouplingsManager<NSU2Doublet, NU1Flds, true> SU2DoubletU1Couplings;
        // activates coupling U(1)-doublet
        typedef TempLat::CouplingsManager<NSU2Doublet, NSU2Flds,true> SU2DoubletSU2Couplings;
        // activates coupling SU(2)-doublet
    };

#define MODELNAME lphi4SU2U1
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
    private:
        double g, h, lambda, qG, qH;
        // Here are the declaration of the model specific parameters. They are 'private'
        // to force you using them only within your model and not outside.

    public:

        MODELNAME(ParameterParser& parser, RunParameters<double>& runPar, std::shared_ptr<MemoryToolBox> toolBox): //Constructor of our model.
                Model<MODELNAME>(parser,runPar.getLatParams(), toolBox, runPar.dt, STRINGIFY(MODELLABEL)) //MODELLABEL is defined in the cmake.
        {

            /////////
            // Initial homogeneous components of the fields
            // (read from parameters file, or specified here if not)
            /////////

            // SCALAR SINGLET: initial homogeneous amplitude and derivative
            fldS0(0_c) = parser.get<double>("initial_amplitudes");
            piS0(0_c) = parser.get<double>("initial_momenta");

            // COMPLEX SCALAR NORM: initial homogeneous amplitude and derivative
            double normCmplx0 = parser.get<double>("cmplx_field_initial_norm");
            double normPiCmplx0 = parser.get<double>("cmplx_momentum_initial_norm");

            // We distribute the norm equally between the two components
            // using the "Complexify" function
            fldCS0(0_c) = Complexify(normCmplx0 / sqrt(2.0), normCmplx0 / sqrt(2.0));
            piCS0(0_c)  = Complexify(normPiCmplx0 / sqrt(2.0), normPiCmplx0 / sqrt(2.0));

            // SU(2) COMPLEX NORM: initial homogeneous amplitude and derivative
            double normDoublet0 = parser.get<double>("SU2Doublet_initial_norm");
            double normPiDoublet0 = parser.get<double>("SU2Doublet_initial_momenta_norm");

            // We distribute the norm equally between the four components
            // using the "MakeSU2Doublet" function
            fldSU2Doublet0(0_c) = MakeSU2Doublet(a,normDoublet0 / 2);
            piSU2Doublet0(0_c)  = MakeSU2Doublet(a,normPiDoublet0 / 2);

            /////////
            // Parameters of the model (read from parameters file)
            /////////
            // --> Comment: Gauge couplings are specified in the parameters file (e.g. gU1s, gSU2s), and do not need to be defined here

            qG = parser.get<double>("qG");
            qH = parser.get<double>("qH");
            lambda = parser.get<double>("lambda");

            g = sqrt(qG*lambda);
            h = sqrt(qH*lambda);

            /////////
            // Rescaling for program variables
            /////////

            alpha = 1;
            fStar = normDoublet0;
            omegaStar = sqrt(lambda) * normDoublet0;
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
        //     which are introduced as respectively "norm(fldCS(0_c))"
        //     and "norm(fldSU2Doublet(0_c))".

        auto potentialTerms(Tag<0>)
        // Term 0: Quartic potential of the SU(2) doublet
        {
            return  pow<4>(norm(fldSU2Doublet(0_c)));
        }

        auto potentialTerms(Tag<1>)
        // Term 1: Interaction between SU(2) doublet and scalar singlet
        {
            return   qG * pow<2>(norm(fldSU2Doublet(0_c)) * fldS(0_c));
        }

        auto potentialTerms(Tag<2>)
        // Term 2: Interaction between SU(2) doublet and complex scalar
        {
            return  2 * qH * pow<2>(norm(fldSU2Doublet(0_c)) * norm(fldCS(0_c)));
        }

        /////////
        // Derivatives of the program potential with respect fields
        /////////

        auto potDeriv(Tag<0>)
        // Derivative with respect scalar singlet
        {
            return 2 * qG * pow<2>(norm(fldSU2Doublet(0_c))) * fldS(0_c);
        }

        auto potDerivNormCS(Tag<0>)
        // Derivative with respect complex scalar norm
        {
            return 4 * qH * pow<2>(norm(fldSU2Doublet(0_c))) * norm(fldCS(0_c));
        }

        auto potDerivNormSU2Doublet(Tag<0>)
        // Derivative with respect SU(2) doublet norm
        {
            return 4 * pow<3>(norm(fldSU2Doublet(0_c))) + norm(fldSU2Doublet(0_c)) * ( 2 * qG * pow<2>(fldS(0_c)) + 4 * qH * pow<2>(norm(fldCS(0_c))) );
        }

        /////////
        //  Second derivatives of the program potential with respect fields
        /////////

        auto potDeriv2(Tag<0>)
        // 2nd derivative with respect scalar singlet
        {
            return  2 * qG * pow<2>(norm(fldSU2Doublet(0_c)));
        }

        auto potDeriv2NormCS(Tag<0>)
        // 2nd derivative with respect complex scalar norm
        {
            return  4 * qH * pow<2>(norm(fldSU2Doublet(0_c)));
        }

        auto potDeriv2NormSU2Doublet(Tag<0>)
        // 2nd derivative with respect SU(2) doublet norm
        {
            return 12 * pow<2>(norm(fldSU2Doublet(0_c))) + 2 * qG * pow<2>(fldS(0_c)) + 4 * qH * pow<2>(norm(fldCS(0_c))) ;
        }


    };
}

#endif  //LPHI4_H
