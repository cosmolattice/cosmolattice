#ifndef COSMOINTERFACE_COUPLINGSMANAGER_H
#define COSMOINTERFACE_COUPLINGSMANAGER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/exception.h"


namespace TempLat {

    MakeException(NotEnoughChargesForThisCouplingsManager);
    MakeException(NotEnoughCouplingsForThisCouplingsManager);


    /** \brief A class which stores statically an array of booleans; useful to manage what couples to what.
     *
     * 
     * Unit test: make test-couplingsmanager
     **/

    // This class stores in its type an arbitrary number of booleans, which are used to define which field couple to which other field.
    // The best way to understand how this class work is to see how it is used in the GaugeDerivatives and MatterCurrent classes.
    template<int NMatter, int NGauge, bool... Bools>
    class CouplingsManager {
    public:
        /* Put public methods here. These should change very little over time. */
        CouplingsManager() = default;

        // In addition to simply knowing what couples to what, we also store the gauge field coupling g and the effective charge g*Q (Q being the matter charge).
        CouplingsManager(const std::vector<double>& charges, std::array<double,NGauge> couplings) :
        gs(couplings)
        {
            setEffectiveCharges(charges, couplings);
        }

        // This function stores the effective charges, but only between fields that actually are coupled.
        void setEffectiveCharges(const std::vector<double>& charges, std::vector<double> couplings)
        {
            if((charges.size() != howManyCouples()) && (couplings.size() != 0) ) throw( NotEnoughChargesForThisCouplingsManager(std::to_string(charges.size()) + " charges were specified while " + std::to_string(howManyCouples()) + " non-trivial doesCouples are expected. Abort.") );
            if(couplings.size() != NGauge && charges.size() != 0) throw( NotEnoughCouplingsForThisCouplingsManager(std::to_string(couplings.size()) + " couplings were specified while " + std::to_string(NGauge) + " gauge fields are present. Abort.") );
            size_t count = 0;
            for(size_t i = 0; i < doesCouples.size(); ++i){
                if(doesCouples[i])
                {
                    effectiveCharges[i] = charges[count] * couplings[i % NGauge];
                    ++count;
                }
                else effectiveCharges[i] = 0;

            }
        }

        // This function, accessible at compile time, tells how many fields are coupled in this CouplingManager.
        static constexpr size_t howManyCouples()
        {
            size_t c = 0;
            for(size_t i = 0 ; i< doesCouples.size(); ++i) c += doesCouples[i] ? 1 : 0;
            return c;
        }

        // The operator () is used to access the effective charge g*Q. Its first argument specifies the matter number of the matter field
        // and the second refers to the gauge field.
        template<int nmat, int ng>
        double operator()(Tag<nmat>,Tag<ng>) const {
            return effectiveCharges[nmat * NGauge + ng];
        }

        // We can also access only the gauge field coupling, in case needed.
        template<int ng>
        double coupling(Tag<ng>) const {
            return gs[ng];
        }

        // Accessible at compile time function which tells whether or not a given matter field and a given gauge field couples or not.
        template<int nmat, int ng>
         static constexpr bool couples(Tag<nmat>,Tag<ng>)
        {
            return doesCouples[nmat*NGauge + ng];
        }

    private:
        // Compile-time accessible booleans thich tells what couples to what.
        static constexpr std::array<bool, sizeof...(Bools)> doesCouples = {Bools...};
        std::array<double, sizeof...(Bools)> effectiveCharges;
        std::array<double, sizeof...(Bools)> gs;

    };

    //Pre c++17, need redundant out of line definition for static constexpr.
    template<int NMatter, int NGauge, bool... Bools>
    constexpr std::array<bool, sizeof...(Bools)> CouplingsManager<NMatter,NGauge, Bools...>::doesCouples;



    // Below is an empty class, which is needed to make the program compile when one or some of the coupling manager
    // is not used.
    template<>
    class CouplingsManager<0,0> {
    public:
        /* Put public methods here. These should change very little over time. */
        CouplingsManager() = default;

        CouplingsManager(const std::vector<double>& vec, std::array<double,0> couplings)
        {
        }

        void setEffectiveCharges(const std::vector<double>& vec, std::vector<double> couplings)
        {
        }

        static constexpr size_t howManyCouples()
        {
            return 0;
        }
        template<int nmat, int ng>
        double operator()(Tag<nmat>,Tag<ng>) const {
            return 0;
        }

        template<int ng>
        double coupling(Tag<ng>) const {
            return 0;
        }

        template<int nmat, int ng>
        static constexpr bool couples(Tag<nmat>,Tag<ng>)
        {
            return false;
        }

    private:
        static constexpr std::array<bool, 1> doesCouples = {false};

    };

    class CouplingsManagerTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/couplingsmanager_test.h"
#endif


#endif
