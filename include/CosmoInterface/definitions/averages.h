#ifndef COSMOINTERFACE_HELPERS_AVERAGES_H
#define COSMOINTERFACE_HELPERS_AVERAGES_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/definitions/fieldfunctionals.h"
#include "CosmoInterface/definitions/potential.h"

namespace TempLat
{
  /** @brief A class used to compute common averages.
   *
   *
   **/
  class Averages
  {
  public:
    // Put public methods here. These should change very little over time.
    Averages() = delete;

    // --> Scalar singlet:

    template <class Model> static inline auto grad2S(Model &model) // <Grad[f]^2> (sum over fields)
    {
      return average(Total(i, 0, Model::Ns - 1, FieldFunctionals::grad2S(model, i)));
    }

    template <class Model> static inline auto pi2S(Model &model) // <pi^2 [f]> (sum over fields)
    {
      return average(Total(i, 0, Model::Ns - 1, FieldFunctionals::pi2S(model, i)));
    }

    // --> Complex scalars:

    template <class Model> static inline auto grad2CS(Model &model) // <D_i[f]^2> (sum over i, fields)
    {
      return average(Total(a, 0, Model::NCs - 1, FieldFunctionals::grad2CS(model, a)));
    }

    template <class Model> static inline auto pi2CS(Model &model) // <pi^2 [f]>  (sum over fields)
    {
      return average(Total(i, 0, Model::NCs - 1, FieldFunctionals::pi2CS(model, i)));
    }

    // --> SU2 doublets:

    template <class Model> static inline auto grad2SU2Doublet(Model &model) // <D_i[f]^2> (sum over i, fields)
    {
      return average(Total(a, 0, Model::NSU2Doublet - 1, FieldFunctionals::grad2SU2Doublet(model, a)));
    }

    template <class Model> static inline auto pi2SU2Doublet(Model &model) // <pi^2 [f]>  (sum over fields)
    {
      return average(Total(i, 0, Model::NSU2Doublet - 1, FieldFunctionals::pi2SU2Doublet(model, i)));
    }

    // --> U1 gauge sector:

    template <class Model> static inline auto B2U1(Model &model)
    {
      return average(Total(a, 0, Model::NU1 - 1, FieldFunctionals::B2U1(model, a)));
    }

    template <class Model> static inline auto pi2U1(Model &model) // <pi^2 [f]>  (sum over fields)
    {
      return average(Total(a, 0, Model::NU1 - 1, FieldFunctionals::pi2U1(model, a)));
    }

    // --> SU2 gauge sector:

    template <class Model> static inline auto B2SU2(Model &model)
    {
      return average(Total(a, 0, Model::NSU2 - 1, FieldFunctionals::B2SU2(model, a)));
    }

    template <class Model> static inline auto pi2SU2(Model &model) // <pi^2 [f]>  (sum over fields)
    {
      return average(Total(a, 0, Model::NSU2 - 1, FieldFunctionals::pi2SU2(model, a)));
    }

    // Function to compute all averages. Useful at initialization and to restart simulation from disk.
    template <typename Model> static inline void setAllAverages(Model &model)
    {
      model.potAvI = average(Potential::potential(model));
      model.potAvSI = average(Potential::potential(model));

      if constexpr (Model::Ns > 0) {
        model.pi2AvI = Averages::pi2S(model);
        model.pi2AvSI = model.pi2AvI;
        model.grad2AvI = Averages::grad2S(model);
      }
      if constexpr (Model::NU1 > 0) {
        model.U1pi2AvI = Averages::pi2U1(model);
        model.U1pi2AvSI = model.U1pi2AvI;
        model.U1Mag2AvI = Averages::B2U1(model);
      }
      if constexpr (Model::NSU2 > 0) {
        model.SU2pi2AvI = Averages::pi2SU2(model);
        model.SU2pi2AvSI = model.SU2pi2AvI;
        model.SU2Mag2AvI = Averages::B2SU2(model);
      }

      if constexpr (Model::NCs > 0) {
        model.CSpi2AvI = Averages::pi2CS(model);
        model.CSpi2AvSI = model.CSpi2AvI;
        model.CSgrad2AvI = Averages::grad2CS(model);
      }
      if constexpr (Model::NSU2Doublet > 0) {
        model.SU2DblPi2AvI = Averages::pi2SU2Doublet(model);
        model.SU2DblPi2AvSI = model.SU2DblPi2AvI;
        model.SU2DblGrad2AvI = Averages::grad2SU2Doublet(model);
      }

      if constexpr (Model::IsNonMinimallyCoupled) {
        ForLoop(i, 0, Model::Ns - 1, model.fld2AvSI_i(i) = average(pow<2>(model.fldS(i)));
                model.grad2AvSI_i(i) = average(FieldFunctionals::grad2S(model, i));
                model.pi2AvSI_i(i) = average(FieldFunctionals::pi2S(model, i));

                model.grad2AvSI_i(i) = 0.5 * pow<-2>(model.aI) * model.grad2AvSI_i(i);
                model.pi2AvSI_i(i) = 0.5 * pow<-6>(model.aI) * model.pi2AvSI_i(i);

                model.fldPiAvSI(i) = average(model.fldS(i) * model.piS(i));
                model.fldVpAvSI(i) = average(model.fldS(i) * Potential::derivS(model, i)););
      }
    }
  };
} // namespace TempLat

#endif
