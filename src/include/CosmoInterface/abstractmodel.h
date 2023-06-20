#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019


#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/lattice/algebra/su2algebra/su2algebra.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "TempLat/lattice/algebra/gaugealgebra/forwardcovariantderivative.h"
#include "TempLat/util/exception.h"
#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/util/templatvector.h"
#include "TempLat/util/templatarray.h"
#include "CosmoInterface/couplingsmanager.h"
#include "CosmoInterface/definitions/potential.h"

#include "TempLat/lattice/algebra/helpers/getvalue.h"

#include "TempLat/parameters/parameterparser.h"

namespace TempLat {


    /** \brief A class which contains everything a model should have; models derive from here.
     *
     *
     **/

	// Exceptions that kill the program when some conditions are met (used below)
     MakeException(PotentialDerivativeNotDefined);
     MakeException(EmptyModel);
     MakeException(NotTested);

    struct DefaultModelPars {

    	// Number of fields of each species (set to zero by default)
        static constexpr size_t NScalars = 0; // scalar singlets
        static constexpr size_t NCScalars = 0; // complex scalars
        static constexpr size_t NU1Flds = 0; // U(1) gauge fields
        static constexpr size_t NSU2Doublet = 0; // SU(2) doublets
        static constexpr size_t NSU2Flds = 0; // SU(2) gauge fields

        // Number of terms in the potential. It can be changed in the model file.
        static constexpr size_t NPotTerms = 1;

        // Number of dimensions (1,2,3). It can be changed in the model file.
        static constexpr size_t NDim = 3;

		// Coupling managers, they deal with the possible couplings between the gauge fields and complex scalars/SU2 doublets
        typedef CouplingsManager<NCScalars, NU1Flds> CsU1Couplings;   // couplings U(1) gauge-complex scalar
        typedef CouplingsManager<NSU2Doublet, NU1Flds> SU2DoubletU1Couplings;   // couplings U(1) gauge-SU2 doublet
        typedef CouplingsManager<NSU2Doublet, NSU2Flds> SU2DoubletSU2Couplings;   // couplings SU(2) gauge-SU2 doublet

    };

    // In order to make some of the expression template mechanism works, the number of fields needs to be known
    // at compile time. Also, to take advantage of the possible algebraic simplification 0*a=0 and to implement
    // couplings to generic number of fields (not fully available in v 1.0), one needs to know at compile time what couples
    // to what. All these parameters are passed as template argument to the model. In order to simplify the syntax,
    // the following macros allow the user to define a model simply by specifying a user-defined class, containing the appropriate variable.

#define MakeAbstractModelTemplateArgs(_ModelName, _ModelParsType, _FloatType) _ModelName, _ModelParsType::NPotTerms, _ModelParsType::NScalars,_ModelParsType::NCScalars,_ModelParsType::NU1Flds, _ModelParsType::NSU2Doublet,_ModelParsType::NSU2Flds,typename _ModelParsType::CsU1Couplings, typename _ModelParsType::SU2DoubletU1Couplings,typename _ModelParsType::SU2DoubletSU2Couplings,_FloatType,_ModelParsType::NDim
#define MakeModelFloatType(_ModelName, _ModelParsType, _FloatType) AbstractModel<MakeAbstractModelTemplateArgs(_ModelName, _ModelParsType, _FloatType)>
#define MakeModel(_ModelName, _ModelParsType) AbstractModel<MakeAbstractModelTemplateArgs(_ModelName, _ModelParsType, double)>

    // Mother of all the models. The arguments are passed as template parameters.
    template<class R, size_t NPOTTERMS ,size_t NS, size_t NC, size_t  NU1FLDS, size_t NSU2DOUBLET, size_t NSU2FLDS, typename CSU1COUPLINGS, typename SU2DOUBLETU1COUPLINGS, typename SU2DOUBLETSU2COUPLINGS,  typename T = double, int NDIM = 3>
    class AbstractModel {
    public:
        // We store all the arguments passed as template arguments into class variable. That way they can easily be
        // accessed throughout the program, by whichever class which inherits from AbstractModel.
        static constexpr size_t Ns = NS;
        static constexpr size_t NCs = NC;
        static constexpr size_t NPotTerms = NPOTTERMS;
        static constexpr size_t NSU2Doublet = NSU2DOUBLET;
        static constexpr size_t NU1 = NU1FLDS;
        static constexpr size_t NSU2 = NSU2FLDS;
        static constexpr size_t NDim = NDIM;
        static constexpr size_t NGWs = 6;
        static constexpr T MPl = Constants::reducedMPlanck<T>;  // Reduced Planck mass, MPl=2.435*10^18 GeV


