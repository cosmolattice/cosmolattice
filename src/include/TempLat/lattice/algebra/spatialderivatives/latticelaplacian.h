#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_LAPLACIANLOCAL_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_LAPLACIANLOCAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/shiftedaccessor.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/algebra/operators/operators.h"

namespace TempLat {



        /** \brief A class which implements the laplacian.
         *
         *
         * Unit test: make test-laplacianlocal
         **/



//"Simple" version

        template<ptrdiff_t nDimensions,typename R>
        class LatticeLaplacian : public UnaryOperator<R>{
        public:
        typedef typename GetGetReturnType<R>::type SV;
        typedef typename GetFloatType<SV>::type S;
          using UnaryOperator<R>::mR;
          LatticeLaplacian(R pR):UnaryOperator<R>(pR),dx2(pow(GetDx::getDx(pR),2)) {
            fixLaplacianMap(GetJumps::apply(mR));
          }

          void doWeNeedGhosts() override
          {
            mR.confirmGhostsUpToDate();
          }
          auto get(ptrdiff_t i)
          {
            auto result = -2*nDimensions * GetValue::get(mR,i);
            for(size_t j(0);j<2*nDimensions;++j) result+=GetValue::get(mR,i+mShiftAccessors[j]);
           // for(size_t j(0);j<2*nDimensions;++j) result+=GetValue::get(mShiftAccessors[j],i);
            return result / dx2;
          }
          auto operator()(R& mF, Looper& itX)
          {
            auto result = -2*nDimensions * mF(itX);
            //for(size_t j(0);j<2*nDimensions;++j) result+=mF.get(mShiftAccessors[j],itX());
            for(size_t j(0);j<2*nDimensions;++j) result+=mF.get(itX()+mShiftAccessors[j]);
            return result / dx2;
          }
          std::string operatorString() const override {
              return "Laplacian";
          }

            void eval(ptrdiff_t i) override
            {
                DoEval::eval(mR, i);
                for(size_t j=0; j<2*nDimensions;++j) DoEval::eval(mR, i + mShiftAccessors[j]);
            }

          /** \brief Symbolic derivatives. */
          template <typename S>
          inline
          auto d(const S& other) {
              return LatticeLaplacian<nDimensions,R>(GetDeriv::get(mR, other));
          }
        private:
            /* Put all member variables and private methods here. These may change arbitrarily. */
          //std::vector<ShiftedAccessor> mShiftAccessors;
          std::array<ptrdiff_t,2*nDimensions> mShiftAccessors;
          //std::vector<ExpressionShifter<0,R>> mShiftAccessors;
          S mCentralWeight;
          const S dx2;

          void fixLaplacianMap(const JumpsHolder& jump) {

                mCentralWeight = 0;
                //mShiftAccessors.clear();
                for ( ptrdiff_t dim = 0; dim < nDimensions; ++dim) {
                    mCentralWeight -= 2.;
                    //std::vector<ptrdiff_t> shifts(nDimensions, (ptrdiff_t) 0);
                    mShiftAccessors[2*dim]=whatJump(dim, 1, jump);
                    mShiftAccessors[2*dim+1]=whatJump(dim, -1, jump);
                   // shifts[dim] = 1;
                    //mShiftAccessors.push_back(shift(mR,shifts));
                    //shifts[dim] = -1;
                    //mShiftAccessors.push_back(shift(mR,shifts));
                  }
            }

            ptrdiff_t whatJump(ptrdiff_t dim, ptrdiff_t dSign, const JumpsHolder& jump) {
                std::vector<ptrdiff_t> shifts(nDimensions, (ptrdiff_t) 0);
                shifts[dim] = dSign;
                ShiftedAccessor tmp(jump,shifts);

                return tmp.getJump();
                //mShiftAccessors.push_back(tmp.getJump());
                //mShiftGetterMap.back().confirmSpace(newLayout, spaceType);

            }
          public:
#ifdef TEMPLATTEST
              static inline void Test(TDDAssertion& tdd);
#endif

        };

