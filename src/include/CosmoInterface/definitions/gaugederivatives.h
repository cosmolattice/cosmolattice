#ifndef COSMOINTERFACE_HELPERS_GAUGEDERIVATIVES_H
#define COSMOINTERFACE_HELPERS_GAUGEDERIVATIVES_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/staticif.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldshift.h"
#include "TempLat/lattice/algebra/su2algebra/su2shift.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletshift.h"
#include "TempLat/lattice/algebra/su2algebra/su2wrapper.h"
#include "TempLat/lattice/algebra/su2algebra/su2generators.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/lattice/algebra/listoperators/foldmultiply.h"

namespace TempLat {

    /** \brief A class which computes spatial scalar and (covariant) gauge derivatives
     *
     **/

    MakeException(MoreThanOneDoublet);


    class GaugeDerivatives {
    public:
        /* Put public methods here. These should change very little over time. */
        GaugeDerivatives() = delete;


        // Laplacian and gauge covariant Laplacian

        template<class Model, int N>
        static auto LaplacianS(Model& model, Tag<N> n)
        {
            return (1.0 /pow<2>(model.dx)) * Total(i,1,Model::NDim, shift(model.fldS(n),i) + shift(model.fldS(n),-i) - 2 * model.fldS(n) );
        }

        template<class Model, int N>
        static auto covLaplacianCS(Model& model, Tag<N> n)
        {
            auto covPlus = Total(i,1,Model::NDim, U1sForCSCovDerivs(model, n, i) * shift(model.fldCS(n),i));
            auto covMinus = Total(i,1,Model::NDim, shift(conj(U1sForCSCovDerivs(model, n, i)) * model.fldCS(n),-i));
            return (covPlus + covMinus - (2.0 * Model::NDim) * model.fldCS(n)) / pow<2>(model.dx);
        }

        template<class Model, int N>
        static auto covLaplacianSU2Doublet(Model& model, Tag<N> n)
        {
            auto covPlus = Total(i,1,Model::NDim, SU2sForSU2DoubletCovDerivs(model, n, i) * (U1sForSU2DoubletCovDerivs(model, n, i) * shift(model.fldSU2Doublet(n),i)));
            auto covMinus = Total(i,1,Model::NDim, shift(dagger(SU2sForSU2DoubletCovDerivs(model, n, i)) * (conj(U1sForSU2DoubletCovDerivs(model, n, i)) * model.fldSU2Doublet(n)),-i));
            return (covPlus + covMinus - (2.0 * Model::NDim) * model.fldSU2Doublet(n)) / pow<2>(model.dx);
        }


        // Forward gradients and forward covariant gradients

        template<class Model, int N, int I>
        static auto forwardGradient(Model& model, Tag<N> n,  Tag<I> i)
        {
            return (shift<I>(model.fldS(n)) - model.fldS(n)) / model.dx;
        }

        template<class Model, int N, int I>
        static auto forwardCovGradientCS(Model& model, Tag<N> n,  Tag<I> i)
        {
            return (U1sForCSCovDerivs(model, n, i) * shift<I>(model.fldCS(n)) - model.fldCS(n)) / model.dx;
        }

        template<class Model, int N, int I>
        static auto forwardCovGradientSU2Doublet(Model& model, Tag<N> n,  Tag<I> i)
        {
            return (SU2sForSU2DoubletCovDerivs(model, n, i) * (U1sForSU2DoubletCovDerivs(model, n, i) * shift<I>(model.fldSU2Doublet(n))) - model.fldSU2Doublet(n)) / model.dx;
        }

        // Functions to compute the correct link contributions to the covariant derivatives,
        // depending on to which matter fields couples to which gauge fields.

        template<class Model, int N, int I>
        static auto U1sForCSCovDerivs(Model& model, Tag<N> n, Tag<I> i) // N is the matter index and i is the spatial dimension
        {

            return fold_multiply(
                    MakeArray(a,0,Model::NU1 - 1,
                            IfElse(Model::CsU1Couplings::couples(Tag<N>(),a),
                                     complexPhase(- model. dx * model.gQ_CsU1(n, a) * model.fldU1(a)(i)), // if this CS couples to this U1, then U1 link
                                    OneType()))); // else, 1
        }

        template<class Model, int N, int I>
        static auto U1sForSU2DoubletCovDerivs(Model& model, Tag<N> n, Tag<I> i)  // N is the matter index and i is the spatial dimension
        {
            return fold_multiply(MakeArray(a,0,Model::NU1 - 1,
                    IfElse(Model::SU2DoubletU1Couplings::couples(Tag<N>(),a),
                            complexPhase(-model.dx * model.gQ_SU2DblU1(n, a) * model.fldU1(a)(i)), // if this SU2 doublet couples to this U1, then U1 link
                            OneType()))); // else, 1
        }

        template<class Model, int N, int I>
        static auto SU2sForSU2DoubletCovDerivs(Model& model, Tag<N> t, Tag<I> i)  // N is the matter index and i is the spatial dimension
        {
            if(Model::NSU2 > 0 && N>0) throw MoreThanOneDoublet("As implemented now, the equations are only consistent for one doublet in the presence of SU2 gauge fields (or several but the same charge). The reason is that one needs to correct for the charge inside the link, as in the U1 case. Abort.");
            return fold_multiply(MakeArray(a,0,Model::NSU2 - 1,
                    IfElse(Model::SU2DoubletSU2Couplings::couples(Tag<N>(),a),
                            model.fldSU2(a)(i), // if this SU2 doublet couples to this SU2, then SU2
                            OneType()))); // else, 1
        }



       // "Vectorial" version of some of the above functions

        template<class Model, int N>
        static auto forwardGradient(Model& model, Tag<N> n)
        {
            return MakeVector(i,1,Model::NDim, forwardGradient(model,n,i));
        }

        template<class Model, int N>
        static auto forwardCovGradientSU2Doublet(Model& model, Tag<N> n)
        {
            return MakeVector(i,1,Model::NDim, forwardCovGradientSU2Doublet(model,n,i));
        }

        template<class Model, int N>
        static auto forwardCovGradientCS(Model& model, Tag<N> n)
        {
            return MakeVector(i,1,Model::NDim, forwardCovGradientCS(model,n,i));
        }


        template<class Model, int N>
        static auto U1sForCSCovDerivs(Model& model, Tag<N> n) {
            return MakeVector(i, 1, Model::NDim, U1sForCSCovDerivs(model,n, i));
        }

        template<class Model, int N>
        static auto U1sForSU2DoubletCovDerivs(Model& model, Tag<N> t)
        {
            return MakeVector(i, 1, Model::NDim, U1sForSU2DoubletCovDerivs(model,t, i));
        }

        template<class Model, int N>
        static auto SU2sForSU2DoubletCovDerivs(Model& model, Tag<N> t)
        {
            return MakeVector(i, 1, Model::NDim, SU2sForSU2DoubletCovDerivs(model,t, i));
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/definitions/gaugederivatives_test.h"
#endif


#endif
