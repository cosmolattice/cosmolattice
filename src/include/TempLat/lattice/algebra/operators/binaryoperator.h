#ifndef TEMPLAT_LATTICE_ALGEBRA_BINARYOPERATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_BINARYOPERATOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/containsspace.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/getjumps.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/confirmghosts.h"

#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"

#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"

namespace TempLat {

    /** \brief A parent class which implements the common methods that all binary operators (*, +, -,... ) share.
     *  Inherits from SpaceStateInterface in order to make the confirm(Config/Fourier)Space methods
     *  accessible from any type of template instance.
     *
     * Unit test: make test-binaryoperator
     **/
    template<typename R, typename T>
    class BinaryOperator{
    public:


      BinaryOperator(const R& pR, const T& pT):
      mR(pR),
      mT(pT)
      {}

      virtual void doWeNeedGhosts()
      {
        GhostsHunter::apply(mR);
        GhostsHunter::apply(mT);
      }
      void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
          ConfirmSpace::apply(mR, newLayout, spaceType);
          ConfirmSpace::apply(mT, newLayout, spaceType);
      }
      ptrdiff_t confirmGhostsUpToDate()
      {
        return ConfirmGhosts::apply(mR) + ConfirmGhosts::apply(mT);
      }
      inline JumpsHolder getJumps() {
          auto a = GetJumps::apply(mR);
          auto b = GetJumps::apply(mT);
          if ( a != b && !(a.isEmpty() || b.isEmpty())) throw DifferentJumpsHolderException("Two different memory layouts in binary operator"/*, toString()*/);
          return a.isEmpty() ? b : a;
      }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
            DoEval::eval(mT, i);
        }

      inline auto getDx() const
      {
        return HasDx<R>::value ? GetDx::getDx(mR) : (HasDx<T>::value ? GetDx::getDx(mT) : 1);
      }

      inline auto getKIR() const
      {
        return HasKIR<R>::value ? GetKIR::getKIR(mR) : (HasKIR<T>::value ? GetKIR::getKIR(mT) : 1);
      }

      /** \brief Override this method in your derived class, to have an easy implementation of your toString method. */
      virtual std::string operatorString() const {
          return " ";
      }

      /** \brief If your descending class implements `operatorString()` and your operator is of the type "a OP b" (where OP is * or whatever), this toString method does all the work for you. */
      std::string toString() const {

          std::string tt = GetString::get(mR);

          if ( ContainsSpace::test(tt) ) tt = "(" + tt + ")";

          std::string ss = GetString::get(mT);

          if ( ContainsSpace::test(ss) ) ss = "(" + ss + ")";

          std::string result = tt + " " + operatorString() + " " + ss;

          return result;
      }
  /** For measurement objects: need the toolbox for easiest access to loopers and whatever else. */
      virtual inline
      std::shared_ptr<MemoryToolBox> getToolBox() {
          auto a = GetToolBox::get(mR);
          auto b = GetToolBox::get(mT);
          return a.get() != NULL ? a : b;
      }

    protected:
      R mR;
      T mT;
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/binaryoperator_test.h"
#endif


#endif