        template<typename R>
        class LatticeLaplacian<3,R> : public UnaryOperator<R>{
        public:
          using UnaryOperator<R>::mR;
          typedef typename GetGetReturnType<R>::type SV;
          typedef typename GetFloatType<SV>::type S;
          LatticeLaplacian(R pR):UnaryOperator<R>(pR),dx2(pow(GetDx::getDx(pR),2)) {
            //say<<pR;
            fixLaplacianMap(GetJumps::apply(mR));
          }

          auto get(ptrdiff_t i)
          {
            //say << "dx2" << dx2;
            auto result = -6.0 * GetValue::get(mR,i);
            for(size_t j(0);j<6;++j) result+=GetValue::get(mR,i+jumps[j]);
            return result / dx2;
          }
          void doWeNeedGhosts() override
          {
            mR.confirmGhostsUpToDate();
          }

            void eval(ptrdiff_t i) override
            {
                DoEval::eval(mR, i);
                for(size_t j=0; j<6;++j) DoEval::eval(mR, i + jumps[j]);
            }

          auto operator()(R& mF, Looper& itX)
          {
            auto result = -6.0 * mF.get(itX);
            for(size_t j(0);j<6;++j) result+=mF.get(itX()+jumps[j]);
            return result / dx2;
          }

          // Symbolic derivatives.
          template <typename S>
          inline
          auto d(const S& other) {
              return LatticeLaplacian<3,R>(GetDeriv::get(mR, other));
          }
        private:
          std::array<ptrdiff_t,6> jumps;
          const S dx2;

          //int jump0Up,jump0Down,jump1Up,jump1Down,jump2Up,jump2Down;
            // Put all member variables and private methods here. These may change arbitrarily.

          void fixLaplacianMap(const JumpsHolder& jump) {

                jumps[0]=whatJump(0, 1, jump);
                jumps[1]=whatJump(0, -1, jump);
                jumps[2]=whatJump(1, 1, jump);
                jumps[3]=whatJump(1, -1, jump);
                jumps[4]=whatJump(2, 1, jump);
                jumps[5]=whatJump(2, -1, jump);
            }

            ptrdiff_t whatJump(ptrdiff_t dim, ptrdiff_t dSign, const JumpsHolder& jump) {
                std::vector<ptrdiff_t> shifts(3, (ptrdiff_t) 0);
                shifts[dim] = dSign;
                ShiftedAccessor tmp(jump,shifts);

                return tmp.getJump();
                //mShiftGetterMap.back().confirmSpace(newLayout, spaceType);

            }

        };

        class LatticeLaplacianTester {
        public:
#ifdef TEMPLATTEST
          static inline void Test(TDDAssertion& tdd);
#endif

        };

        template<int nDimensions, typename R>
        typename std::enable_if< HasGetMethod<R>::value, LatticeLaplacian<nDimensions,R>>::type
        LatLapl(R pR)
        {
            return LatticeLaplacian<nDimensions,R>(pR);
        }

        template<int nDimensions=3, typename R>
        //typename std::enable_if< HasGetMethod<R>::value, LatticeLaplacian<nDimensions,R>>::type
        auto
        LLatLapl(R pR)
        {
            return (-6 * pR
            + shift<1,0,0>(pR)
            + shift<-1,0,0>(pR)
            + shift<0,1,0>(pR)
            + shift<0,-1,0>(pR)
            + shift<0,0,1>(pR)
            + shift<0,0,-1>(pR))/pow(GetDx::getDx(pR),2);


        }

    template<int nDimensions=3, typename R>
    //typename std::enable_if< HasGetMethod<R>::value, LatticeLaplacian<nDimensions,R>>::type
    auto
    LLLatLapl(R pR)
    {
        return (-6_c * pR
                + shift<1>(pR)
                + shift<-1>(pR)
                + shift<2>(pR)
                + shift<-2>(pR)
                + shift<3>(pR)
                + shift<-3>(pR))/std::pow(GetDx::getDx(pR),2);


    }


        //Might need a Laplacian tester struct

} /* TempLat */

#include "TempLat/lattice/field/field.h"
#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian_test.h"
#endif


#endif
