#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_KERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_KERNELS_H

#include "CosmoInterface/evolvers/kernels/scalarsingletkernels.h"
#include "CosmoInterface/evolvers/kernels/gwskernels.h"
#include "CosmoInterface/evolvers/kernels/scalefactorkernels.h"
#include "CosmoInterface/evolvers/kernels/complexscalarkernels.h"
#include "CosmoInterface/evolvers/kernels/su2doubletkernels.h"
#include "CosmoInterface/evolvers/kernels/u1kernels.h"
#include "CosmoInterface/evolvers/kernels/su2kernels.h"

#include "CosmoInterface/fieldsnumbering.h"



namespace TempLat::Kernels {



        template<class Model, class KernelType, int N>
        static inline auto get(FieldsNumbering::fldS, Model& model, Tag<N> n, KernelType kt)
        {
            return ScalarSingletKernels::get_momentum(model, n, kt);
        }


        template<class Model, class KernelType, int N>
        static inline auto get(FieldsNumbering::piS, Model& model, Tag<N> n, KernelType kt)
        {
            return ScalarSingletKernels::get(model, n, kt);
        }


        template<class Model, class KernelType, int N>
        static inline auto get(FieldsNumbering::fldCS, Model& model, Tag<N> n, KernelType kt)
        {
            return ComplexScalarKernels::get_momentum(model, n, kt);
        }

        template<class Model, class KernelType, int N>
        static inline auto get(FieldsNumbering::piCS, Model& model, Tag<N> n, KernelType kt)
        {
            return ComplexScalarKernels::get(model, n, kt);
        }




        template<class Model, class KernelType, int N>
        static inline auto get(FieldsNumbering::fldSU2Doublet, Model& model, Tag<N> n, KernelType kt)
        {
            return SU2DoubletKernels::get_momentum(model, n, kt);
        }

        template<class Model, class KernelType, int N>
        static inline auto get(FieldsNumbering::piSU2Doublet, Model& model, Tag<N> n, KernelType kt)
        {
            return SU2DoubletKernels::get(model, n, kt);
        }


        template<class Model, class KernelType, int N>
        static inline auto get(FieldsNumbering::fldU1, Model& model, Tag<N> n, KernelType kt)
        {
            return U1Kernels::get_momentum(model, n, kt);
        }

        template<class Model, class KernelType, int N>
        static inline auto get(FieldsNumbering::piU1, Model& model, Tag<N> n, KernelType kt)
        {
            return U1Kernels::get(model, n, kt);
        }

        template<class Model, class KernelType, int N>
        static inline auto get(FieldsNumbering::fldSU2, Model& model, Tag<N> n, KernelType kt)
        {
            return SU2Kernels::get_momentum(model, n, kt);
        }

        template<class Model, class KernelType, int N>
        static inline auto get(FieldsNumbering::piSU2, Model& model, Tag<N> n, KernelType kt)
        {
            return SU2Kernels::get(model, n, kt);
        }
} // namespace TempLat::Kernels

#endif
