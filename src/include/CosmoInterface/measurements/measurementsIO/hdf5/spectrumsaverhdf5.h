#ifndef COSMOINTERFACE_MEASUREMENTS_MEASUREMENTSIO_HDF5_SPECTRUMSAVERHDF5_H
#define COSMOINTERFACE_MEASUREMENTS_MEASUREMENTSIO_HDF5_SPECTRUMSAVERHDF5_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020
#ifdef HDF5
#include "TempLat/util/prettytostring.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "CosmoInterface/runparameters.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5file.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionresult.h"


namespace TempLat {


    /** \brief A class which save the spectrum (or any RadialProjectionResults) in a hdf5 format.
     *
     *
     * Unit test: make test-spectrumsaverhdf5
     **/

    template <typename T>
    class SpectrumSaverHDF5 {
    public:


        /* Put public methods here. These should change very little over time. */

        SpectrumSaverHDF5(FilesManager& fm, std::string fn, bool amIRoot, bool append, const RunParameters<T>& rPar) :
        filename (fm.getWorkingDir() +"spectra_" + fn +  ".h5"),
        verbosity(rPar.spectraVerbosity),
        nBins(rPar.nBinsSpectra),
        deltaKBin(rPar.deltaKBin),
        nGrid(rPar.N),
        kIR(rPar.kIR)
        {
            HDF5File f;
            if(!append) {
                f.create(filename);
                f.close();
            }

        }

        SpectrumSaverHDF5(FilesManager& fm, const Field<T>& fld, bool amIRoot, bool append, const RunParameters<T>& rPar)  :
        filename(fm.getWorkingDir() + "spectra_" + fld.toString().erase(fld.toString().find("(",3)) + ".h5"),
        verbosity(rPar.spectraVerbosity),
        nBins(rPar.nBinsSpectra),
        deltaKBin(rPar.deltaKBin),
        nGrid(rPar.N),
        kIR(rPar.kIR)
        {
            HDF5File f;
            if(!append) {
                f.create(filename);
                f.close();
            }
        }

        virtual ~SpectrumSaverHDF5(){}



        void save(std::vector<std::shared_ptr<RadialProjectionResult<T>>> arr, T t)  {

          if(nBins > -1){
              for(size_t i = 0; i < arr.size(); ++i) {
                //  arr[i]->rebin(nBins , std::floor(pow(3, 0.5) / 2.0 * nGrid));  // The second argument is the total length of the grid in k . The bin size in computed as this divided by nBins. This choice corresponds to bin of size kIR for the default choice.
                  arr[i]->rescaleBins(kIR);
              }
          }

            std::vector<T> valAv;
            std::vector<T> mult;

            HDF5File file;
            file.open(filename, ReadWrite);

            HDF5Group group = file.createOrOpenGroup(PrettyToString::get(t,10));


            for(auto&& it :(*arr[0]))
            {
                mult.emplace_back(it.getValue().multiplicity * 2); // *2 is to print an integer multiplicity, against original design
            }
            HDF5Dataset multData = group.createDataset<T>("momMultiplicity", std::vector<hsize_t>(1, mult.size()));
            multData.write(mult);
            multData.close();



            if(verbosity != 0){
              std::vector<T> binAv;
              for(auto&& it :(*arr[0]))
              {
                  binAv.emplace_back(it.getBin().average);
              }
              HDF5Dataset binAvData = group.createDataset<T>("momBinAverage", std::vector<hsize_t>(1, binAv.size()));
              binAvData.write(binAv);
              binAvData.close();
            }


            if(verbosity != 1){
              HDF5Dataset binAvData = group.createDataset<T>("momBinCentralValues", std::vector<hsize_t>(1, arr[0]->getCentralBinBounds().size()));
              binAvData.write(arr[0]->getCentralBinBounds());
              binAvData.close();
            }





            for(size_t i = 0; i< arr.size(); ++i)
            {
                valAv.clear();
                for(auto&& it :(*arr[i]))
                {
                    valAv.emplace_back(it.getValue().average);
                }

                HDF5Dataset valueAvData = group.createDataset<T>("spectAverage_"+std::to_string(i), std::vector<hsize_t>(1, valAv.size()));
                valueAvData.write(valAv);
                valueAvData.close();
            }



            if(verbosity == 2)
            {
                std::vector<T> binMinVal;
                std::vector<T> valMinVal;
                std::vector<T> binMaxVal;
                std::vector<T> valMaxVal;
                std::vector<T> binSampleVar;
                std::vector<T> valSampleVar;
                for(auto&& it : (*arr[0]))
                {
                    binMinVal.emplace_back(it.getBin().minVal);
                    valMinVal.emplace_back(it.getValue().minVal);
                    binMaxVal.emplace_back(it.getBin().maxVal);
                    valMaxVal.emplace_back(it.getValue().maxVal);
                    binSampleVar.emplace_back(it.getBin().sampleVariance);
                    valSampleVar.emplace_back(it.getValue().sampleVariance);

                }
                HDF5Dataset binMinValData = group.createDataset<T>("momBinMin",std::vector<hsize_t>(1, binMinVal.size()));
                binMinValData.write(binMinVal);
                HDF5Dataset valMinValData = group.createDataset<T>("spectMin", std::vector<hsize_t>(1, valMinVal.size()));
                valMinValData.write(valMinVal);
                HDF5Dataset binMaxValData = group.createDataset<T>("momBinMax", std::vector<hsize_t>(1, binMaxVal.size()));
                binMaxValData.write(binMaxVal);
                HDF5Dataset valMaxValData = group.createDataset<T>("spectMax", std::vector<hsize_t>(1, valMaxVal.size()));
                valMaxValData.write(valMaxVal);
                HDF5Dataset binSampleVarianceData = group.createDataset<T>("momBinVariance", std::vector<hsize_t>(1, binSampleVar.size()));
                binSampleVarianceData.write(binSampleVar);
                HDF5Dataset valSampleVarianceData = group.createDataset<T>("spectVariance", std::vector<hsize_t>(1, valSampleVar.size()));
                valSampleVarianceData.write(valSampleVar);

                binMinValData.close();
                valMinValData.close();
                binMaxValData.close();
                valMaxValData.close();
                binSampleVarianceData.close();
                valSampleVarianceData.close();

            }
            group.close();
            file.close();
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        std::string filename;
        int verbosity;
        int nBins;
        int deltaKBin;
        T nGrid, kIR;


    };

    class SpectrumSaverHDF5Tester{
    public:
#ifdef TEMPLATTEST

        static inline void Test(TDDAssertion &tdd);

#endif
    };

} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/measurementsIO/hdf5/spectrumsaverhdf5_test.h"
#endif
#endif

#endif
