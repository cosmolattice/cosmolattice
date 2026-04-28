#ifndef COSMOINTERFACE_DEFINITIONS_GWSPROJECTORTYPE1_H
#define COSMOINTERFACE_DEFINITIONS_GWSPROJECTORTYPE1_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
 *   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
 *   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros,  Year: 2026

#include "TempLat/util/constants.h"

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"

namespace TempLat
{
  /** @brief A class which computed the energy of the GWs, using the type 1 (neutral) TT projector.
   *
   * Unit test: ctest -R test-symmetrictracelessfieldmultiply
   **/

  template <typename R, typename T, typename Model> class GWProjector : public TempLat::BinaryOperator<R, T>
  {
  public:
    // Put public methods here. These should change very little over time.
    using TempLat::BinaryOperator<R, T>::mR;
    using TempLat::BinaryOperator<R, T>::mT;
    using S = Model::FloatType;

    GWProjector(const R &pR, const T &pT, const Model &model)
        : TempLat::BinaryOperator<R, T>(pR, pT), kIR(static_cast<S>(2. * Constants::pi<S> / GetNGrid::get(model)))
    {
    }

    static consteval size_t getNDim() { return Model::NDim; }

    virtual std::string operatorString() const override { return "TT_Projected_Energy"; }

    S kIR;
  };

