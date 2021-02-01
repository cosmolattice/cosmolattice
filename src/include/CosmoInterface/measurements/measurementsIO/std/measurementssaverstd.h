#ifndef COSMOINTERFACE_MEASUREMENTS_STD_MEASUREMENTSSAVERSTD_H
#define COSMOINTERFACE_MEASUREMENTS_STD_MEASUREMENTSSAVERSTD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/conditionaloutput/outputstream.h"
#include <sstream>
#include <iomanip>
#include "TempLat/lattice/field/field.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"

namespace TempLat {


    /** \brief A class which is used to save measurements using the standard library.
     * 
     * 
     * Unit test: make test-measurementssaverstd
     **/

    template<typename T>
    class MeasurementsSaverStd {
    public:
        /* Put public methods here. These should change very little over time. */
        MeasurementsSaverStd(FilesManager& fm, std::string fn, bool amIRoot, bool appendMode, const std::vector<std::string>& headers = {}) :
        mMode(!appendMode ?  std::ios_base::out : std::ios_base::app ),
        outputAv(std::make_shared<OutputStream<T>>(fm.getWorkingDir() + fm.getTag() +"average_" + fn + ".txt", amIRoot, mMode)),
        stream(std::make_shared<std::stringstream>()),
                headerStream(std::make_shared<std::stringstream>()),
                headerSaved(false)
        {
            if(!appendMode && fm.getPrintHeaders()) for(auto& str : headers) addHeader(str);
            else headerSaved = true;
        }

        MeasurementsSaverStd(FilesManager& fm, const Field<T>& fld,bool amIRoot, bool appendMode, const std::vector<std::string>& headers = {}) :
                mMode(!appendMode ?  std::ios_base::out : std::ios_base::app ),
                stream(std::make_shared<std::stringstream>()),
                headerStream(std::make_shared<std::stringstream>()),
                headerSaved(false)
        {
            std::string name = fld.toString();
            name = name.erase(name.find("(",3));
            name = fm.getWorkingDir() +"average_" + name;
            outputAv = std::make_shared<OutputStream<T>>(name + ".txt", amIRoot, mMode);
            if(!appendMode && fm.getPrintHeaders()) for(auto& str : headers) addHeader(str);
            else headerSaved = true;

        }

        virtual ~MeasurementsSaverStd(){}

        void addAverage(const T& r)
        {
            (*stream) << std::setprecision(15) << "    " << r;
        }

        void addHeader(const std::string& str)
        {
            (*headerStream) << "    " << str;
        }
        void save()
        {

            if(!headerSaved){
                (*outputAv) << "#" << StringTrimmer::ltrimmed(headerStream->str()) << "\n";
                headerSaved = true;
            }
            (*outputAv) << StringTrimmer::ltrimmed(stream->str()) << "\n";
            stream->str("");
            outputAv->flush();
        }



    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::ios_base::openmode mMode;
        std::shared_ptr<OutputStream<T>> outputAv;
        std::shared_ptr<std::stringstream> stream;
        std::shared_ptr<std::stringstream> headerStream;

        bool headerSaved;



    };

    struct MeasurementsSaverStdTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/measurementsIO/std/measurementssaverstd_test.h"
#endif


#endif
