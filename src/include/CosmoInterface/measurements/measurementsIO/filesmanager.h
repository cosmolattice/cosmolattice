#ifndef COSMOINTERFACE_MEASUREMENTS_FILESMANAGER_H
#define COSMOINTERFACE_MEASUREMENTS_FILESMANAGER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorytoolbox.h"

namespace TempLat {


    /** \brief Interface to switch between hdf5 and std output for measurements. This class hides the polymorphism under the hood.
     * It is mostly useful for the averages, which are not implemented in hdf5 yet.
     *
     * 
     * Unit test: make test-filesmanager
     **/

    MakeException(UseHDF5ButNotCompiled);


    class FilesManager{
    public:
        FilesManager(std::string fn, std::shared_ptr<MemoryToolBox> toolbox, bool pUseHDF5, bool pUseHDF5Spectra, bool pPrintHeaders, std::string pTag = "") :
        mToolbox(toolbox),
        mUseHDF5Spectra(pUseHDF5Spectra),
        mPrintHeaders(pPrintHeaders),
        workingDir(fn),
        tag(pTag)
        {
        }

        void flush()
        {
        }

        bool getUseHDF5() const {
            return false; //Averages in hdf5 not implemented.
        }
        bool getUseHDF5Spectra() const {
            return mUseHDF5Spectra;
        }
        bool getPrintHeaders() const {
            return mPrintHeaders;
        }

        auto getToolBox() const
        {
            return mToolbox;
        }

        std::string getWorkingDir() const
        {
            return workingDir;
        }
        std::string getTag() const
        {
            return tag;
        }

    private:

        std::shared_ptr<MemoryToolBox> mToolbox;
        bool mUseHDF5Spectra;
        bool mPrintHeaders;
        std::string workingDir;
        std::string tag;
    };




} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/measurementsIO/filesmanager_test.h"
#endif


#endif