		// Coupling managers between complex scalar/SU2 doublets and gauge fields
        using  CsU1Couplings = CSU1COUPLINGS;
        using  SU2DoubletU1Couplings = SU2DOUBLETU1COUPLINGS;
        using  SU2DoubletSU2Couplings = SU2DOUBLETSU2COUPLINGS;
        using FloatType = T;


        // Objects containing the fields and their momenta:

        //Kernels RK




        // --> Scalar singlets
        FieldCollection<Field, T, Ns,true> fldS; //The last parameter is to "vectorise" the assignement. Can use it with scalar algebra (meaning scalar fields and U(1) non-compact gauge fields), but not with the rest.
    	FieldCollection<Field, T, Ns,true> piS;
        // Does not make a huge difference anyhow, so in case of doubt put nothing or false (equivalent).

        // --> Complex scalars
        FieldCollection<ComplexField,T,NCs> fldCS;
        FieldCollection<ComplexField,T,NCs> piCS;

        // --> SU2 doublets
        FieldCollection<SU2Doublet, T , NSU2Doublet> fldSU2Doublet;
        FieldCollection<SU2Doublet, T , NSU2Doublet> piSU2Doublet;

		// --> U(1) gauge fields
        VectorFieldCollection<Field, T, NDIM, NU1> fldU1;
        VectorFieldCollection<Field, T, NDIM, NU1> piU1;

        // --> SU(2) gauge fields
        VectorFieldCollection<SU2Field, T, NDIM, NSU2> fldSU2;
        VectorFieldCollection<SU2LieAlgebraField, T, NDim, NSU2> piSU2;


        // Variables that store the scale factor and energies during the evolution.
        // Suffixes indicate at which time they are evaluated:
        // Semi Integer Minus (SIM) at t-dt/2, Integer at (t), Semi Integer at (t+dt/2)

        // --> Averages scalar
        T grad2AvI, grad2AvSI;
        T pi2AvSI, pi2AvSIM, pi2AvIM, pi2AvI;

        // --> Averages SU2Doublet
        T CSgrad2AvI, CSgrad2AvSI;
        T CSpi2AvSI, CSpi2AvSIM, CSpi2AvIM, CSpi2AvI;

        // --> Averages CS
        T SU2DblGrad2AvI, SU2DblGrad2AvSI;
        T SU2DblPi2AvSI, SU2DblPi2AvSIM, SU2DblPi2AvIM, SU2DblPi2AvI;

        // --> Averages U1
        T U1Mag2AvI, U1Mag2AvSI;
        T U1pi2AvSI, U1pi2AvSIM, U1pi2AvIM, U1pi2AvI;

        // --> Averages SU2
        T SU2Mag2AvI, SU2Mag2AvSI;
        T SU2pi2AvSI, SU2pi2AvSIM, SU2pi2AvIM, SU2pi2AvI;

        // --> Averages potential
        T potAvI, potAvSI;

        // --> Scale factor (and time-derivative)
        T aSI, aI, aIM;
        T aDotSI, aDotSIM, aDotI, aDotIM;


        //Initial Amplitudes (homogeneous modes)
        TempLatArray<T,Ns> fldS0;  // scalar singlet
        TempLatArray<ComplexFieldWrapper<T,T>,NCs> fldCS0;  // complex scalar
        TempLatArray<SU2DoubletWrapper<T,T,T,T>, NSU2Doublet> fldSU2Doublet0;  // SU2 doublet

		    //Initial time-derivatives (homogeneous modes)
        TempLatArray<T,Ns> piS0;  // scalar singlet
        TempLatArray<ComplexFieldWrapper<T,T>,NCs> piCS0;  // complex scalar
        TempLatArray<SU2DoubletWrapper<T,T,T,T>, NSU2Doublet> piSU2Doublet0;  // SU2 doublet
        T b0;

