#ifndef COSMOINTERFACE_HELPERS_COMPOSITEFIELDS_H
#define COSMOINTERFACE_HELPERS_COMPOSITEFIELDS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/lattice/algebra/operators/operators.h"
#include "CosmoInterface/definitions/gaugederivatives.h"
#include "TempLat/lattice/algebra/gaugealgebra/forwardcovariantderivative.h"
#include "TempLat/lattice/algebra/su2algebra/su2multiply.h"
#include "TempLat/lattice/algebra/gaugealgebra/fieldstrength.h"
#include "TempLat/lattice/algebra/gaugealgebra/plaquette.h"
#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/spatialderivatives/normgradientsquare.h"
#include "TempLat/lattice/algebra/axionalgebra/electricfield2.h"
#include "TempLat/lattice/algebra/axionalgebra/magneticfield4.h"
#include "TempLat/lattice/algebra/gaugealgebra/magneticfield.h"

namespace TempLat
{
  /** @brief A class which stores expressions of field functionals.
   * By "field functional" we refer to any function of the field variables such as quadratic forms, e.g. <phi^2>, and
   * others.
   *
   * The methods of this class compute the field functionals that appear in the total energy density,
   * but without the corresponding multiplying factors. For example, when we compute <Pi^2> for a scalar singlet,
   * only in energies.h we multiply it by 1/2*a^(-6) to get the corresponding kinetic energy.
   *
   **/
  class FieldFunctionals
  {
  public:
    // Put public methods here. These should change very little over time.

    FieldFunctionals() = delete;

    // --> Scalar singlet:

    /** @brief Returns the square of the gradient of the scalar field, i.e. <D_i[f]^2> (sum over i). Note that for a
     * scalar singlet, the covariant derivative reduces to the ordinary gradient, so we can use the norm of the gradient
     * squared directly.
     *
     * @param model The model from which to extract the field.
     * @param i The tag corresponding to the scalar field in the model.
     */
    template <class Model, int I> // <Grad[f]^2>
    static inline auto grad2S(Model &model, Tag<I> i)
    {
      return Grad2(model.fldS(i));
    }

    /** @brief Returns the square of the canonical momentum of the scalar field, i.e. <pi^2>.
     *
     * @param model The model from which to extract the field.
     * @param i The tag corresponding to the scalar field in the model.
     */
    template <class Model, int I> // <pi^2>
    static inline auto pi2S(Model &model, Tag<I> i)
    {
      return pow<2>(model.piS(i));
    }

    // --> Complex scalar:

    /**
     * @brief  Returns the square of the covariant gradient of the complex scalar field, i.e. <D_i[f]^2> (sum over i).
     * Note that for a complex scalar, the covariant derivative includes the gauge field.
     *
     * Uses the forward covariant gradient.
     *
     * @param model The model from which to extract the field and the gauge field.
     * @param i The tag corresponding to the complex scalar field in the model.
     */
    template <class Model, int I> // <D_i[f]^2> (sum over i)
    static inline auto grad2CS(Model &model, Tag<I> i)
    {
      return Total(j, 1, Model::NDim, norm2(GaugeDerivatives::forwardCovGradientCS(model, i, j)));
    }

    /**
     * @brief  Returns the square of the canonical momentum of the complex scalar field, i.e. <pi^2>.
     *
     * @param model The model from which to extract the field.
     * @param i The tag corresponding to the complex scalar field in the model.
     */
    template <class Model, int I> // <pi^2>
    static inline auto pi2CS(Model &model, Tag<I> i)
    {
      return norm2(model.piCS(i));
    }

    // --> SU2 doublet:

    /**
     * @brief  Returns the square of the covariant gradient of the SU2 doublet field, i.e. <D_i[f]^2> (sum over i).
     * Note that for a SU2 doublet, the covariant derivative includes the gauge field.
     *
     * @param model The model from which to extract the field and the gauge field.
     * @param i The tag corresponding to the SU2 doublet field in the model.
     */
    template <class Model, int I> // <D_i[f]^2> (sum over i)
    static inline auto grad2SU2Doublet(Model &model, Tag<I> i)
    {
      return Total(j, 1, Model::NDim, norm2(GaugeDerivatives::forwardCovGradientSU2Doublet(model, i, j)));
    }

    /**
     * @brief  Returns the square of the canonical momentum of the SU2 doublet field, i.e. <pi^2>.
     *
     * @param model The model from which to extract the field.
     * @param i The tag corresponding to the SU2 doublet field in the model.
     */
    template <class Model, int I> // <pi^2>
    static inline auto pi2SU2Doublet(Model &model, Tag<I> i)
    {
      return norm2(model.piSU2Doublet(i));
    }

    // --> U1 gauge sector:

