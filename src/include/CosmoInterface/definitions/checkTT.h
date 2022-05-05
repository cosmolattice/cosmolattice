#ifndef TEMPLAT_COSMOINTERFACE_CHECKTT_H
#define TEMPLAT_COSMOINTERFACE_CHECKTT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros, Adrien Florio, Nicolás Layza,  Year: 2022

// #include "TempLat/lattice/field/field.h"
// #include "TempLat/lattice/algebra/algebra.h"
// #include "CosmoInterface/runparameters.h"
// #include "CosmoInterface/definitions/GWsProjectorCmplx.h"
// #include "CosmoInterface/definitions/GWsProjectorReal.h"
// #include "CosmoInterface/definitions/averages.h"
//
// #include "TempLat/util/rangeiteration/tagliteral.h"
// #include "TempLat/lattice/algebra/complexalgebra/complexalgebra.h"
// #include "TempLat/lattice/algebra/operators/power.h"
// #include "TempLat/lattice/algebra/operators/operators.h"
// #include "TempLat/lattice/algebra/spatialderivatives/forwdij.h"
// #include "TempLat/lattice/algebra/spatialderivatives/neutdij.h"
// #include "TempLat/lattice/field/collections/vectorfieldcollection.h"
//
// #include "TempLat/util/rangeiteration/make_list_tag.h"
// #include "TempLat/util/rangeiteration/sum_in_range.h"
//
//
// namespace TempLat {
//
//
//     /** \brief A class which computes the power spectrum, with the appropriate rescaling to make it volume independent.
//      *
//      *
//      **/
//
//      //MakeException(WrongPSType);
//
//
//       //So now I am using one (or maybe 2) extra memory sites to compute this,but it the requires much more inverse fourier transforms (12). A different possibility will be to use up to 3 extra field (one auxiliar, and one for the quantities of interest. This will reduce the number of FT to just 6.
//       class CheckTT{
//       public:
//         template<typename T>
//         CheckTT(const RunParameters<T>& par) :
//         PRJType(par.GWprojectorType)
//         {}
//
//         template<class Model, int I, int J, typename Q = double>
//         auto getProjectedField(Model& model, Tag<I> i, Tag<J> j) {
//             auto toolBox = model.getOneField().getToolBox();
//             Field<Q> AuxFld("AuxFld", toolBox);
//             if (PRJType == 1) {
//                 AuxFld.inFourierSpace() = GWsProjectorCmplx::LambdaUfld(model, i, j);
//                 AuxFld.inFourierSpace().setZeroMode(0);
//             }
//             else {
//                 AuxFld.inFourierSpace() = GWsProjectorReal::LambdaUfld(model, i, j);
//                 AuxFld.inFourierSpace().setZeroMode(0);
//             }
//             return AuxFld;
//         }
//
//
//         template<typename Model, int I>
//         auto checkTrans(Model& model, Tag<I> i) {
//
//             double avgGrad, avgDihij;
//
//             if(PRJType == 1){
//
//                 avgGrad = average(abs(backDiff(getProjectedField(model, i, 1_c) ,1_c) +  backDiff(getProjectedField(model, i, 2_c), 2_c) + backDiff(getProjectedField(model, i, 3_c), 3_c)));
//             }
//             else{
//                 avgGrad = 2 * average(abs(neutDiff(getProjectedField(model, i, 1_c),1_c)+neutDiff(getProjectedField(model, i, 2_c),2_c)+neutDiff(getProjectedField(model, i, 3_c),3_c)));
//             }
//
//             avgDihij = average(abs(forwDij(getProjectedField(model, i, 1_c) ,1_c) +  forwDij(getProjectedField(model, i, 2_c) ,2_c) + forwDij(getProjectedField(model, i, 3_c) ,3_c)));
//
//         	return avgGrad/avgDihij;
//
// 		}
//
// 		template<typename Model>
//         auto checkTrace(Model& model) {
//
//             auto Trhij = average(abs(getProjectedField(model, 1_c, 1_c) + getProjectedField(model, 2_c, 2_c) + getProjectedField(model, 3_c, 3_c)));
//             auto Trhij2 = average(abs(getProjectedField(model, 1_c, 1_c)) + abs(getProjectedField(model, 2_c, 2_c))  + abs(getProjectedField(model, 3_c,3_c)));
//
//             return Trhij/Trhij2;
// 		}
//
//
// 	 private:
//         /* Put all member variables and private methods here. These may change arbitrarily. */
//
//      int PRJType;
//
// 	};
//
//     class CheckTTTester {
//     public:
// #ifdef TEMPLATTEST
//         static inline void Test(TDDAssertion& tdd);
// #endif
//
//     };
//
//
// }
// #ifdef TEMPLATTEST
// #include "CosmoInterface/measurements/checkTT_test.h"
// #endif


#endif

/* FCN */
