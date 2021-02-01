#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2LIEALGEBRAFIELD_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2LIEALGEBRAFIELD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2021

#include "TempLat/lattice/algebra/su2algebra/su2field.h"


namespace TempLat {


    /** \brief A class which implements a SU2 field (algebra). Same than the group, except set 0 component to 0 and has a function which
     * returns its components wrt to sigma/2 instead of sigma, which is more conventional for the algebra (but internally, also expanded as
     * a function of sigma).
     *
     * 
     * Unit test: make test-su2liealgebrafield
     **/

    template<typename T>
    class SU2LieAlgebraField : public SU2FieldBase<T>
    {
    public:

        using SU2FieldBase<T>::fs;

        /* Put public methods here. These should change very little over time. */
        SU2LieAlgebraField(Field<T> f1, Field<T> f2, Field<T> f3):
        SU2FieldBase<T>(f1,f2,f3)
        {

        }

        SU2LieAlgebraField(std::string name, std::shared_ptr<MemoryToolBox> toolBox, LatticeParameters<T> pLatPar):
        SU2FieldBase<T>(name, toolBox, pLatPar)
        {

        }

        template <int N>
        auto SU2Get(Tag<N> t)
        {
            return (*this)(t);
        }

        template <int N>
        auto SU2LieAlgebraGet(Tag<N> t)
        {
            return 2 * (*this)(t);
        }

        ZeroType operator()(Tag<0> t)
        {
            return ZeroType();
        }

        template<int M>
        auto& operator()(Tag<M> t)
        {
            return fs[M-1];
        }


        auto SU2Get(Tag<0> t, ptrdiff_t i)
        {
            return 0.0;
        }
        template<int M>
        auto SU2Get(Tag<M> t, ptrdiff_t i)
        {
            return fs[M-1].get(i);
        }


        template <typename R>
        void operator=(R&& r)
        {
            SU2FieldBase<T>::operator=(r);
        }

        std::string toString() const{
            return SU2FieldBase<T>::toString();
        }


        inline auto getDx() const
        {
            return SU2FieldBase<T>::getDx();
        }

        inline auto getKIR() const
        {
            return SU2FieldBase<T>::getKIR();
        }

        using Getter = SU2Getter;
        static constexpr size_t SHIFTIND = 0;
        static constexpr size_t size = 4;
        static constexpr size_t numberToSkipAsTuple = 1;

    private:

        std::array<T,4> SU2Get(ptrdiff_t i) //This function is private, as it is designed only for internal use and more importantly, it does not return the correct 0th component,
        {                                   // for optmisation purposes.
            return SU2FieldBase<T>::SU2Get(i);
        }


    };


    struct SU2LieAlgebraFieldTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/su2algebra/su2liealgebrafield_test.h"
#endif


#endif
