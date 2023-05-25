#ifndef COSMOINTERFACE_HELPERS_GWSPROJECTOR_H
#define COSMOINTERFACE_HELPERS_GWSPROJECTOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros, Adrien Florio, Nicolás Layza,  Year: 2022

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/util/constants.h"
#include "TempLat/lattice/algebra/algebra.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/lattice/algebra/complexalgebra/asfourier.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/definitions/averages.h"

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/complexalgebra/complexalgebra.h"
#include "TempLat/lattice/algebra/gaugealgebra/fieldstrength.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"

#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"

namespace TempLat {

    /** \brief A class computes the projected product of h_ij h_ij^* in base of the u fields. See Technical Note II.
     *
     *
     * Unit test: make test-GWsProjector
     **/

    class GWsProjector {
    public:
        /* Put public methods here. These should change very little over time. */
        GWsProjector(int PRJType):
        mType(PRJType)
        {
        }


    public:

		/* Function that computes the Projector in base of the choice of lattice momenta given by the forward derivative of eqn 68 in the Art arXiv:2006.15122 */
		template<class Model, class Looper, int I, int J, typename T = double>
        inline auto Pr(Model& model, Looper& it, Tag<I> i, Tag<J> j) {

            auto pVec = it.getVec();

            size_t N = GetNGrid::get(model.getOneField());

            // Note that we force the vector to have 3 components instead of Model::NDim so the code compiles for any NDim. The GW module only works for NDim=3. If NDim != 3, the code does not run and returns an error.
            auto klattice = MakeVector(l, 1, 3,
                                       mType == 1 ? std::sin(2 * Constants::pi<T> * pVec[l-1] / N) :
                                       mType == 2 ? std::complex<T>(1) - std::complex<T>(std::cos(2.0 * Constants::pi<T> / N * pVec[l-1]),std::sin(2.0 * Constants::pi<T> / N * pVec[l-1])) :
                                       mType == 3 ? std::complex<T>(1) - std::complex<T>(std::cos(2.0 * Constants::pi<T> / N * pVec[l-1]),std::sin(-2.0 * Constants::pi<T> / N * pVec[l-1])) : std::complex<T>(1.));

            T klatticeSquare = Total(k, 1, 3, abs((klattice(k))*conj(klattice(k))));

            if(i == j) return (std::complex<T>)(1) - conj(klattice(i)) * (klattice(j)) / klatticeSquare;
            return - conj(klattice(i)) * (klattice(j)) / klatticeSquare;


//         return dx;
        }

//         template<int I, int J, class Model, class Looper>
//         static  auto Projh(Tag<I> i, Tag<J> j, Model& model, Looper& it) {
//             return Total(k, 1, 3, Pr(model, it, i, j) *  GetEval::getEval(model.pi_GWtensor(j,k).inFourierSpace(), it()));
//         }

        template<class Model, class Looper>
        auto projectedGW_complex(Model& model, Looper& it) {

            auto P11 = Pr(model, it, 1_c, 1_c);
            auto P12 = Pr(model, it, 1_c, 2_c);
            auto P13 = Pr(model, it, 1_c, 3_c);
            auto P21 = conj(P12);
            auto P22 = Pr(model, it, 2_c, 2_c);
            auto P23 = Pr(model, it, 2_c, 3_c);
            auto P31 = conj(P13);
            auto P32 = conj(P23);
            auto P33 = Pr(model, it, 3_c, 3_c);

            auto u11 = GetValue::get(model.pi_GWtensor(1_c,1_c).inFourierSpace(), it());
            auto u12 = GetValue::get(model.pi_GWtensor(1_c,2_c).inFourierSpace(), it());
            auto u13 = GetValue::get(model.pi_GWtensor(1_c,3_c).inFourierSpace(), it());
            auto u21 = u12;
            auto u22 = GetValue::get(model.pi_GWtensor(2_c,2_c).inFourierSpace(), it());
            auto u23 = GetValue::get(model.pi_GWtensor(2_c,3_c).inFourierSpace(), it());
            auto u31 = u13;
            auto u32 = u23;
            auto u33 = GetValue::get(model.pi_GWtensor(3_c,3_c).inFourierSpace(), it());

            auto Pu11 = P11 * u11 + P12 * u21 + P13 * u31;
            auto Pu12 = P11 * u12 + P12 * u22 + P13 * u32;
            auto Pu13 = P11 * u13 + P12 * u23 + P13 * u33;
            auto Pu21 = P21 * u11 + P22 * u12 + P23 * u13;
            auto Pu22 = P21 * u12 + P22 * u22 + P23 * u23;
            auto Pu23 = P21 * u13 + P22 * u23 + P23 * u33;
            auto Pu31 = P31 * u11 + P32 * u21 + P33 * u13;
            auto Pu32 = P31 * u12 + P32 * u22 + P33 * u23;
            auto Pu33 = P31 * u13 + P32 * u23 + P33 * u33;

            auto Pu_s11 = P11 * conj(u11) + P12 * conj(u21) + P13 * conj(u31);
            auto Pu_s12 = P11 * conj(u12) + P12 * conj(u22) + P13 * conj(u32);
            auto Pu_s13 = P11 * conj(u13) + P12 * conj(u23) + P13 * conj(u33);
            auto Pu_s21 = P21 * conj(u11) + P22 * conj(u12) + P23 * conj(u13);
            auto Pu_s22 = P21 * conj(u12) + P22 * conj(u22) + P23 * conj(u23);
            auto Pu_s23 = P21 * conj(u13) + P22 * conj(u23) + P23 * conj(u33);
            auto Pu_s31 = P31 * conj(u11) + P32 * conj(u21) + P33 * conj(u13);
            auto Pu_s32 = P31 * conj(u12) + P32 * conj(u22) + P33 * conj(u23);
            auto Pu_s33 = P31 * conj(u13) + P32 * conj(u23) + P33 * conj(u33);

            auto Tr1 = (Pu11 * Pu_s11 + Pu12 * Pu_s21 + Pu13 * Pu_s31) + (Pu21 * Pu_s12 + Pu22 * Pu_s22 + Pu23 * Pu_s32) + (Pu31 * Pu_s13 + Pu32 * Pu_s23 + Pu33 * Pu_s33);
            auto Tr2a = (Pu11 + Pu22 + Pu33);
            auto Tr2b = (Pu_s11 + Pu_s22 + Pu_s33);

            return pow(model.aI,2*(model.alpha-3)) * abs(Tr1 - .5 * Tr2a * Tr2b);
        }

