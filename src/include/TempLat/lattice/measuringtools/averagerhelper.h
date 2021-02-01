#ifndef TEMPLAT_LATTICE_MEASUREMENTS_AVERAGERHELPER_H
#define TEMPLAT_LATTICE_MEASUREMENTS_AVERAGERHELPER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"

namespace TempLat {


    /** \brief A class which
     * Average Routimes common to single field and list averager.
     * 
     * 
     * Unit test: make test-averagerhelper
     **/
    MakeException(AveragerWrongSpace);

    template<typename vType, bool isComplexValued>
    class AveragerHelper {
    public:
        /* Put public methods here. These should change very little over time. */
        AveragerHelper() {

        }
        template <typename T>
        static void onBeforeAverageFourier(T&& pT, SpaceStateInterface::SpaceType pSpaceType) {
            /* likewise, make sure we are in configuration space (here the FFT may be fired!). */
            if( pSpaceType != SpaceStateInterface::SpaceType::Fourier ) throw AveragerWrongSpace(" Called compute fourier space in averager specifying configuration layout. Abort.");

            ConfirmSpace::apply(pT,pT.getToolBox()->mLayouts.getFourierSpaceLayout(), pSpaceType);
            GhostsHunter::apply(pT);

        }
        template <typename T>
        static void onBeforeAverageConfiguration(T&& pT, SpaceStateInterface::SpaceType pSpaceType) {
            /* likewise, make sure we are in configuration space (here the FFT may be fired!). */
            if( pSpaceType != SpaceStateInterface::SpaceType::Configuration ) throw AveragerWrongSpace(" Called compute configuration space in averager specifying fourier layout. Abort.");
            ConfirmSpace::apply(pT,pT.getToolBox()->mLayouts.getConfigSpaceLayout(), pSpaceType);
            GhostsHunter::apply(pT);

        }


        static vType
        normalize(std::shared_ptr<MemoryToolBox> toolBox, SpaceStateInterface::SpaceType pSpaceType, vType & value) {

            const LayoutStruct& layout = pSpaceType == SpaceStateInterface::SpaceType::Fourier ?
                                         toolBox->mLayouts.getFourierSpaceLayout() :
                                         toolBox->mLayouts.getConfigSpaceLayout();

            auto accounting = layout.getHermitianPartners()->getNumberOfIndependentValues();

            return normalizeTypeSpecific(value, accounting);
        }



    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        template <bool forComplex = isComplexValued, typename std::enable_if<forComplex>::type* dummy = nullptr>
        static vType normalizeTypeSpecific(vType value, HermitianValueAccounting accounting) {

            double normRe = accounting.getRealValueCount()  ;
            double normIm = accounting.getImaginaryValueCount();

            return vType( value.real() / normRe, value.imag() / normIm);
        }

        template <bool forComplex = isComplexValued, typename std::enable_if<!forComplex>::type* dummy = nullptr>
        static vType normalizeTypeSpecific(vType value, HermitianValueAccounting accounting) {

            double normRe = accounting.getRealValueCount()  ;

            return value / normRe;
        }


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/averagerhelper_test.h"
#endif


#endif
