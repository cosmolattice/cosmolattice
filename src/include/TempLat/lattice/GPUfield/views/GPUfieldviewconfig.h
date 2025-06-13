#ifndef TEMPLAT_LATTICE_GPUFIELD_VIEWS_GPUFIELDVIEWCONFIG_H
#define TEMPLAT_LATTICE_GPUFIELD_VIEWS_GPUFIELDVIEWCONFIG_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/abstractfield.h"
#include "TempLat/lattice/GPUfield/views/abstractview.h"
#include "TempLat/lattice/GPUmemory/GPUmemoryblock.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"

#include "TempLat/parallel/kokkos/kokkos.h"

namespace TempLat {
    MakeException(GPUFieldViewConfigWrongSpaceConfirmation);

    /** \brief A view on the field which, when interacted with, assures every time again that things are in
     *   configuration space, and possibly the ghost cells are updated when needed.
     *   The final Field class defaults to config space, which means it inherits from this class.
     *
     * Unit test: make test-fieldviewconfig
     **/

    /** \brief A simple class which provides a get method for basic types.
     * Field class
     *
     **/
    template<typename CRTP, typename T>
    class GPUConfigView : public CRTP, public AbstractView<T> {
    public:
        /* Put public methods here. These should change very little over time. */
        //using NumberType = typename CRTP::NumberType;

        GPUConfigView(std::string name, const GPUMemoryBlock<T>& memory, const LatticeParameters<T> &pLatPar)
            : AbstractView<T>(name, memory, pLatPar), mDisableFFTBlocking(false) {
        }

        template<typename R>
        KOKKOS_FORCEINLINE_FUNCTION
        void assign(R &&g) {
            // TODO
            /*
            int i = 0;
            auto &it = mToolBox->itX();
            onBeforeAssignment(g);

            for (it.begin(); it.end(); ++(it)) {
                i = it();
                DoEval::eval(g, i);
                mManager->operator[](i) = GetEval::getEval(g, i);
            }
            mManager->setGhostsAreStale();
            */
        }

        template<typename R>
        KOKKOS_FORCEINLINE_FUNCTION
        void operator=(R &&g) {
            this->assign(std::forward<R>(g));
        }

        template<typename R>
        KOKKOS_FORCEINLINE_FUNCTION
        void operator+=(R &&g) {
            this->operator=(*this + g);
        }

        KOKKOS_FORCEINLINE_FUNCTION
        void operator=(const GPUConfigView<CRTP, T> &other) {
            this->assign(other);
        }

        T get(ptrdiff_t i) const {
            // TODO
            return 0;
        }

        T &getSet(ptrdiff_t i) {
            // TODO
            return 0;
        }

        KOKKOS_FORCEINLINE_FUNCTION
        const JumpsHolder &getJumps() const {
            /*
            return mToolBox->mLayouts.getConfigSpaceJumps();
            */
        }

        KOKKOS_FORCEINLINE_FUNCTION
        void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
            switch (spaceType) {
                case SpaceStateInterface::SpaceType::Fourier:
                    if (!mDisableFFTBlocking) throw GPUFieldViewConfigWrongSpaceConfirmation(
                        "GPUFieldViewConfig explicitly only can be used in configuration space. Do not transform to Fourier space in place. Or, if you know what you are doing and you are not doing multiple in-place FFT's on your integration data, you can call Field<T>::setDisableFFTBlocking() to disable this block, and enable going from configuration to Fourier space.");
                    break;
                case SpaceStateInterface::SpaceType::Configuration:
                default:
                    AbstractField<T>::confirmSpace(newLayout, spaceType);
                    break;
            }
        }

        KOKKOS_FORCEINLINE_FUNCTION
        const auto &getLayout() {
            return 0;
            //TODO
            //return mToolBox->mLayouts.getConfigSpaceLayout();
        }

        KOKKOS_FORCEINLINE_FUNCTION
        Looper &getIt() {
            // TODO
            //return (Looper &) mToolBox->itX();
        }

        KOKKOS_FORCEINLINE_FUNCTION
        void updateGhosts() {
            // TODO
            //this->mManager->updateGhosts();
        }

        T &get(const Looper &itX) {
            // TODO
            //return mManager->operator[](itX());
            return 0;
        }

        T get(const Looper &itX) const {
            // TODO
            //return mManager->operator[](itX());
            return 0;
        }

        std::string toString() const {
            return "";
            //return mManager->getName() + "(x)";
        }

        /** \brief Disable the blocking of going from configuration space to fourier space.
         *  Used by PowerSpectrumBuilder, which uses newly allocate memory which is filled in configuration space,
         *  and then FFT'ed to fourier space.
         */
        void setDisableFFTBlocking() {
            mDisableFFTBlocking = true;
        }

        template<typename... Args>
        auto &operator()(bool &test, Args... args) {
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
        auto &operator()(bool &test, std::vector<ptrdiff_t> position) {
            /*
            ptrdiff_t offset = this->getOffsetFromCoords(test, position);
            // our hack to give something that is not in the memory, without throwing an exception.
            static T dummy = 0;
            dummy = std::numeric_limits<T>::infinity();
            return offset > -1 ? mManager->operator[](offset) : dummy;*/
            // TODO
            return 0;
        }

        template<typename R>
        void onBeforeAssignment(R &&g) {
            /*
            // likewise, make sure we are in configuration space (here the FFT may be fired!).
            mManager->confirmConfigSpace();

            ConfirmSpace::apply(g, mToolBox->mLayouts.getConfigSpaceLayout(),
                                SpaceStateInterface::SpaceType::Configuration);

            GhostsHunter::apply(g);
            */
        }

        //MPI aware seetting of value. Use exceptionnaly (remove zero mode for example)
        template<typename... Args>
        void set(const T &toSet, Args... args) {
            std::vector<ptrdiff_t> asVec{{args...}};
            set(toSet, asVec);
        }

        void set(const T &toSet, std::vector<ptrdiff_t> vec) {
            bool tmp;
            auto &res = (*this)(tmp, vec);
            if (tmp) res = toSet;
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
#include "TempLat/lattice/GPUfield/views/GPUfieldviewconfig_test.h"
#endif


#endif