        template<class Model, class Looper>
        auto projectedGW_real(Model& model, Looper& it) {

            auto P11 = Pr(model, it, 1_c, 1_c);
            auto P12 = Pr(model, it, 1_c, 2_c);
            auto P13 = Pr(model, it, 1_c, 3_c);
            auto P21 = conj(P12);
            auto P22 = Pr(model, it, 2_c, 2_c);
            auto P23 = Pr(model, it, 2_c, 3_c);
            auto P31 = conj(P13);
            auto P32 = conj(P23);
            auto P33 = Pr(model, it, 3_c, 3_c);

            auto u11 = GetEval::getEval(model.pi_GWtensor(1_c,1_c).inFourierSpace(), it());
            auto u12 = GetEval::getEval(model.pi_GWtensor(1_c,2_c).inFourierSpace(), it());
            auto u13 = GetEval::getEval(model.pi_GWtensor(1_c,3_c).inFourierSpace(), it());
            auto u21 = u12;
            auto u22 = GetEval::getEval(model.pi_GWtensor(2_c,2_c).inFourierSpace(), it());
            auto u23 = GetEval::getEval(model.pi_GWtensor(2_c,3_c).inFourierSpace(), it());
            auto u31 = u13;
            auto u32 = u23;
            auto u33 = GetEval::getEval(model.pi_GWtensor(3_c,3_c).inFourierSpace(), it());

            auto Pu11 = P11 * u11 + P12 * u21 + P13 * u31;
            auto Pu12 = P11 * u12 + P12 * u22 + P13 * u32;
            auto Pu13 = P11 * u13 + P12 * u23 + P13 * u33;
            auto Pu21 = P21 * u11 + P22 * u12 + P23 * u13;
            auto Pu22 = P21 * u12 + P22 * u22 + P23 * u23;
            auto Pu23 = P21 * u13 + P22 * u23 + P23 * u33;
            auto Pu31 = P31 * u11 + P32 * u21 + P33 * u13;
            auto Pu32 = P31 * u12 + P32 * u22 + P33 * u23;
            auto Pu33 = P31 * u13 + P32 * u23 + P33 * u33;

            auto Tr1 = (Pu11 * conj(Pu11) + Pu12 * conj(Pu21) + Pu13 * conj(Pu31)) + (Pu21 * conj(Pu12) + Pu22 * conj(Pu22) + Pu23 * conj(Pu32)) + (Pu31 * conj(Pu13) + Pu32 * conj(Pu23) + Pu33 * conj(Pu33));
            auto Tr2 = (Pu11 + Pu22 + Pu33);

            return pow(model.aI,2*(model.alpha-3)) * abs(Tr1 - .5 * Tr2 * conj(Tr2));
        }

        template<class Model, class Looper>
        auto getProjectionatpoint(Model& model, Looper& it) {
            if(mType == 0) return projectedGW_real(model, it);
            else return projectedGW_complex(model, it);
            return 0.;
        }

    private:
        int mType;
	};

    class GWsProjectorTester {
    public:
#ifdef TEMPLATTEST
        static inline  void Test(TDDAssertion& tdd);
#endif

    };


} /* FCN */

#ifdef TEMPLATTEST
#include "CosmoInterface/definitions/GWsProjector_test.h"
#endif


#endif
