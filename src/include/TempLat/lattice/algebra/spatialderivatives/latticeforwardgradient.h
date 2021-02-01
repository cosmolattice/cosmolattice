#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/shiftedaccessor.h"
#include "TempLat/lattice/algebra/helpers/getjumps.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"


namespace TempLat {

        /** \brief A class which implements the forward gradient.
         *
         *
         * Unit test: make test-forwardgradientlocal
         **/

    template<int nDimensions, typename R>
    class LatticeForwardGradient {
    public:
        typedef typename GetGetReturnType<R>::type SV;
        typedef typename GetFloatType<SV>::type S;
        /* Put public methods here. These should change very little over time. */
        LatticeForwardGradient(const R& pR):
        mR(pR),
        dx(GetDx::getDx(mR))
        {
          fixGradientMap(GetJumps::apply(mR));
        }

        template<typename T>
        T operator()(Looper& pIt, const ptrdiff_t& dir) {
            return (mR(pIt()+jumps[dir])-mR(pIt)) / dx;
        }

        auto vectorGet(ptrdiff_t i, ptrdiff_t j) {
            return (mR.get(i+jumps[j])-mR.get(i)) / dx;
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
            for(size_t j=0; j<nDimensions;++j) DoEval::eval(mR, i + jumps[j]);
        }

       /* auto get(ptrdiff_t i)
        {
            return pow<2>( mR.get(i+jumps[0])-mR.get(i) ) + pow<2>( mR.get(i+jumps[1])-mR.get(i) ) + pow<2>( mR.get(i+jumps[2])-mR.get(i) );
        }*/

        ptrdiff_t getVectorSize()
        {
          return nDimensions;
        }
        auto norm2()
        {
          return dot(*this,*this);
        }

        auto norm()
        {
          return pow(this->norm2(),0.5);
        }

        auto operator[](const ptrdiff_t& i)
        {
          return getVectorComponent(*this, i);
        }

        auto norm2( Looper &pIt) {
            double res=0;
            for(size_t i=0; i<nDimensions;++i) res+=std::pow((*this).vectorGet(pIt(),i),2);
            return res;
        }
        auto norm(Looper &pIt) {
            return std::pow(this->norm2(pIt),0.5);
        }

        std::string toString() const{
            return "Grad(" + GetString::get(mR)+")";
        }

        void doWeNeedGhosts(ptrdiff_t i)
        {
          mR.confirmGhostsUpToDate();
        }

        void doWeNeedGhosts()
        {
          mR.confirmGhostsUpToDate();
        }

      inline auto getDx() const
      {
        return  dx;
      }

      inline auto getKIR() const
      {
        return GetKIR::getKIR(mR);
      }

        void confirmSpace(ptrdiff_t i, const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
            ConfirmSpace::apply(mR, i, newLayout, spaceType);
        }
        ptrdiff_t confirmGhostsUpToDate(ptrdiff_t i)
        {
          return ConfirmGhosts::apply(mR,i);
        }
        inline JumpsHolder getJumps() { //Don't need indexing for get jumps.
            return GetJumps::apply(mR);
        }


        /** For measurement objects: need the toolbox for easiest access to loopers and whatever else. */
        virtual inline
        std::shared_ptr<MemoryToolBox> getToolBox() {
            return GetToolBox::get(mR);
        }

        template <typename S>
        inline
        auto d(const S& other) {
            return LatticeForwardGradient<nDimensions,R>(GetDeriv::get(mR, other));
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        R mR;
        const S dx;
        std::array<ptrdiff_t,nDimensions> jumps;

        void fixGradientMap(const JumpsHolder& jump) {
              for ( ptrdiff_t dim = 0; dim < nDimensions; ++dim) {
                  jumps[dim]=pushBack(dim, 1, jump);
              }
        }

        ptrdiff_t pushBack(ptrdiff_t dim, ptrdiff_t dSign, const JumpsHolder& jump) {
            std::vector<ptrdiff_t> shifts(nDimensions, (ptrdiff_t) 0);
            shifts[dim] = dSign;

            return ShiftedAccessor(jump,shifts).getJump();
        }

    };

    class LatticeForwardGradientTester {
    public:
#ifdef TEMPLATTEST
      static inline void Test(TDDAssertion& tdd);
#endif

    };

    template<int nDimensions=3, typename R>
    LatticeForwardGradient<nDimensions,R> LatForwardGrad(R pR)
    {
        return LatticeForwardGradient<nDimensions,R>(pR);
    }


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/spatialderivatives/latticeforwardgradient_test.h"
#endif


#endif