  template <typename R, typename T, typename Model> class GWProjectorType1 : public GWProjector<R, T, Model>
  {
  public:
    using S = typename GWProjector<R, T, Model>::S;

    GWProjectorType1(const R &pR, const T &pT, const Model &model)
        : GWProjector<R, T, Model>::GWProjector(pR, pT, model)
    {
    }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      auto u = DoEval::eval(this->mR, idx...);
      auto k = DoEval::eval(this->mT, idx...);

      device::array<S, 3> kL;
      S kL2;
      device::array<S, 6> P;
      device::array<complex<S>, 9> Pu;

      constexpr_for<0, 3>([&](auto i) { kL[i] = sin(this->kIR * static_cast<S>(k[i])); });
      kL2 = kL[0] * kL[0] + kL[1] * kL[1] + kL[2] * kL[2];

      P[0] = S(1) - kL[0] * kL[0] / kL2;
      P[1] = -kL[0] * kL[1] / kL2;
      P[2] = -kL[0] * kL[2] / kL2;
      P[3] = S(1) - kL[1] * kL[1] / kL2;
      P[4] = -kL[1] * kL[2] / kL2;
      P[5] = S(1) - kL[2] * kL[2] / kL2;

      Pu[0] = P[0] * u[0] + P[1] * u[1] + P[2] * u[2];
      Pu[1] = P[0] * u[1] + P[1] * u[3] + P[2] * u[4];
      Pu[2] = P[0] * u[2] + P[1] * u[4] - P[2] * (u[0] + u[3]);
      Pu[3] = P[1] * u[0] + P[3] * u[1] + P[4] * u[2];
      Pu[4] = P[1] * u[1] + P[3] * u[3] + P[4] * u[4];
      Pu[5] = P[1] * u[2] + P[3] * u[4] - P[4] * (u[0] + u[3]);
      Pu[6] = P[2] * u[0] + P[4] * u[1] + P[5] * u[2];
      Pu[7] = P[2] * u[1] + P[4] * u[3] + P[5] * u[4];
      Pu[8] = P[2] * u[2] + P[4] * u[4] - P[5] * (u[0] + u[3]);

      auto Tr1 = Pu[0] * conj(Pu[0]) + Pu[1] * conj(Pu[3]) + Pu[2] * conj(Pu[6]) + Pu[3] * conj(Pu[1]) +
                 Pu[4] * conj(Pu[4]) + Pu[5] * conj(Pu[7]) + Pu[6] * conj(Pu[2]) + Pu[7] * conj(Pu[5]) +
                 Pu[8] * conj(Pu[8]);
      auto Tr2 = Pu[0] + Pu[4] + Pu[8];

      return abs(Tr1 - S(0.5) * Tr2 * conj(Tr2));
    }
  };

  template <typename R, typename T, typename Model> class GWProjectorType2 : public GWProjector<R, T, Model>
  {
  public:
    using S = typename GWProjector<R, T, Model>::S;

    GWProjectorType2(const R &pR, const T &pT, const Model &model)
        : GWProjector<R, T, Model>::GWProjector(pR, pT, model)
    {
    }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      auto u = DoEval::eval(this->mR, idx...);
      auto k = DoEval::eval(this->mT, idx...);

      device::array<complex<S>, 3> kL;
      S kL2;
      device::array<complex<S>, 6> P;
      device::array<complex<S>, 9> Pu;
      device::array<complex<S>, 9> Pus;

      constexpr_for<0, 3>([&](auto i) {
        kL[i] = complex<S>(sin(this->kIR * static_cast<S>(k[i])), -S(1.) + cos(this->kIR * static_cast<S>(k[i])));
      });
      kL2 = powr<2>(abs(kL[0])) + powr<2>(abs(kL[1])) + powr<2>(abs(kL[2]));

      P[0] = complex<S>(1., 0.) - conj(kL[0]) * kL[0] / kL2;
      P[1] = -conj(kL[0]) * kL[1] / kL2;
      P[2] = -conj(kL[0]) * kL[2] / kL2;
      P[3] = complex<S>(1., 0.) - conj(kL[1]) * kL[1] / kL2;
      P[4] = -conj(kL[1]) * kL[2] / kL2;
      P[5] = complex<S>(1., 0.) - conj(kL[2]) * kL[2] / kL2;

      Pu[0] = P[0] * u[0] + P[1] * u[1] + P[2] * u[2];
      Pu[1] = P[0] * u[1] + P[1] * u[3] + P[2] * u[4];
      Pu[2] = P[0] * u[2] + P[1] * u[4] - P[2] * (u[0] + u[3]);
      Pu[3] = conj(P[1]) * u[0] + P[3] * u[1] + P[4] * u[2];
      Pu[4] = conj(P[1]) * u[1] + P[3] * u[3] + P[4] * u[4];
      Pu[5] = conj(P[1]) * u[2] + P[3] * u[4] - P[4] * (u[0] + u[3]);
      Pu[6] = conj(P[2]) * u[0] + conj(P[4]) * u[1] + P[5] * u[2];
      Pu[7] = conj(P[2]) * u[1] + conj(P[4]) * u[3] + P[5] * u[4];
      Pu[8] = conj(P[2]) * u[2] + conj(P[4]) * u[4] - P[5] * (u[0] + u[3]);

      Pus[0] = P[0] * conj(u[0]) + P[1] * conj(u[1]) + P[2] * conj(u[2]);
      Pus[1] = P[0] * conj(u[1]) + P[1] * conj(u[3]) + P[2] * conj(u[4]);
      Pus[2] = P[0] * conj(u[2]) + P[1] * conj(u[4]) - P[2] * (conj(u[0]) + conj(u[3]));
      Pus[3] = conj(P[1]) * conj(u[0]) + P[3] * conj(u[1]) + P[4] * conj(u[2]);
      Pus[4] = conj(P[1]) * conj(u[1]) + P[3] * conj(u[3]) + P[4] * conj(u[4]);
      Pus[5] = conj(P[1]) * conj(u[2]) + P[3] * conj(u[4]) - P[4] * (conj(u[0]) + conj(u[3]));
      Pus[6] = conj(P[2]) * conj(u[0]) + conj(P[4]) * conj(u[1]) + P[5] * conj(u[2]);
      Pus[7] = conj(P[2]) * conj(u[1]) + conj(P[4]) * conj(u[3]) + P[5] * conj(u[4]);
      Pus[8] = conj(P[2]) * conj(u[2]) + conj(P[4]) * conj(u[4]) - P[5] * (conj(u[0]) + conj(u[3]));

      auto Tr1 = Pu[0] * Pus[0] + Pu[1] * Pus[3] + Pu[2] * Pus[6] + Pu[3] * Pus[1] + Pu[4] * Pus[4] + Pu[5] * Pus[7] +
                 Pu[6] * Pus[2] + Pu[7] * Pus[5] + Pu[8] * Pus[8];
      auto Tr2 = Pu[0] + Pu[4] + Pu[8];
      auto Tr2s = Pus[0] + Pus[4] + Pus[8];

      return abs(Tr1 - S(.5) * Tr2 * Tr2s);
    }
  };

  template <typename R, typename T, typename Model> class GWProjectorType3 : public GWProjector<R, T, Model>
  {
  public:
    using S = typename GWProjector<R, T, Model>::S;

    GWProjectorType3(const R &pR, const T &pT, const Model &model)
        : GWProjector<R, T, Model>::GWProjector(pR, pT, model)
    {
    }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      auto u = DoEval::eval(this->mR, idx...);
      auto k = DoEval::eval(this->mT, idx...);

      device::array<complex<S>, 3> kL;
      S kL2;
      device::array<complex<S>, 6> P;
      device::array<complex<S>, 9> Pu;
      device::array<complex<S>, 9> Pus;

      constexpr_for<0, 3>([&](auto i) {
        kL[i] = complex<S>(sin(this->kIR * static_cast<S>(k[i])), S(1.) - cos(this->kIR * static_cast<S>(k[i])));
      });

      kL2 = powr<2>(abs(kL[0])) + powr<2>(abs(kL[1])) + powr<2>(abs(kL[2]));

      P[0] = complex<S>(1., 0.) - conj(kL[0]) * kL[0] / kL2;
      P[1] = -conj(kL[0]) * kL[1] / kL2;
      P[2] = -conj(kL[0]) * kL[2] / kL2;
      P[3] = complex<S>(1., 0.) - conj(kL[1]) * kL[1] / kL2;
      P[4] = -conj(kL[1]) * kL[2] / kL2;
      P[5] = complex<S>(1., 0.) - conj(kL[2]) * kL[2] / kL2;

      Pu[0] = P[0] * u[0] + P[1] * u[1] + P[2] * u[2];
      Pu[1] = P[0] * u[1] + P[1] * u[3] + P[2] * u[4];
      Pu[2] = P[0] * u[2] + P[1] * u[4] - P[2] * (u[0] + u[3]);
      Pu[3] = conj(P[1]) * u[0] + P[3] * u[1] + P[4] * u[2];
      Pu[4] = conj(P[1]) * u[1] + P[3] * u[3] + P[4] * u[4];
      Pu[5] = conj(P[1]) * u[2] + P[3] * u[4] - P[4] * (u[0] + u[3]);
      Pu[6] = conj(P[2]) * u[0] + conj(P[4]) * u[1] + P[5] * u[2];
      Pu[7] = conj(P[2]) * u[1] + conj(P[4]) * u[3] + P[5] * u[4];
      Pu[8] = conj(P[2]) * u[2] + conj(P[4]) * u[4] - P[5] * (u[0] + u[3]);

      Pus[0] = P[0] * conj(u[0]) + P[1] * conj(u[1]) + P[2] * conj(u[2]);
      Pus[1] = P[0] * conj(u[1]) + P[1] * conj(u[3]) + P[2] * conj(u[4]);
      Pus[2] = P[0] * conj(u[2]) + P[1] * conj(u[4]) - P[2] * (conj(u[0]) + conj(u[3]));
      Pus[3] = conj(P[1]) * conj(u[0]) + P[3] * conj(u[1]) + P[4] * conj(u[2]);
      Pus[4] = conj(P[1]) * conj(u[1]) + P[3] * conj(u[3]) + P[4] * conj(u[4]);
      Pus[5] = conj(P[1]) * conj(u[2]) + P[3] * conj(u[4]) - P[4] * (conj(u[0]) + conj(u[3]));
      Pus[6] = conj(P[2]) * conj(u[0]) + conj(P[4]) * conj(u[1]) + P[5] * conj(u[2]);
      Pus[7] = conj(P[2]) * conj(u[1]) + conj(P[4]) * conj(u[3]) + P[5] * conj(u[4]);
      Pus[8] = conj(P[2]) * conj(u[2]) + conj(P[4]) * conj(u[4]) - P[5] * (conj(u[0]) + conj(u[3]));

      auto Tr1 = Pu[0] * Pus[0] + Pu[1] * Pus[3] + Pu[2] * Pus[6] + Pu[3] * Pus[1] + Pu[4] * Pus[4] + Pu[5] * Pus[7] +
                 Pu[6] * Pus[2] + Pu[7] * Pus[5] + Pu[8] * Pus[8];
      auto Tr2 = Pu[0] + Pu[4] + Pu[8];
      auto Tr2s = Pus[0] + Pus[4] + Pus[8];

      return abs(Tr1 - S(0.5) * Tr2 * Tr2s);
    }
  };

  template <typename Model> auto projectGWType1(const Model &model)
  {
    return GWProjectorType1((*model.piGWs).inFourierSpace(), WaveNumber(model.getToolBox()), model);
  }

  template <typename Model> auto projectGWType2(const Model &model)
  {
    return GWProjectorType2((*model.piGWs).inFourierSpace(), WaveNumber(model.getToolBox()), model);
  }

  template <typename Model> auto projectGWType3(const Model &model)
  {
    return GWProjectorType3((*model.piGWs).inFourierSpace(), WaveNumber(model.getToolBox()), model);
  }

} // namespace TempLat

#endif
