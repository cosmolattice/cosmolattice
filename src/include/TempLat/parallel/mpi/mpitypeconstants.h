#ifndef TEMPLAT_PARALLEL_MPI_MPITYPECONSTANTS_H
#define TEMPLAT_PARALLEL_MPI_MPITYPECONSTANTS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tdd.h"

#ifndef NOMPI
#include <mpi.h>
#else

typedef int MPI_Comm;
#ifndef MPI_COMM_WORLD
#define MPI_COMM_WORLD 1
#endif
#ifndef MPI_COMM_NULL
#define MPI_COMM_NULL 1
#endif

typedef int MPI_Op;

typedef int MPI_Datatype;

typedef int MPI_Status;

#endif

namespace TempLat {
    
    /** \brief A bunch of compile-time templates for getting the
     * right MPI constants for your type.
     *
     * Unit test: make test-mpitypeconstants
     **/
    
    /** \brief or without having to define a dummy variable to pass on, how about some templates: */
#ifndef NOMPI
    template <typename T>
    constexpr typename std::enable_if< std::is_same<char, T>::value, MPI_Datatype>::type MPITypeSelect() { return MPI_CHAR; }
    
    template <typename T>
    constexpr typename std::enable_if< std::is_same<bool, T>::value, MPI_Datatype>::type MPITypeSelect() { return MPI_CXX_BOOL; }
    
    template <typename T>
    constexpr typename std::enable_if< std::is_same<float, T>::value, MPI_Datatype>::type MPITypeSelect() { return MPI_FLOAT; }
    
    template <typename T>
    constexpr typename std::enable_if< std::is_same<double, T>::value, MPI_Datatype>::type MPITypeSelect() { return MPI_DOUBLE; }
    
    template <typename T>
    constexpr typename std::enable_if< std::is_same<int, T>::value, MPI_Datatype>::type MPITypeSelect() { return MPI_INT; }
    
    template <typename T>
    constexpr typename std::enable_if< std::is_same<long int, T>::value, MPI_Datatype>::type MPITypeSelect() { return MPI_LONG; }
    
    template <typename T>
    constexpr typename std::enable_if< std::is_same<long long int, T>::value, MPI_Datatype>::type MPITypeSelect() { return MPI_LONG_LONG_INT; }
    
    template <typename T>
    constexpr typename std::enable_if< std::is_same<unsigned long int, T>::value, MPI_Datatype>::type MPITypeSelect() { return MPI_UNSIGNED_LONG; }
    
    template <typename T>
    constexpr typename std::enable_if< std::is_same<unsigned long long int, T>::value, MPI_Datatype>::type MPITypeSelect() { return MPI_UNSIGNED_LONG_LONG; }
    
    /** \brief If you call any method that relies on MPITypeSelect, but you want to pass
     * a custom class with a custom type, your class can implement the method
     * `static MPI_Datatype getMPIType();` (see ghostupdater_test.h of an example!)
     * which is then detected by the following SFINAE template:
     */
    // SFINAE test
    template <typename T>
    class HasToMPIDataType
    {
    private:
        typedef char YesType[1];
        typedef char NoType[2];
        
        template <typename C> static YesType& test( decltype(&C::getMPIType) ) ;
        template <typename C> static NoType& test(...);
        
        
    public:
        enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
    };
    template <typename T>
    constexpr typename std::enable_if< HasToMPIDataType<T>::value, MPI_Datatype>::type MPITypeSelect() { return T::getMPIType(); }

    
#else
    
    template <typename T>
    constexpr MPI_Datatype MPITypeSelect() { return 0; }
    
    //          maximum
#define MPI_MAX   0
    
    //          minimum
#define MPI_MIN   0
    
    //          sum
#define MPI_SUM   0
    
    //          product
#define MPI_PROD  0
    
    //          logical and
#define MPI_LAND  0
    
    //          bit-wise and
#define MPI_BAND  0
    
    //          logical or
#define MPI_LOR   0
    
    //          bit-wise or
#define MPI_BOR   0
    
    //          logical xor
#define MPI_LXOR  0
    
    //          bit-wise xor
#define MPI_BXOR  0
    
    //         max value and location
#define MPI_MAXLOC 0
    
    //         min value and location
#define MPI_MINLOC 0
    
#endif
    class MPITypeConstants {
    public:
        
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
        
    };
    
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/mpi/mpitypeconstants_test.h"
#endif


#endif
