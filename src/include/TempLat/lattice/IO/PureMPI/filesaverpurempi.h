#ifndef TEMPLAT_LATTICE_IO_PUREMPI_FILESAVERPUREMPI_H
#define TEMPLAT_LATTICE_IO_PUREMPI_FILESAVERPUREMPI_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/parameters/parameterparser.h"

namespace TempLat {

  MakeException(PureMPISaverNotImplemented);


    /** \brief A class which should be imlemented if someone wants to save files only with mpi.
     *
     *
     * Unit test: make test-filesaverpurempi
     **/

    class FileSaverPureMPI {
    public:
        /* Put public methods here. These should change very little over time. */
        FileSaverPureMPI() {

        }
        void open(std::string fn, std::shared_ptr<MemoryToolBox> toolBox)
		{
          throw PureMPISaverNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
		}
        void open(std::string fn)
        {
            throw PureMPISaverNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }

        void create(std::string fn)
        {
            throw PureMPISaverNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }

        template<typename R, typename T>
        void save(T t, R r, std::string name){ //used to store an entity in a time series. The name is the one of te group, data set labelled by t.
            throw PureMPISaverNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }

        template<typename R>
        void save(R&& r){
          throw PureMPISaverNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }

        void close(){
            throw PureMPISaverNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }

        template<typename Float, typename R>
        void save(Float t, R&& r) {
            throw PureMPISaverNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }

        void save(ParameterParser& pars)
        {
            throw PureMPISaverNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");

        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/IO/PureMPI/filesaverpurempi_test.h"
#endif


#endif
