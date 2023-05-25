#ifndef TEMPLAT_COSMOINTERFACE_GWSPOWERSPECTRUM_H
#define TEMPLAT_COSMOINTERFACE_GWSPOWERSPECTRUM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros, Adrien Florio, Nicolás Layza,  Year: 2022

#include "TempLat/util/function.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/algebra.h"

#include "CosmoInterface/definitions/radialprojectorGW.h"
#include "CosmoInterface/runparameters.h"


namespace TempLat {


    /** \brief A class which computes the power spectrum, with the appropriate rescaling to make it volume independent.
     *
     *
     **/

     MakeException(WrongPSTypeGW);

     class GWsPowerSpectrumMeasurer{
      public:
        template<typename T>
        GWsPowerSpectrumMeasurer(const RunParameters<T>& par) :
        nbins(par.nBinsSpectra),
        PSType(par.powerSpectrumType),
        PSVersion(par.powerSpectrumVersion),
        PRJType(par.GWprojectorType)
        {}

        template<class Model >
        auto gwspowerSpectrum(Model& model)  {
            return this->gwspowerSpectrum(model, GetNGrid::get(model.getOneField()),  model.getOneField().getKIR());
        }


        template<class Model, typename T>
        auto gwspowerSpectrum(Model& model, ptrdiff_t N, T kIR)  {

            ptrdiff_t N3 = pow<3>(N);
            T dx = 2 * Constants::pi<T> / kIR / N;  // lattice spacing

            T kMaxBins = std::floor(pow(3, 0.5) / 2.0 * N) + 1;

           if (PSVersion != 3){
                auto fk2 = projectRadiallyGW(model, PSVersion == 1, PRJType, PSType, PSVersion).measure(model, nbins, kMaxBins);
                if (PSType == 2){
                    return Function(ntilde, pow<3>(kIR * ntilde * dx ) / N3 /(8*pow<2>(Constants::pi<T>)*pow(model.aI,2*model.alpha))*pow<2>(model.fStar / Constants::reducedMPlanck<T>)/Energies::rho(model)) * fk2;
                }
                else if (PSType == 1){
                    fk2.sumInsteadOfAverage();
                    return Function(ntilde, kIR * ntilde * dx  / pow<5>(N) / (8*(Constants::pi<T>)*pow(model.aI,2*model.alpha))*pow<2>(model.fStar/Constants::reducedMPlanck<T>)/Energies::rho(model)) * fk2;
                }
                else{
                  throw(WrongPSTypeGW("You tried to call an undefined PSType " +std::to_string(PSType) + ", abort."));
                  return fk2; //To remove moot warning.
                }
            }
            else{
                if (PSType == 2){
                    auto fk2 = projectRadiallyGW(model, false, PRJType, PSType, PSVersion).measure(model,  nbins, kMaxBins);
                    return  (pow<3>(kIR *  dx ) / N3 / (8*pow<2>(Constants::pi<T>)*pow(model.aI,2*model.alpha))*pow<2>(model.fStar / Constants::reducedMPlanck<T>)/Energies::rho(model)) * fk2;
                }
                else{
                    auto fk2 = projectRadiallyGW(model, false, PRJType, PSType, PSVersion).measure(model,  nbins, kMaxBins);
                    fk2.sumInsteadOfAverage();
                    return (kIR  * dx  / pow<5>(N) / (8*(Constants::pi<T>)*pow(model.aI,2*model.alpha))*pow<2>(model.fStar / Constants::reducedMPlanck<T>)/Energies::rho(model)) * fk2;
                }
            }
        }




      private:
        ptrdiff_t nbins;
        int PSType;
        int PSVersion;
        int PRJType;
     };




    class GWsPowerSpectrumTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif

    };




} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/gwspowerspectrum_test.h"
#endif


#endif