    /**
     * @brief  Returns the square of the field strength tensor of the U1 gauge field, i.e. <F_{ij}^2> (sum over i,j).
     *
     * @param model The model from which to extract the field.
     * @param a The tag corresponding to the U1 gauge field in the model.
     */
    template <class Model, int A>
    static inline auto
    B2U1(Model &model,
         Tag<A> a) // In 3D, returns F_{21}^2 + F_{31}^2 + F_{32}^2 (necessary to compute the magnetic energy)
    {
      return Total(
          i, 1, Model::NDim,
          Total(j, 1, Model::NDim, IfElse(IsLess(j, i), pow<2>(fieldStrength(model.fldU1(a), i, j)), ZeroType());));
    }

    /**
     * @brief  Returns the square of the canonical momentum of the U1 gauge field, i.e. <pi^2>.
     *
     * @param model The model from which to extract the field.
     * @param a The tag corresponding to the U1 gauge field in the model.
     */
    template <class Model, int A> // <pi^2>
    static inline auto pi2U1(Model &model, Tag<A> a)
    {
      return Total(i, 1, Model::NDim, pow<2>(model.piU1(a)(i)));
    }

    /**
     * @brief Returns the product of E and B fields for the U1 gauge field, which gives the topological charge density.
     *
     * @param model The model from which to extract the field.
     * @param n The tag corresponding to the U1 gauge field in the model
     */
    template <class Model, int N> static inline auto EBU1(Model &model, Tag<N> n)
    {
      return Total(i, 1, Model::NDim,
                   electricField2(model.piU1(n), i) * magneticField4(magneticField(model.fldU1(n), i), i));
    }

    // --> SU2 gauge sector:

    /**
     * @brief  Returns "momentum" of the SU(2) gauge fields' EOM (a bit different because evolving the links). Also note
     * that our algebra variables are antihermitean, so we are really saving i E.
     *
     * @param model The model from which to extract the field.
     * @param n The tag corresponding to the SU2 gauge field in the model.
     * @param i The tag corresponding to the spatial direction.
     */
    template <class Model, int N, int DIR> static auto get_SU2_electric(Model &model, Tag<N> n, Tag<DIR> i)
    {
      return model.piSU2(n)(i);
    }

    /**
     * @brief Returns the electric field for the SU2 gauge field, as a vector with the components in each spatial
     * direction.
     *
     * @param model The model from which to extract the field.
     * @param n The tag corresponding to the SU2 gauge field.
     */
    template <class Model, int N> static auto get_SU2_electric(Model &model, Tag<N> n)
    {
      return MakeVector(i, 1, Model::NDim, get_SU2_electric(model, n, i));
    }

    /**
     * @brief Returns the square of the field strength tensor of the SU2 gauge field, i.e. <F_{ij}^2> (sum over i,j).
     * This is equivalent to the square of the magnetic field, and we compute it from the plaquette.
     *
     * @param model The model from which to extract the field.
     * @param a The tag corresponding to the SU2 gauge field in the model.
     */
    template <class Model, int A> static inline auto B2SU2(Model &model, Tag<A> a)
    {
      return 4.0 / (pow<4>(model.dx) * pow<2>(model.gQ_SU2DblSU2(0_c, a))) *
             Total(i, 1, Model::NDim,
                   Total(j, 1, Model::NDim,
                         IfElse(IsLess(j, i), 2.0 - trace(plaq(model.fldSU2(a), i, j)), // if
                                ZeroType()                                              // else
                         );));
    }

    /**
     * @brief This is a bit of a misnommer. We return the quantity that is related to the eletric field by powers of
     * a(t).
     *
     * @param model The model from which to extract the field.
     * @param a The tag corresponding to the SU2 gauge field in the model.
     */
    template <class Model, int A> // <pi^2>
    static inline auto pi2SU2(Model &model, Tag<A> a)
    {
      auto El = get_SU2_electric(model, a);
      return Total(i, 1, Model::NDim, Total(b, 1, 3, 4 * pow<2>(El(i)(b))));
    }

    /**
     * @brief Returns the product of E and B fields for the SU2 gauge field, which gives the topological charge density.
     *
     * @param model The model from which to extract the field.
     * @param n The tag corresponding to the SU2 gauge field in the model
     */
    template <class Model, int N> // <pi^2>
    static inline auto TrEBSU2(Model &model, Tag<N> n)
    {
      auto El = get_SU2_electric(model, n);
      // Once again, the 4 comes from the fact that usually E^a is defined with respect to sigma_a/2 in the algebra. (a)
      // returns the expansion with respect to sigma_a.
      return 4 * Total(i, 1, Model::NDim, Total(a, 1, 3, El(i)(a) * model.getCloverSU2()(n)(i)(a)));
    }
  };
} // namespace TempLat

#endif
