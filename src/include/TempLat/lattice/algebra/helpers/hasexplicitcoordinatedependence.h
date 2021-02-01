#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEXPLICITCOORDINATEDEPENDENCE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEXPLICITCOORDINATEDEPENDENCE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <type_traits>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"

namespace TempLat {

    /** \brief Enables adapting
     *  the coordinates inside a possible shifted view on this
     *  object. This is very expensive, time-wasting, which is
     *  why we want to be able to detect such an explicit
     *  coordinate dependence, and skip the coordinate-shift
     *  computation when it is not used, saving loads of cpu time.
     *
     *  Since this only involves a compile-time boolean,
     *  we can grab it down the entire chain of nested
     *  getters and operators, and store the resulting value.
     *
     * Unit test: make test-hasexplicitcoordinatedependence
     **/
    
    /** \brief Default: no dependence. */
    template <class T, class S = void_t<>>
    struct HasExplicitCoordinateDependence : public std::false_type {
    };

    /** \brief Specialization: single object that mentions its explicit coordinate dependence. */
    template <class T>
    struct HasExplicitCoordinateDependence
    <T, void_t<decltype(T::EXPLICITCOORDINATEDEPENDENCE)>>
    {
        static constexpr bool value = T::EXPLICITCOORDINATEDEPENDENCE;
    };

    /** \brief Specialization: template object (Getter) whose enclosed class might mention its explicit coordinate dependence. */
    template <template <typename SS> class T, typename S>
    struct HasExplicitCoordinateDependence
    <T<S>, void_t<>>
    {
        static constexpr bool value = HasExplicitCoordinateDependence<S>::value;
    };

    /** \brief Specialization: template object (Getter) whose enclosed class might mention its explicit coordinate dependence. */
    template <template <typename SS, typename SSS> class T, typename S1, typename S2>
    struct HasExplicitCoordinateDependence
    <T<S1, S2>, void_t<>>
    {
        static constexpr bool value = HasExplicitCoordinateDependence<S1>::value || HasExplicitCoordinateDependence<S2>::value;
    };

    
    struct HasExplicitCoordinateDependenceTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
    
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasexplicitcoordinatedependence_test.h"
#endif


#endif
