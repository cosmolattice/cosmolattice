#ifndef TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWCONFIG_H
#define TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWCONFIG_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/abstractfield.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"

namespace TempLat {

    MakeException(FieldViewConfigWrongSpaceConfirmation);

    /** \brief A view on the field which, when interacted with, assures every time again that things are in
     *   configuration space, and possibly the ghost cells are updated when needed.
     *   The final Field class defaults to config space, which means it inherits from this class.
     *
     * Unit test: make test-fieldviewconfig
     **/


     template<typename T>
     class ConfigView : public AbstractField<T> {
       /** \brief A simple class which provides a get method for basic types.
        * Field class
        *
        *
        *
        **/
     public:

         using AbstractField<T>::mManager;
         using AbstractField<T>::mToolBox;

         /* Put public methods here. These should change very little over time. */
         ConfigView(std::string name, std::shared_ptr<MemoryToolBox> toolBox, LatticeParameters<T> pLatPar ):
         AbstractField<T>(name,toolBox,pLatPar),
         mDisableFFTBlocking(false)
         {
             mManager->setGhostsAreStale();
             mManager->confirmConfigSpace(); //allocation happens here
             //mManager->allocate();
         }


         template<typename R>
         void assign( R&& g)
         {
             int i=0;
             auto& it=mToolBox->itX();
             onBeforeAssignment(g);

             for(it.begin();it.end();++(it))
             {
                 i=it();
                 DoEval::eval(g,i);
                 mManager->operator[](i) = GetEval::getEval(g,i);
             }
             mManager->setGhostsAreStale();
         }


         template<typename R>
         void operator=( R&& g)
         {
            this->assign(std::forward<R>(g));
         }


         template<typename R>
         void operator+=( R&& g)
         {
           this->operator=(*this+g);
         }


         void operator=(const ConfigView<T>& other) { //overwrite the default = operator.
             this->assign(other);
         }

         T get(ptrdiff_t i) const
         {
           return mManager->operator[](i);
         }

         T& getSet(ptrdiff_t i)
         {
             return mManager->operator[](i);
         }

         virtual const JumpsHolder& getJumps() const
         {
           return mToolBox->mLayouts.getConfigSpaceJumps();
         }

         inline void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType){
           switch (spaceType) {
               case SpaceStateInterface::SpaceType::Fourier:
                   if ( !mDisableFFTBlocking) throw FieldViewConfigWrongSpaceConfirmation("FieldViewConfig explicitly only can be used in configuration space. Do not transform to Fourier space in place. Or, if you know what you are doing and you are not doing multiple in-place FFT's on your integration data, you can call Field<T>::setDisableFFTBlocking() to disable this block, and enable going from configuration to Fourier space.");
                   break;
               case SpaceStateInterface::SpaceType::Configuration:
               default:
                   AbstractField<T>::confirmSpace(newLayout, spaceType);
                   break;
           }
         }

         const auto& getLayout()
         {
           return mToolBox->mLayouts.getConfigSpaceLayout();
         }

         virtual Looper& getIt()
         {
           return (Looper&)mToolBox->itX();
         }

         void updateGhosts()
         {
           this->mManager->updateGhosts();
         }

         T&  get(const Looper& itX)
         {
           return   mManager->operator[](itX());
         }


         T  get(const Looper& itX) const
         {
           return   mManager->operator[](itX());
         }


       std::string toString() const {
         return mManager->getName()+"(x)";
       }

         /** \brief Disable the blocking of going from configuration space to fourier space.
          *  Used by PowerSpectrumBuilder, which uses newly allocate memory which is filled in configuration space,
          *  and then FFT'ed to fourier space.
          */
         void setDisableFFTBlocking() {
             mDisableFFTBlocking = true;
         }


         template <typename... Args>
         auto& operator()(bool& test, Args... args) {
   //            const ptrdiff_t sz = sizeof...(args);
             std::vector<ptrdiff_t> asVec{{args...}};
             return operator()(test, asVec);
         }
         /** \brief Getting a single entry from an array. Variadic because the number of dimensions is variable.
          *  Use for tests only, never for actual integrations and iterations.
          *  The arguments are in LOCAL SPACE!! Not global. Use true array indices, but not accounting for ghosts.
          *  At the same time, the arguments are in original dimension order, not transposed.
          *  Transposition will be applied internally where applicable.
          */
         auto& operator()(bool& test, std::vector<ptrdiff_t> position)
          {
             ptrdiff_t offset = this->getOffsetFromCoords(test, position);
             /* our hack to give something that is not in the memory, without throwing an exception. */
             static T dummy = 0;
             dummy = std::numeric_limits<T>::infinity();
             return offset > -1 ? mManager->operator[](offset) : dummy;
         }

         template<typename R>
         void onBeforeAssignment(R&& g) {
             /* likewise, make sure we are in configuration space (here the FFT may be fired!). */
             mManager->confirmConfigSpace();

             ConfirmSpace::apply(g,mToolBox->mLayouts.getConfigSpaceLayout(), SpaceStateInterface::SpaceType::Configuration);

             GhostsHunter::apply(g);

         }
         //MPI aware seetting of value. Use exceptionnaly (remove zero mode for example)
         template <typename... Args>
         void set(const T& toSet, Args... args) {
             std::vector<ptrdiff_t> asVec{{args...}};
             set(toSet, asVec);
         }

         void set(const T& toSet, std::vector<ptrdiff_t> vec) {
             bool tmp;
             auto& res = (*this)(tmp, vec);
             if(tmp) res = toSet;
         }


     private:

       bool mDisableFFTBlocking;


     public:
#ifdef TEMPLATTEST
         static inline void Test(TDDAssertion& tdd);
#endif
     };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/views/fieldviewconfig_test.h"
#endif


#endif
