#ifndef COSMOINTERFACE_MEASUREMENTSSAVER_H
#define COSMOINTERFACE_MEASUREMENTSSAVER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "TempLat/util/conditionaloutput/outputstream.h"
#include <sstream>
#include "CosmoInterface/measurements/measurementsIO/std/measurementssaverstd.h"

namespace TempLat {

    /** \brief A class which implements an interface to the measurement IO. Allow to switch between different format, but for now only std is implemented
     * (in the future, may want to have a hdf5 format for the measurements as well).
     *
     * Unit test: make test-measurementssaver
     **/

    template <typename T>
    class MeasurementsSaver{
    public:

        MeasurementsSaver(FilesManager& fm, std::string fn, bool amIRoot, bool appendMode, const std::vector<std::string>& headers = {}, bool dontCreate = false) :
        printHeaders(fm.getPrintHeaders()){
            if(!dontCreate) {
                ms = std::make_shared<MeasurementsSaverStd<T>>(fm, fn, amIRoot, appendMode, headers);
                IExist = true;
            }
            else IExist = false;
        }

        MeasurementsSaver(FilesManager& fm, const Field<T>& fld,bool amIRoot, bool appendMode, const std::vector<std::string>& headers = {}, bool dontCreate = false):
        printHeaders(fm.getPrintHeaders()){
            if(!dontCreate) {
                ms = std::make_shared<MeasurementsSaverStd<T>>(fm, fld, amIRoot, appendMode, headers);
                IExist = true;
            }
            else IExist = false;
        }

        void addAverage(const T& r)
        {
            if(IExist) ms->addAverage(r);
        }

        void save()
        {
            if(IExist) ms->save();
        }

    private:

        std::shared_ptr<MeasurementsSaverStd<T>> ms;
        bool IExist;
        bool printHeaders;

    };
    


} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/measurementsIO/measurementssaver_test.h"
#endif


#endif
