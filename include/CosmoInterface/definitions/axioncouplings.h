#ifndef COSMOINTERFACE_HELPERS_AXIONCOUPLINGS_H
#define COSMOINTERFACE_HELPERS_AXIONCOUPLINGS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Nicolas Loayza, Ander Urio, Adrien Florio,  Year: 2025

#include "CosmoInterface/couplingsmanager.h"

#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/staticif.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldshift.h"
#include "TempLat/lattice/algebra/su2algebra/su2shift.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletshift.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"

#include "TempLat/lattice/algebra/spatialderivatives/backdiff.h"
#include "TempLat/lattice/algebra/spatialderivatives/forwdiff.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
#include "TempLat/lattice/algebra/axionalgebra/magneticfield4.h"
#include "TempLat/lattice/algebra/axionalgebra/electricfield2.h"
#include "TempLat/lattice/algebra/constants/symbols.h"

namespace TempLat
{

  /** \brief A class which computes the matter currents and charge densities of the gauge fields
   *
   *
   *
   **/
  class AxionCouplings
  {
  public:
    /* Put public methods here. These should change very little over time. */
    AxionCouplings() = default;

    ////// Scalar AXION Source (used in the kernels of the EOM) //////

    // Primary signature with tMinust0
    template <class Model, int N>
    static auto ScalarAxionSource(Model &model, Tag<N> n, typename Model::FloatType tMinust0)
    {

      auto AxionCouplScalar =
          Total(a, 0, Model::NU1 - 1,
                IfElse(Model::ScalarU1AxionCouplings::couples(Tag<N>(), a),
                       model.alphaLambda_SU1(n, a) * Total(i, 1, Model::NDim,
                                                           electricField2(model.piU1(n), i) *
                                                               magneticField4(magneticField(model.fldU1(n), i), i)),
                       ZeroType()));

      const double NonLinearSwitch = (tMinust0 > model.tNonLinearAxionU1) ? 1.0 : 0.0;

      return AxionCouplScalar * NonLinearSwitch;
    }

    // Backward-compatible wrapper
    template <class Model, int N> static auto ScalarAxionSource(Model &model, Tag<N> n)
    {
      return ScalarAxionSource(model, n, 0.0);
    }

    ////// U(1) AXION COUPLING (used in the kernels of the EOM) //////

    // Primary signature with tMinust0
    template <class Model, int N, int I>
    static auto U1AxionCoupling(Model &model, Tag<N> n, Tag<I> i, typename Model::FloatType tMinust0)
    {

      auto AxionCoupl1 =
          -0.5 * Total(a, 0, Model::Ns - 1,
                       IfElse(Model::ScalarU1AxionCouplings::couples(a, Tag<N>()),
                              model.alphaLambda_SU1(a, n) *
                                  (model.piS(a) * magneticField4(magneticField(model.fldU1(n), i), i) +
                                   shift(model.piS(a) * magneticField4(magneticField(model.fldU1(n), i), i), i)),
                              ZeroType()));

      auto AxionCoupl2 =
          0.25 *
          Total(
              a, 0, Model::Ns - 1,
              IfElse(
                  Model::ScalarU1AxionCouplings::couples(a, Tag<N>()),
                  model.alphaLambda_SU1(a, n) *
                      (Total(j, 1, Model::NDim,
                             Total(k, 1, Model::NDim,
                                   Symbols::epsilon(i, j, k) *
                                       (shift(forwDiff(model.fldS(a), j) * shift(electricField2(model.piU1(n), k), j),
                                              i) +
                                        forwDiff(model.fldS(a), j) * shift(electricField2(model.piU1(n), k), j)))) +
                       Total(j, 1, Model::NDim,
                             Total(k, 1, Model::NDim,
                                   Symbols::epsilon(i, j, k) *
                                       (shift(backDiff(model.fldS(a), j) * shift(electricField2(model.piU1(n), k), -j),
                                              i) +
                                        backDiff(model.fldS(a), j) * shift(electricField2(model.piU1(n), k), -j))))),
                  ZeroType()));

      const double NonLinearSwitch = (tMinust0 > model.tNonLinearAxionU1) ? 1.0 : 0.0;

      return (pow(model.aI, model.alpha - 3) * AxionCoupl1 +
              pow(model.aI, -1 + model.alpha) * NonLinearSwitch * AxionCoupl2);
    }

    // Backward-compatible wrapper
    template <class Model, int N, int I> static auto U1AxionCoupling(Model &model, Tag<N> n, Tag<I> i)
    {
      return U1AxionCoupling(model, n, i, 0.0);
    }

    ////// Scalar Axion Source to Gauss Law //////

    template <class Model, int N> static auto ScalarAxionGaussLaw(Model &model, Tag<N> n)
    { // N is the Nth U1 abelian field, i is the spatial component.
      return Total(a, 0, Model::Ns - 1,
                   IfElse(Model::ScalarU1AxionCouplings::couples(a, Tag<N>()),
                          -(model.alphaLambda_SU1(a, n)) * (model.fStar / Model::MPl) *
                              Total(i, 1, Model::NDim,
                                    (forwDiff(model.fldS(a), i) *
                                     shift(magneticField4(magneticField(model.fldU1(n), i), i), i)) +
                                        (backDiff(model.fldS(a), i) *
                                         shift(magneticField4(magneticField(model.fldU1(n), i), i), -i))) *
                              0.5,
                          ZeroType()));
    }

    // Primary signature with tMinust0
    template <class Model, int N>
    static auto U1AxionCoupling(Model &model, Tag<N> t, typename Model::FloatType tMinust0)
    {
      return MakeVector(i, 1, Model::NDim, U1AxionCoupling(model, t, i, tMinust0));
    }

    // Backward-compatible wrapper
    template <class Model, int N> static auto U1AxionCoupling(Model &model, Tag<N> t)
    {
      return U1AxionCoupling(model, t, 0.0);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

} // namespace TempLat

#endif
