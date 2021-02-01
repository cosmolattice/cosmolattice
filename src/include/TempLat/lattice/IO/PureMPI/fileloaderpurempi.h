#ifndef TEMPLAT_LATTICE_IO_PUREMPI_FILELOADERPUREMPI_H
#define TEMPLAT_LATTICE_IO_PUREMPI_FILELOADERPUREMPI_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/parameters/parameterparser.h"

namespace TempLat {


    /** \brief A class which should implement mpi IO, but does not.
     *
     *
     * Unit test: make test-fileloaderpurempi
     **/

     MakeException(PureMPILoaderNotImplemented);

    class FileLoaderPureMPI  {
    public:
        /* Put public methods here. These should change very little over time. */
        FileLoaderPureMPI() {

        }

        template<typename R>
        void load(R&& r) {
            throw PureMPILoaderNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }

        template<typename T>
        void loadReal(Field<T>& f)
        {
          throw PureMPILoaderNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }
        template<typename T>
        void loadComplex(FourierView<T>& f)
        {
          throw PureMPILoaderNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }

        void open(std::string fn, std::shared_ptr<MemoryToolBox> toolBox)
        {
            throw PureMPILoaderNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }
        void open(std::string fn)
        {
            throw PureMPILoaderNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }
        void reset()
        {
            throw PureMPILoaderNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }



        void loadParams(ParameterParser& par)
        {
            throw PureMPILoaderNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
        }


        template<typename R>
        void load(R& t, std::string name){ //used to store a number. The name is the one of the dataset which contains this number.
            throw PureMPILoaderNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");

        }


        void close(){
            throw PureMPILoaderNotImplemented("The pure MPI IO interface is yet to be implemented. If you require I/O for your lattice, please use the HighFive/HDF5 interface. If you don't, then just disable the options/functions which requires I/O");
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
#include "TempLat/lattice/IO/PureMPI/fileloaderpurempi_test.h"
#endif


#endif
