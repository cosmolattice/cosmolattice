#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_VECTORDOTTER_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_VECTORDOTTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"

namespace TempLat {

    /** \brief A class which takes two VectorGetters, and implements a
     *  special get method which returns the dot-product / contraction of
     *  the two vectors.
     *
     * Unit test: make test-vectordotter
     **/
     template <typename R, typename T>
     class VectorDotter : public BinaryOperator<R,T>{
     public:
       using BinaryOperator<R,T>::mR;
       using BinaryOperator<R,T>::mT;
         VectorDotter(R& a, T& b): BinaryOperator<R,T>(a,b) {
             measureVectorSize();
         }

         /** \brief Getter for two instances: return type automatically determined by the type which we get by multiplying one element of T with one element of S. */
         inline auto get(ptrdiff_t i) {
             //say<<mVectorSize<<" "<<mR.getVectorSize();
             decltype(mR.vectorGet(i, 0) * mT.vectorGet(i, 0) ) result = 0;
             /* sorry, an if-statement inside a getter function: if T and S are the same thing, let's not call its getter twice (it might be an expensive algebraic operation. */
             if ( (void*) &mR == (void*) &mT ) {
                 for ( ptrdiff_t j = 0, jEnd = mVectorSize; j < jEnd; ++j) {
                     auto a = mR.vectorGet(i,j);
                     result += a * a;
                 }
             } else {
                 for ( ptrdiff_t j = 0, jEnd = mVectorSize; j < jEnd; ++j) {
                     result += mR.vectorGet(i,j) * mT.vectorGet(i,j);
                 }
             }
             //say<<mVectorSize;
             return result;
         }
         void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
             BinaryOperator<R,T>::confirmSpace(newLayout, spaceType);
             measureVectorSize();
         }
         virtual std::string operatorString() const {
             return ".";
         }

     private:
         /* Put all member variables and private methods here. These may change arbitrarily. */
         ptrdiff_t mVectorSize;

         void measureVectorSize() {
             /* get new dimensionality */
             mVectorSize = std::min( mT.getVectorSize(), mR.getVectorSize() );
         }

     };


     template<typename R, typename T>
     VectorDotter<R,T> dot(R& r, T& t)
     {

       return VectorDotter<R,T>(r,t);
     }

     template<typename R>
     typename std::enable_if<HasVectorGetMethod<R>::value, VectorDotter<R,R>>::type
      norm2(R r)
     {
       return VectorDotter<R,R>(r,r);
     }

     template<typename R>
     auto norm(R r)
     {
       return pow(norm2(r),0.5);
     }

    class VectorDotterTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };




}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/vectordotter_test.h"
#endif


#endif
