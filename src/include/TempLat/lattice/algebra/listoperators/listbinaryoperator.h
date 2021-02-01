#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_BINARYOPERATORVECTOR_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_BINARYOPERATORVECTOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/getvectorsize.h"
#include "TempLat/lattice/algebra/helpers/getjumps.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/confirmghosts.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"



namespace TempLat {

    /** \brief Wrapper for algebra with FieldCollection. Here we define the base methods for checking and everything.
     * Can use this algebra to implement matrix and vector algebra! IDEA: statically type MATRIXTYPE type and template
     * methods of the type AddMatrixTypes<R,T>::type.
     *
     * Unit test: make test-binaryoperator
     **/
    template<typename R, typename T>
    class ListBinaryOperator{
    public:
      ListBinaryOperator(const R& pR, const T& pT):
      mR(pR),
      mT(pT)
      {
      }

        template <int N>
        void confirmSpace(Tag<N> i, const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
          ConfirmSpace::apply(mR, i, newLayout, spaceType);
          ConfirmSpace::apply(mT, i, newLayout, spaceType);
      }
        template <int N>
        ptrdiff_t confirmGhostsUpToDate(Tag<N> i)
      {
        return ConfirmGhosts::apply(mR,i)+ConfirmGhosts::apply(mT,i);
      }
      inline JumpsHolder getJumps() { //Don't need indexing for get jumps.
          auto a = GetJumps::apply(mR);
          auto b = GetJumps::apply(mT);
          if ( a != b && !(a.isEmpty() || b.isEmpty())) throw DifferentJumpsHolderException("Two different memory layouts in binary operator"/*, toString()*/);
          return a.isEmpty() ? b : a;
      }

      /** \brief Override this method in your derived class, to have an easy implementation of your toString method. */
      virtual std::string operatorString() const {
          return " ";
      }

      /** \brief If your descending class implements `operatorString()` and your operator is of the type "a OP b" (where OP is * or whatever), this toString method does all the work for you. */
      template <int N>
      std::string toString(Tag<N> i) const {

          std::string tt = GetString::get(mR,i);

          if ( ContainsSpace::test(tt) ) tt = "(" + tt + ")";

          std::string ss = GetString::get(mT,i);

          if ( ContainsSpace::test(ss) ) ss = "(" + ss + ")";

          std::string result = tt + " " + operatorString() + " " + ss;

          return result;
      }
  /** For measurement objects: need the toolbox for easiest access to loopers and whatever else. */
      virtual inline
      std::shared_ptr<MemoryToolBox> getToolBox() { //no need for indexing here either
          auto a = GetToolBox::get(mR);
          auto b = GetToolBox::get(mT);
          return a.get() != NULL ? a : b;
      }

      using Getter = GetComponent;

    protected:
      R mR;
      T mT;



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* TempLat */

#//include "TempLat/lattice/algebra/operators/listoperators/listbinaryoperator.h"


#endif