        //Initial potential
        T pot0, pot0SI;

        //Couplings and charges
        //Charges manager. Say what couples to what and return in a user friendly way the appropriate charge
        // times coupling combination. Important for a general mechanism to couple arbitrary fields one to another.
        CsU1Couplings gQ_CsU1;
        SU2DoubletU1Couplings gQ_SU2DblU1;
        SU2DoubletSU2Couplings gQ_SU2DblSU2;


        // Effective masses, used for setting the initial fluctuations of the scalar fields
        TempLatArray<T,Ns> masses2S; // scalar singlet
        TempLatArray<ComplexFieldWrapper<T,T>,NCs> masses2CS; // complex scalar
        TempLatArray<SU2DoubletWrapper<T,T,T,T>,NSU2Doublet> masses2SU2Doublet;  // SU2 doublet


        T alpha, fStar, omegaStar; //Rescalings for program variable definitions: (alpha,f_*,w_*)

        T dx, kIR, dt;  //Length element and time step

        // name of the model
        std::string name;

        // --> Graviational u fields
        std::unique_ptr<FieldCollection<Field, T, 6, true>> fldGWs;
        std::unique_ptr<FieldCollection<Field, T, 6, true>> piGWs;


        AbstractModel(ParameterParser& parser, const LatticeParameters<T>& par, std::shared_ptr<MemoryToolBox> toolBox, T pDt, std::string pName="") :
          fldS("scalar",toolBox,par),
          piS("pi_scalar",toolBox,par),
          fldCS("cmplx_scalar",toolBox,par),
          piCS("pi_cmplx_scalar",toolBox,par),
          fldSU2Doublet("SU2Doublet",toolBox,par),
          piSU2Doublet("pi_SU2Doublet",toolBox,par),
          fldU1("U1",toolBox,par),
          piU1("pi_U1",toolBox,par),
          fldSU2("SU2Fld",toolBox,par),
          piSU2("pi_SU2Fld",toolBox,par),
          aSI(1),aI(1), aIM(1),
          dx(par.getDx()),
          kIR(par.getKIR()),
          dt(pDt),
          name(pName),
          fldGWs(parser.get<bool>("withGWs", false) ? std::make_unique<FieldCollection<Field, T, 6, true>>("fldGWs", toolBox, par) : nullptr),
          piGWs( parser.get<bool>("withGWs", false) ? std::make_unique<FieldCollection<Field, T, 6, true>>("piGWs", toolBox, par) : nullptr)
          {
            // Uncomment these exceptions in case you want to run a model with more than one U(1) or SU(2) gauge field (this feature has yet not been tested)
            if(NDim != 3 && fldGWs != nullptr) throw(RunParametersInconsistent("NDims must be equal to 3 to run GWs. If you want to run with NDim != 3, make sure withGWs = false."));
            if(NU1 > 1) throw(NotTested("The physics interface has not been fully tested with NU1 > 1. Abort. If you want to go on anyway, uncomment the exception thrown in src/include/CosmoInterface/abstractmodel.h and please report any problems."));
            if(NSU2 > 1) throw(NotTested("The physics interface has not been fully tested with NSU2 > 1. Abort. If you want to go on anyway, uncomment the exception thrown in src/include/CosmoInterface/abstractmodel.h and please report any problems."));

			      // We read the gauge couplings and charges from the input file. They are all set to 1 unless otherwise specified.
            auto gU1s = parser.get<double,NU1>("gU1s", 1.0);
            auto gSU2s = parser.get<double,NSU2>("gSU2s", 1.0);
            auto CSU1Charges = parser.get<double,CsU1Couplings::howManyCouples()>("CSU1_charges",1);
            auto SU2DoubletU1Charges = parser.get<double,SU2DoubletU1Couplings::howManyCouples()>("SU2DoubletU1_charges",1);
            auto SU2DoubletSU2Charges = parser.get<double,SU2DoubletSU2Couplings::howManyCouples()>("SU2DoubletSU2_charges",1);

			      // Gauge couplings and charges are set
            gQ_CsU1.setEffectiveCharges(CSU1Charges, gU1s);
            gQ_SU2DblU1.setEffectiveCharges(SU2DoubletU1Charges, gU1s);
            gQ_SU2DblSU2.setEffectiveCharges(SU2DoubletSU2Charges, gSU2s);

          }

