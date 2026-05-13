#ifndef COSMOINTERFACE_DEFINITIONS_CHIRALPOWERSPECTRUM_H
#define COSMOINTERFACE_DEFINITIONS_CHIRALPOWERSPECTRUM_H

#include "TempLat/util/constants.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"

namespace TempLat
{
  template <typename R, typename T, typename Model>
  class ChiralProjector : public TempLat::BinaryOperator<R, T>
  {
  public:
    using TempLat::BinaryOperator<R, T>::mR;
    using TempLat::BinaryOperator<R, T>::mT;
    using S = typename Model::FloatType;

    DEVICE_FUNCTION
    ChiralProjector(const R& pR, const T& pT, const Model& model, int component, bool sign)
      : TempLat::BinaryOperator<R, T>(pR, pT),
        mComponent(component),
        mSign(sign),
        mN(static_cast<S>(GetNGrid::get(model)))
    { }

    static consteval size_t getNDim() { return Model::NDim; }

    virtual std::string operatorString() const override { return "Chiral_Projector"; }

  private:
    int mComponent;   // 0,1,2 internally
    bool mSign;
    S mN;

    template <typename X>
    DEVICE_FORCEINLINE_FUNCTION complex<S> extraPhaseMinus(const X& n) const
    {
      const S phase = -Constants::pi<S> * static_cast<S>(n) / mN;
      return complex<S>(cos(phase), sin(phase));
    }

    template <typename X>
    DEVICE_FORCEINLINE_FUNCTION S kEff(const X& n) const
    {
      return sin(Constants::pi<S> * static_cast<S>(n) / mN);
    }

    DEVICE_FORCEINLINE_FUNCTION S delta(int a, int b) const
    {
      return a == b ? S(1) : S(0);
    }

    DEVICE_FORCEINLINE_FUNCTION S epsilon(int a, int b, int c) const
    {
      if ((a == 0 && b == 1 && c == 2) || (a == 1 && b == 2 && c == 0) || (a == 2 && b == 0 && c == 1)) return S(1);
      if ((a == 0 && b == 2 && c == 1) || (a == 2 && b == 1 && c == 0) || (a == 1 && b == 0 && c == 2)) return S(-1);
      return S(0);
    }

  public:
    template <typename... IDX>
  requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
    requires IsVariadicIndex<IDX...>;
    DoEval::eval(t, idx...);
  }
DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX&... idx) const
  {
    static_assert(Model::NDim == 3, "ChiralProjector only implemented for 3D.");

    auto k = DoEval::eval(this->mT, idx...);

    device::array<complex<S>, 3> u;
    u[0] = DoEval::eval(this->mR(1_c), idx...);
    u[1] = DoEval::eval(this->mR(2_c), idx...);
    u[2] = DoEval::eval(this->mR(3_c), idx...);

    device::array<S, 3> kL;
    device::array<complex<S>, 3> up;

    constexpr_for<0, 3>([&](auto a) {
        kL[a] = sin(Constants::pi<S> * static_cast<S>(k[a]) / mN);

        const S phase = -Constants::pi<S> * static_cast<S>(k[a]) / mN;
        up[a] = complex<S>(cos(phase), sin(phase)) * u[a];
    });

    const S kL2 = kL[0]*kL[0] + kL[1]*kL[1] + kL[2]*kL[2];
    if (kL2 == S(0)) return S(0);

    const S normk = sqrt(kL2);
    const int i = mComponent;

    complex<S> projected = complex<S>(0.,0.);

    constexpr_for<0, 3>([&](auto jtag) {
        constexpr int j = jtag;

        complex<S> sigma(S(0.5) * ((i == j ? S(1) : S(0)) - kL[i]*kL[j]/kL2), 0.0);

        S epsSum = 0;
        constexpr_for<0, 3>([&](auto ltag) {
            constexpr int l = ltag;
            epsSum += epsilon(i,j,l) * kL[l];
        });

        const S helicity = mSign ? S(1) : S(-1);
        sigma += complex<S>(0.0, -S(0.5) * helicity * epsSum / normk);

        projected += sigma * up[j];
    });

    return pow<2>(abs(projected));
  }
  };

  template <typename Model, int U1, int C>
DEVICE_FORCEINLINE_FUNCTION auto projectChiralU1Type1(const Model& model, Tag<U1>, Tag<C>, bool sign, bool AorE)
{
    static_assert(C >= 1 && C <= 3, "Vector component tag must be 1,2,3.");

    if (AorE)
    {
        return ChiralProjector(MakeVector(i, 1, Model::NDim, model.fldU1(Tag<U1>())(i).inFourierSpace()), WaveNumber(model.getToolBox()), model, C - 1, sign);
    }
    else
    {
        return ChiralProjector(MakeVector(i, 1, Model::NDim, model.piU1(Tag<U1>())(i).inFourierSpace()), WaveNumber(model.getToolBox()), model, C - 1, sign);
    }
}

} // namespace TempLat

#endif