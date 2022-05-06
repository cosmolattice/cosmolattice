#ifndef TEMPLAT_COSMOINTERFACE_POWERSPECTRUM_H
#define TEMPLAT_COSMOINTERFACE_POWERSPECTRUM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "TempLat/util/function.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/algebra.h"
#include "CosmoInterface/runparameters.h"


namespace TempLat {


    /** \brief A class which computes the power spectrum, with the appropriate rescaling to make it volume independent.
     *
     *
     **/

     MakeException(WrongPSType);

     class PowerSpectrumMeasurer{
      public:
        template<typename T>
        PowerSpectrumMeasurer(const RunParameters<T>& par) :
        nbins(par.nBinsSpectra),
        PSType(par.powerSpectrumType),
        PSVersion(par.powerSpectrumVersion)
        {}
        template< typename R>
        auto powerSpectrum(R f)  {
            return this->powerSpectrum(f, GetNGrid::get(f), GetKIR::getKIR(f));
        }



        // This function computes the power spectrum.
        // --> The normalization factor ensures that it recovers the appropriate expression in the continuum limit.
        //     This is discussed in Sect. 3 of arXiv:2006.15122.

        template<typename T>
        auto powerSpectrum(Field<T> f, ptrdiff_t N, T kIR)  {

            ptrdiff_t N3 = pow<3>(N);
            T dx = 2 * Constants::pi<T> / kIR / N;  // lattice spacing

            T kMaxBins = std::floor(pow(3, 0.5) / 2.0 * N) + 1;

            if (PSVersion != 3){
                auto fk2 = projectRadiallyFourier(pow<2>(abs(f.inFourierSpace())), PSVersion == 1).measure(nbins, kMaxBins); //PSversion == true is a boolean. It tells the spectrum to use the centralValues, and not the bins, when rescaling.

                if (PSType == 2){
                  return Function(ntilde, pow<3>(kIR * ntilde * dx ) / N3 / 2.0 / pow<2>(Constants::pi<T>)) * fk2;
                }
                else if (PSType == 1){
                  fk2.sumInsteadOfAverage();
                  return Function(ntilde, kIR * ntilde * dx  / pow<5>(N) / 2.0 / Constants::pi<T>) * fk2;
                }
                else{
                  throw(WrongPSType("You tried to call an undefined PSType " +std::to_string(PSType) + ", abort."));
                  return fk2; //To remove moot warning.
                }
            }
            else{
              WaveNumber ntilde(f.getToolBox()); //WaveNumber is the same as FourierSite, except it does not require to know the dimension at compile time.
              if (PSType == 2){

                  auto fk2 = projectRadiallyFourier(pow<3>(ntilde.norm()) * pow<2>(abs(f.inFourierSpace())), false).measure(nbins, kMaxBins);
                  return  (pow<3>(kIR *  dx ) / N3 / 2.0 / pow<2>(Constants::pi<T>)) * fk2;
              }
              else if (PSType == 1){
                auto fk2 = projectRadiallyFourier(ntilde.norm() * pow<2>(abs(f.inFourierSpace())), false).measure(nbins, kMaxBins);
                  fk2.sumInsteadOfAverage();
                  return ( kIR  * dx  / pow<5>(N) / 2.0 / Constants::pi<T>) * fk2;
              }
              else{
                throw(WrongPSType("You tried to call an undefined PSType " +std::to_string(PSType) + ", abort."));
                return projectRadiallyFourier(abs(f.inFourierSpace()), false).measure(nbins, kMaxBins); //To remove moot warning.
              }
            }
        }


        template<typename R, typename T>
        auto powerSpectrum(R f, ptrdiff_t N, T kIR) { //This function is for expression/composite operator, which need their own memory to perform the fourier transform.
            Field<T> tmp("tmp", GetToolBox::get(f));
            tmp = f;

            return this->powerSpectrum(tmp, N , kIR);
        }

      private:
        ptrdiff_t nbins;
        int PSType;
        int PSVersion;
     };


    class PowerSpectrumTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif

    };




} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/powerspectrum_test.h"
#endif


#endif
