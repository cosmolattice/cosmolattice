#ifndef TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWFOURIER_H
#define TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWFOURIER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/field/abstractfield.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"


namespace TempLat {

    MakeException(FieldViewFourierWrongSpaceConfirmation);

    /** \brief A view on the field which, when interacted with, assures every time again that things are in
     *   *fourier* space, and possibly the ghost cells are updated when needed.
     *
     *   No public constructor: only the friend class Field can instantiate this view on its own memory.
     *
     * Unit test: make test-fieldviewconfig
     **/


     template<typename T>
     class FourierView : public AbstractField<T> {
       /** \brief A simple class which provides a get method for basic types.
        * Field class
        *
        *
        *
        **/
     public:
         using AbstractField<T>::mManager;
         using AbstractField<T>::mToolBox;

         template<typename R>
         FourierView& operator=( R&& g)
         {
           int i=0;
           auto& it=mToolBox->itP();
           onBeforeAssignment(g);

           for(it.begin();it.end();++(it))
           {
                i=it();
               DoEval::eval(g,i);
               mManager->complex(i)=GetEval::getEval(g,i);
           }
           //mManager->setGhostsAreStale();
            return *this;
         }

         template<typename R>
         void onBeforeAssignment(R&& g) {
             /* likewise, make sure we are in configuration space (here the FFT may be fired!). */
             mManager->confirmFourierSpace();

             ConfirmSpace::apply(g,mToolBox->mLayouts.getFourierSpaceLayout(), SpaceStateInterface::SpaceType::Fourier);

             GhostsHunter::apply(g);

         }
         std::complex<T> get(ptrdiff_t i) const
         {
           return mManager->complex(i);
         }
         std::complex<T>& getSet(ptrdiff_t i)
         {
             return mManager->complex(i);
         }


         std::complex<T>&  get(const Looper& itK)
         {
           return   mManager->complex(itK());
         }

         const std::complex<T>&  get(const Looper& itK) const
         {
           return   mManager->complex(itK());
         }

         virtual const JumpsHolder& getJumps() const
         {
           return mToolBox->mLayouts.getFourierSpaceJumps();
         }

         inline
         void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
             switch (spaceType) {
                 case SpaceStateInterface::SpaceType::Configuration:
                     throw FieldViewFourierWrongSpaceConfirmation("FieldViewFourier explicitly only can be used in Fourier space. Do not transform to configuration space in place.");
                     break;
                 case SpaceStateInterface::SpaceType::Fourier:
                 default:
                     AbstractField<T>::confirmSpace(newLayout, spaceType);
                     break;
             }
         }

         std::string toString() const {
           return mManager->getName()+"(k)";
         }

         const auto& getLayout()
         {
           return mToolBox->mLayouts.getFourierSpaceLayout();
         }

         virtual Looper& getIt()
         {
           return (Looper&)mToolBox->itP();
         }

         /** \brief Getting a single entry from an array. Variadic because the number of dimensions is variable.
          *  Use for tests only, never for actual integrations and iterations.
          *  The arguments are in LOCAL SPACE!! Not global. Use true array indices, but not accounting for ghosts.
          *  At the same time, the arguments are in original dimension order, not transposed.
          *  Transposition will be applied internally where applicable.
          */
         template <typename... Args>
         auto& operator()(bool& test, Args... args) {
   //            const ptrdiff_t sz = sizeof...(args);
             std::vector<ptrdiff_t> asVec{{args...}};
             return operator()(test, asVec);
         }
         auto& operator()(bool& test, std::vector<ptrdiff_t> position)
          {
             ptrdiff_t offset = this->getOffsetFromCoords(test, position);
             /* our hack to give something that is not in the memory, without throwing an exception. */
             static std::complex<T> dummy = 0;
             dummy = std::complex<T>( std::numeric_limits<T>::infinity(), 0);
             return offset > -1 ? mManager->complex(offset) : dummy;
         }

         //MPI aware setting of value. Use exceptionnaly (remove zero mode for example)

         template <typename... Args>
         void setZeroMode(const std::complex<T>& toSet) { //This is dimension-aware.
             std::vector<ptrdiff_t> asVec(mToolBox->mNDimensions,0);
             set(toSet, asVec);
         }

         template <typename... Args>
         void set(const std::complex<T>& toSet, Args... args) {
             std::vector<ptrdiff_t> asVec{{args...}};
             set(toSet, asVec);
         }

         void set(const std::complex<T>& toSet, std::vector<ptrdiff_t> vec) {
             bool tmp;
             auto& res = (*this)(tmp, vec);
             if(tmp) res = toSet;
         }


         template<typename S>
         friend class Field;


       private:

         FourierView(const AbstractField<T>& f):
         AbstractField<T>(f)
         {

         }



     public:
#ifdef TEMPLATTEST
         static inline void Test(TDDAssertion& tdd);
#endif
     };

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/views/fieldviewfourier_test.h"
#endif


#endif
