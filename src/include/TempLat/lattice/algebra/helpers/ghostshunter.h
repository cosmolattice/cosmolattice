#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GHOSTSHUNTER_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GHOSTSHUNTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasdoweneedghosts.h"

namespace TempLat {



        /** \brief A class which helps implementing the automatic management of updating the ghost cells.
         *
         *
         * Unit test: make test-ghostshunter
         **/

        class GhostsHunter {
        public:

          template <typename U>
          static inline
          typename std::enable_if<HasDoWeNeedGhostsMethod<U>::value, void>::type
          apply( U& obj) {
              obj.doWeNeedGhosts();
          }

          template <typename U>
          static inline
          typename std::enable_if<!HasDoWeNeedGhostsMethod<U>::value, void>::type
          apply( U& obj) {
          }

          /* Put public methods here. These should change very little over time. */
          template <int N, typename U>
          static inline
          typename std::enable_if<HasDoWeNeedGhostsMethodIndexed<N,U>::value, void>::type
          apply( U& obj, Tag<N> i) {
              obj.doWeNeedGhosts(i);
          }

          template <int N, typename U>
          static inline
          typename std::enable_if<!HasDoWeNeedGhostsMethodIndexed<N,U>::value, void>::type
          apply( U& obj, Tag<N> i) {
          }


            /* Put public methods here. These should change very little over time. */
            template <typename U>
            static inline
            typename std::enable_if<HasDoWeNeedGhostsMethodIndexedDyn<U>::value, void>::type
            apply( U& obj, ptrdiff_t i) {
                obj.doWeNeedGhosts(i);
            }

            template <typename U>
            static inline
            typename std::enable_if<!HasDoWeNeedGhostsMethodIndexedDyn<U>::value, void>::type
            apply( U& obj, ptrdiff_t i) {
            }


        private:
          GhostsHunter() {

          }
            /* Put all member variables and private methods here. These may change arbitrarily. */



        public:
#ifdef TEMPLATTEST
            static inline void Test(TDDAssertion& tdd);
#endif
        };


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/ghostshunter_test.h"
#endif


#endif
