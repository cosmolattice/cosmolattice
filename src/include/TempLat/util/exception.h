#ifndef TEMPLAT_UTIL_EXCEPTION_H
#define TEMPLAT_UTIL_EXCEPTION_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/log/colors.h"
#include "TempLat/util/debug/stacktrace.h"

namespace TempLat {

    
    /** \brief Recursion stopper for helper function for unpacking the variadic arguments of the Exception constructor. */
    inline void ExceptionArgumentUnpacker( std::stringstream& stream) {
        /* stop the recursion. */
    }

    /** \brief Helper function for unpacking the variadic arguments of the Exception constructor. */
    template <typename T, typename... Args>
    inline void ExceptionArgumentUnpacker( std::stringstream& stream, T t, Args... args) {
        stream << t << " ";
        ExceptionArgumentUnpacker(stream, args...);
    }

    
    /** \brief An exception which takes variadic arguments, all recorded into an error message.
     *
     * Unit test: make test-exception
     */
    
    class Exception : public std::exception {
        public:
        /* Put public methods here. These should change very little over time. */
        Exception(const char* itheWhat) : theWhat(itheWhat), strtrace(Stacktrace()) {
            //    std::cerr << "Constructing exception: " << theWhat << "\n" << trace() << "\n";
            theStringWhat = KRED + std::string(itheWhat) + KRESET;
        };
        template <typename... Args>
        Exception(Args... args) :
        theWhat(NULL),
        strtrace(Stacktrace())
        {
            std::stringstream stream;
            ExceptionArgumentUnpacker(stream, args...);
            theStringWhat = KRED + stream.str() + KRESET;
        };
        const char* what() const noexcept
        {
            std::cerr << strtrace << "\n";
            return theStringWhat.length() ? theStringWhat.c_str() : theWhat;
        }
        const std::string& trace() const noexcept { return strtrace; }
            
            
        private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        const char* theWhat;
        std::string theStringWhat;
        const std::string strtrace;
            
            
            
        public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#define MakeException(name) class name : public TempLat::Exception {\
public:\
  name(const char* iitheWhat) : TempLat::Exception(iitheWhat) { };\
  template <typename... Args> name(Args... args) : TempLat::Exception(args...) { };\
}


#ifdef TEMPLATTEST
#include "TempLat/util/exception_test.h"
#endif
            
            
#endif