        //The functions below are to be redefined in the models. We define them here to be able to compile anyhow, even if they are not needed by a specific model.
        // Note: A better alternative is to use if constexpr in the main, but this is c++17. Could also use macro but don't like it.

        template <int N>
        auto potDeriv(Tag<N> t)
        {
            throw(PotentialDerivativeNotDefined("You tried to call potDeriv N = " +std::to_string(N) +", which is not defined in your model. Abort."));
            return ZeroType();  // the simulation aborts if the function is not defined in the model. The return ZeroType is to have a return type.
        }
        template <int N>
        auto potDeriv2(Tag<N> t)
        {
            throw(PotentialDerivativeNotDefined("You tried to call potDeriv2 N = " +std::to_string(N) +", which is not defined in your model. Abort."));
            return ZeroType();  // the simulation aborts if the function is not defined in the model
        }

        template <int N>
        auto potDerivNormCS(Tag<N> t)
        {
            throw(PotentialDerivativeNotDefined("You tried to call potDerivNormCS N = " +std::to_string(N) +", which is not defined in your model. Abort."));
            return ZeroType();  // the simulation aborts if the function is not defined in the model
        }

        template <int N>
        auto potDeriv2NormCS(Tag<N> t)
        {
            throw(PotentialDerivativeNotDefined("You tried to call potDeriv2NormCS N = " +std::to_string(N) +", which is not defined in your model. Abort."));
            return ZeroType();  // the simulation aborts if the function is not defined in the model
        }

        template <int N>
        auto potDerivNormSU2Doublet(Tag<N> t)
        {
            throw(PotentialDerivativeNotDefined("You tried to call potDerivNormSU2Doublet N = " +std::to_string(N) +", which is not defined in your model. Abort."));
            return ZeroType();  // the simulation aborts if the function is not defined in the model
        }

        template <int N>
        auto potDeriv2NormSU2Doublet(Tag<N> t)
        {
            throw(PotentialDerivativeNotDefined("You tried to call potDeriv2NormSU2Doublet N = " +std::to_string(N) +", which is not defined in your model. Abort."));
            return ZeroType();  // the simulation aborts if the function is not defined in the model
        }


        auto pi_GWtensor(Tag<1>, Tag<1>) {return (*piGWs)(0_c);}
        auto pi_GWtensor(Tag<1>, Tag<2>) {return (*piGWs)(1_c);}
        auto pi_GWtensor(Tag<1>, Tag<3>) {return (*piGWs)(2_c);}
        auto pi_GWtensor(Tag<2>, Tag<1>) {return (*piGWs)(1_c);}
        auto pi_GWtensor(Tag<2>, Tag<2>) {return (*piGWs)(3_c);}
        auto pi_GWtensor(Tag<2>, Tag<3>) {return (*piGWs)(4_c);}
        auto pi_GWtensor(Tag<3>, Tag<1>) {return (*piGWs)(2_c);}
        auto pi_GWtensor(Tag<3>, Tag<2>) {return (*piGWs)(4_c);}
        auto pi_GWtensor(Tag<3>, Tag<3>) {return (*piGWs)(5_c);}

        auto GWtensor(Tag<1>, Tag<1>) {return (*fldGWs)(0_c);}
        auto GWtensor(Tag<1>, Tag<2>) {return (*fldGWs)(1_c);}
        auto GWtensor(Tag<1>, Tag<3>) {return (*fldGWs)(2_c);}
        auto GWtensor(Tag<2>, Tag<1>) {return (*fldGWs)(1_c);}
        auto GWtensor(Tag<2>, Tag<2>) {return (*fldGWs)(3_c);}
        auto GWtensor(Tag<2>, Tag<3>) {return (*fldGWs)(4_c);}
        auto GWtensor(Tag<3>, Tag<1>) {return (*fldGWs)(2_c);}
        auto GWtensor(Tag<3>, Tag<2>) {return (*fldGWs)(4_c);}
        auto GWtensor(Tag<3>, Tag<3>) {return (*fldGWs)(5_c);}



