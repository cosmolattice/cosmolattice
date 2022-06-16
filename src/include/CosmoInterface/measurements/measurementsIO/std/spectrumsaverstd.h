#ifndef COSMOINTERFACE_MEASUREMENTS_STD_SPECTRUMSAVERSTD_H
#define COSMOINTERFACE_MEASUREMENTS_STD_SPECTRUMSAVERSTD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/conditionaloutput/outputstream.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionresult.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/field/field.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "CosmoInterface/runparameters.h"


namespace TempLat {


    /** \brief A class which implements the std spectra output.
     *
     *
     * Unit test: make test-spectrumsaverstd
     **/

    template <typename T>
    class SpectrumSaverStd {
    public:


        SpectrumSaverStd(FilesManager& fm, std::string fn, bool amIRoot, bool append, const RunParameters<T>& rPar) :
        mMode(!append ?  std::ios_base::out : std::ios_base::app ),
        outputSpectrum(std::make_shared<OutputStream<T>>(fm.getWorkingDir() + "spectra_" + fn +  ".txt", amIRoot, mMode)),
        verbosity(rPar.spectraVerbosity),
        nBins(rPar.nBinsSpectra),
        deltaKBin(rPar.deltaKBin),
        nGrid(rPar.N),
        kIR(rPar.kIR),
        printHeader(fm.getPrintHeaders())
        {
        }

        virtual ~SpectrumSaverStd(){}

        SpectrumSaverStd(FilesManager& fm, const Field<T>& fld,bool amIRoot, bool append, const RunParameters<T>& rPar) :
        mMode(!append ?  std::ios_base::out : std::ios_base::app ),
        verbosity(rPar.spectraVerbosity),
        nBins(rPar.nBinsSpectra),
        deltaKBin(rPar.deltaKBin),
        nGrid(rPar.N),
        kIR(rPar.kIR),
        printHeader(fm.getPrintHeaders())
        {
            std::string name = fld.toString();
            name = name.erase(name.find("(",3));
            name = fm.getWorkingDir() +  "spectra_" + name;
            outputSpectrum = std::make_shared<OutputStream<T>>(name + ".txt", amIRoot, mMode);
        }

		 
        void save(std::vector<std::shared_ptr<RadialProjectionResult<T>>> arr, T t)
        {

            if(printHeader){
                if(verbosity != 1) (*outputSpectrum) << "1: binCentralValue" << " ";
                if(verbosity != 0) (*outputSpectrum) << (*arr[0])[0].getHeaderBin(verbosity, verbosity == 1 ? 1 : 2) << " ";
                for(size_t i = 0; i < arr.size() - 1; ++i) (*outputSpectrum) << (*arr[i])[0].getHeaderValue(verbosity, false, verbosity == 2 ? 4 * (i+1) + 2  : i+2)<< " ";
                (*outputSpectrum) << (*(arr.back()))[0].getHeaderValue(verbosity, true, verbosity == 2 ? 4 * (arr.size()) + 2 : (arr.size() + 1)) << "\n";

                printHeader = false;
            }


            auto mNBins = nBins;

            if(nBins > -1){
                for(size_t i = 0; i < arr.size(); ++i) {
                  //  arr[i]->rebin(nBins , std::floor(pow(3, 0.5) / 2.0 * nGrid));  // The second argument is the total length of the grid in k . The bin size in computed as this divided by nBins. This choice corresponds to bin of size kIR for the default choice.
                    arr[i]->rescaleBins(kIR);
                }
            } else mNBins = arr[0]->size();

            // Now we construct the file with the appropriate format.

            std::stringstream sstream;
            for(int i = 0; i< mNBins; ++i)
            {
                sstream.str("");

                if(verbosity == 0){
                  sstream  <<  (*arr[0]).getCentralBinBounds()[i];
                }
                else if (verbosity == 1){
                  sstream  <<  (*arr[0])[i].getBinString(verbosity);
                }
                else if (verbosity == 2){
                  sstream  <<  (*arr[0]).getCentralBinBounds()[i] << " " <<  (*arr[0])[i].getBinString(verbosity);
              }

              sstream <<  " " << (*arr[0])[i].getValueString(arr.size() <= 1, verbosity, true);
              for (size_t j = 1; j < arr.size() - 1; ++j) {
                  sstream << " " << (*arr[j])[i].getValueString(false, verbosity, true);
              }
              if(arr.size() > 1) sstream << " " << (*arr.back())[i].getValueString(true, verbosity, true);
              (*outputSpectrum) << sstream.str()  << "\n";


                /*if(verbosity != 1) sstream  << (*arr[0])[i].toString(true, arr.size() <= 1, verbosity); //true: bin information. arr.size() <= 1: multiplicity
                else sstream << (i+1) * deltaKBin *kIR << " " << (*arr[0])[i].toString(false, arr.size() <= 1,  verbosity); //for verbosity 1, show multiple of deltaKbin for bin value, and only mean value for the spectrum.
                for (size_t j = 1; j < arr.size() - 1; ++j) {
                    sstream << " " << (*arr[j])[i].toString(false, false, verbosity);
                }
                if(arr.size() > 1) sstream << " " << (*arr.back())[i].toString(false, true,verbosity);
                (*outputSpectrum) << sstream.str()  << "\n";*/
            }
            (*outputSpectrum)  << "\n\n";


        }
		
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        std::ios_base::openmode mMode;
        std::shared_ptr<OutputStream<T>> outputSpectrum;
        int verbosity;
        int nBins;
        int deltaKBin;
        T nGrid, kIR;
        bool printHeader;

    };

    struct SpectrumSaverStdTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/measurementsIO/std/spectrumsaverstd_test.h"
#endif


#endif
