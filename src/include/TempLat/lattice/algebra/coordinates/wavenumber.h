#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/listoperators/vectordotter.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/util/rangeiteration/tag.h"
//#include "TempLat/lattice/algebra/vector.h"

namespace TempLat {

    /** \brief A class which allows for accessing (unscaled, dimensionless, index-valued) various
     *  expressions involving the fourier coordinates.
     *
     *
     * Unit test: make test-wavenumber
     **/


    class WaveNumber /*: public Vector*/ {
    public:
        /* Put public methods here. These should change very little over time. */
        WaveNumber( std::shared_ptr<MemoryToolBox> toolBox):
                mToolBox(toolBox) {

        }


        ptrdiff_t getVectorSize()
        {
            return mToolBox->mNDimensions;
        }

        auto vectorGet(ptrdiff_t i, ptrdiff_t j) //TODO: remove
        {
            return mToolBox->getCoordFourier(i)[j];
        }

        template <int N>
        auto operator()(Tag<N> t)
        {
            return getVectorComponent(*this, N-1);
        }

        auto operator[](const ptrdiff_t& i)
        {
            return getVectorComponent(*this, i);
        }
        auto norm2()
        {
            return dot(*this,*this);
        }
        auto norm()
        {
            return pow(this->norm2(),0.5);
        }

        std::string toString(ptrdiff_t j) const
        {
            return "k_" + std::to_string(j);
        }

        std::string toString() const
        {
            return "k";
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::shared_ptr<MemoryToolBox> mToolBox;

    };


    template <size_t  N>
    class FourierSite /*: public Vector*/ {
    public:
        /* Put public methods here. These should change very little over time. */
        FourierSite( std::shared_ptr<MemoryToolBox> toolBox):
        mToolBox(toolBox) {

        }


        ptrdiff_t getVectorSize()
        {
          return mToolBox->mNDimensions;
        }

        auto vectorGet(ptrdiff_t i, ptrdiff_t j) //TODO: remove
        {
          return mToolBox->getCoordFourier(i)[j];
        }

        template <int J>
        auto operator()(Tag<J> t)
        {
            return getVectorComponent(*this, J-1);
        }

        auto operator[](const ptrdiff_t& i)
        {
          return getVectorComponent(*this, i);
        }
        auto norm2()
        {
          return dot(*this,*this);
        }
        auto norm()
        {
          return pow(this->norm2(),0.5);
        }

        std::string toString(int i) const{
          return "k_" + std::to_string(i);
        };
        std::string toString() const{
          return "k";
        };

        static constexpr size_t size = N;

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::shared_ptr<MemoryToolBox> mToolBox;

    };


    struct WaveNumberTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

}
#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/coordinates/wavenumber_test.h"
#endif


#endif