        //Sometimes, it can be useful to get "any field" of the model. This function implements this in a generic way.
        Field<T> getOneField()
        {
            if (Ns > 0) return fldS(0_c);
            else if (NCs > 0) return fldCS(0_c)(0_c);
            else if (NSU2Doublet > 0) return fldSU2Doublet(0_c)(0_c);
            else if (NU1 > 0) return fldU1(0_c)(1_c);
            else if (NSU2 > 0) return fldSU2(0_c)(1_c)(1_c);
            else {
                throw (EmptyModel("The model seems empty, cannot return a field. Abort."));
                return fldS(0_c);
            }

        }

         // The "MemoryToolBox" is a shared variable between most instances of the program. It contains many useful informations about
        // the intrinsic parameter of the library. Sometimes, some of the classes need the MemoryToolBox to be created. This function
        // the user to quickly get it from the model.
        std::shared_ptr<MemoryToolBox> getToolBox()
        {
            return getOneField().getToolBox();
        }




		    //set the initial value of the potential and the masses of the fields from the expression of the potential.
        void setInitialPotentialAndMassesFromPotential()
        {

			      // This adds the homogeneous components of the fields at one point:
            this->addInitValueOnePoint();
            // It's just a trick to compute the initial potential and masses; it must be removed afterwards with "removeInitValue()"

            //Compute initial potential
            pot0 = GetValue::get(Potential::potential(static_cast<R&>(*this)),0);

            //Compute second derivatives of the potential, giving the mass square.
            ForLoop(j, 0, Ns-1, masses2S(j) = GetValue::get(Potential::deriv2S(static_cast<R&>(*this),j),0));
            ForLoop(j, 0, NCs-1,
                            masses2CS(j) = Complexify(GetValue::get(Potential::deriv2CS(static_cast<R&>(*this),j)(0_c),0),
                                                     GetValue::get(Potential::deriv2CS(static_cast<R&>(*this),j)(1_c),0));
            );
            ForLoop(j, 0, NSU2Doublet-1,
                    masses2SU2Doublet(j) = MakeSU2Doublet(a,GetValue::get(Potential::deriv2SU2Doublet(static_cast<R&>(*this),j)(a),0));
            );


			     // This removes the homogeneous component at one point added previously with "addInitValueOnePoint()"
            this->removeInitValue();
        }

        //set only the initial value of the potential from the expression of the potential. Useful if you want to specify
        // the masses not from the second derivative of the potential; you will still need to compute pot0.
        void setInitialPotentialFromPotential()
        {

            // This adds the homogeneous components of the fields at one point:
            this->addInitValueOnePoint();
            // It's just a trick to compute the initial potential and masses; it must be removed afterwards with "removeInitValue()"

            //Compute initial potential at t=0
            pot0 = GetValue::get(Potential::potential(static_cast<R&>(*this)),0);


			      // This removes the homogeneous component at one point added previously with "addInitValueOnePoint()"
            this->removeInitValue();

        }

    private:

		// This sets the homogeneous components of the fields at a single point.
        void addInitValueOnePoint()
        {

            ForLoop(j, 0, Ns-1,fldS(j).getSet(0) = fldS0[j] / fStar);
            ForLoop(j, 0, NCs-1,
                    ForLoop(i,0,1,
                            fldCS(j)(i).getSet(0) = fldCS0(j)(i) / fStar;
                    )
            );
            ForLoop(j, 0, NSU2Doublet-1,
                    ForLoop(i,0,3,
                            fldSU2Doublet(j)(i).getSet(0) = fldSU2Doublet0(j)(i) / fStar;
                    )
            );
        }

		// This removes the homogeneous components of the fields at a single point.
        void removeInitValue()
        {
            ForLoop(j, 0, Ns-1,fldS(j).getSet(0) = 0);
            ForLoop(j, 0, NCs-1,
                    ForLoop(i,0,1,
                            fldCS(j)(i).getSet(0) = 0;
                    )
            );
            ForLoop(j, 0, NSU2Doublet-1,
                    ForLoop(i,0,3,
                            fldSU2Doublet(j)(i).getSet(0) = 0;
                    )
            );
        }

    public:




    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/abstractmodel_test.h"
#endif


#endif
