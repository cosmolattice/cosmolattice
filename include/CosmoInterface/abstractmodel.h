#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMODEL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include <array>
#include "TempLat/lattice/algebra/gaugealgebra/forwardcovariantderivative.h"
#include "TempLat/util/exception.h"
#include "TempLat/util/templatvector.h"
#include "TempLat/parameters/parameterparser.h"

#include "CosmoInterface/abstractmodel/scalarbase.h"
#include "CosmoInterface/abstractmodel/complexscalarbase.h"
#include "CosmoInterface/abstractmodel/su2doubletbase.h"
#include "CosmoInterface/abstractmodel/scalaru1axionbase.h"
#include "CosmoInterface/abstractmodel/nonminimalcouplingbase.h"
#include "CosmoInterface/abstractmodel/scalefactorbase.h"
#include "CosmoInterface/abstractmodel/modelparametersbase.h"
#include "CosmoInterface/abstractmodel/gwbase.h"
#include "CosmoInterface/abstractmodel/u1base.h"
#include "CosmoInterface/abstractmodel/su2base.h"

#include "CosmoInterface/definitions/potential.h"

namespace TempLat
{
  // Exceptions that kill the program when some conditions are met (used below)
  MakeException(EmptyModel);
  MakeException(NotTested);

  struct DefaultModelPars {

    // Number of fields of each species (set to zero by default)
    static constexpr size_t NScalars = 0;    // scalar singlets
    static constexpr size_t NCScalars = 0;   // complex scalars
    static constexpr size_t NU1Flds = 0;     // U(1) gauge fields
    static constexpr size_t NSU2Doublet = 0; // SU(2) doublets
    static constexpr size_t NSU2Flds = 0;    // SU(2) gauge fields

    // Number of terms in the potential. It can be changed in the model file.
    static constexpr size_t NPotTerms = 1;

    // Number of dimensions (1,2,3). It can be changed in the model file.
    static constexpr size_t NDim = 3;

    using NumberType = double;

    // Coupling managers, they deal with the possible couplings between the gauge fields and complex scalars/SU2
    // doublets
    using CsU1Couplings = CouplingsManager<NCScalars, NU1Flds>;             // couplings U(1) gauge-complex scalar
    using SU2DoubletU1Couplings = CouplingsManager<NSU2Doublet, NU1Flds>;   // couplings U(1) gauge-SU2 doublet
    using SU2DoubletSU2Couplings = CouplingsManager<NSU2Doublet, NSU2Flds>; // couplings SU(2) gauge-SU2 doublet
    using ScalarU1AxionCouplings = CouplingsManager<NScalars, NU1Flds>;     // couplings U(1) gauge-scalar axion
    using NonMinimalCouplings = CouplingsManager<NScalars, 1>; // Non-minimal coupling to gravity of scalars.
  };

  // In order to make some of the expression template mechanism works, the number of fields needs to be known
  // at compile time. Also, to take advantage of the possible algebraic simplification 0*a=0 and to implement
  // couplings to generic number of fields (not fully available in v 1.0), one needs to know at compile time what
  // couples to what. All these parameters are passed as template argument to the model. In order to simplify the
  // syntax, the following macros allow the user to define a model simply by specifying a user-defined class, containing
  // the appropriate variable.

#define MakeAbstractModelTemplateArgs(_ModelName, _ModelParsType, _FloatType)                                          \
  _ModelName, _ModelParsType::NPotTerms, _ModelParsType::NScalars, _ModelParsType::NCScalars, _ModelParsType::NU1Flds, \
      _ModelParsType::NSU2Doublet, _ModelParsType::NSU2Flds, typename _ModelParsType::CsU1Couplings,                   \
      typename _ModelParsType::SU2DoubletU1Couplings, typename _ModelParsType::SU2DoubletSU2Couplings,                 \
      typename _ModelParsType::ScalarU1AxionCouplings, typename _ModelParsType::NonMinimalCouplings, _FloatType,       \
      _ModelParsType::NDim
#define MakeModelFloatType(_ModelName, _ModelParsType, _FloatType)                                                     \
  AbstractModel<MakeAbstractModelTemplateArgs(_ModelName, _ModelParsType, _FloatType)>
#define MakeModel(_ModelName, _ModelParsType)                                                                          \
  AbstractModel<MakeAbstractModelTemplateArgs(_ModelName, _ModelParsType, typename _ModelParsType::NumberType)>

  /** @brief A class which contains everything a model should have; models derive from here.
   * Mother of all the models. The arguments are passed as template parameters.
   *
   **/
  template <class R, size_t NPOTTERMS, size_t NS, size_t NC, size_t NU1FLDS, size_t NSU2DOUBLET, size_t NSU2FLDS,
            typename CSU1COUPLINGS, typename SU2DOUBLETU1COUPLINGS, typename SU2DOUBLETSU2COUPLINGS,
            typename SCALARU1AXIONCOUPLINGS, typename NONMINCOUPLINGS, typename T = double, int NDIM = 3>
  class AbstractModel
      : public ScalarBase<NDIM, T, NS>,
        public ComplexScalarBase<NDIM, T, NC, CSU1COUPLINGS>,
        public SU2DoubletSectorBase<NDIM, T, NSU2DOUBLET, SU2DOUBLETU1COUPLINGS, SU2DOUBLETSU2COUPLINGS>,
        public ScalarU1AxionBase<T, SCALARU1AXIONCOUPLINGS>,
        public NonMinimalCouplingBase<T, NS, NONMINCOUPLINGS>,
        public ScaleFactorBase<T>,
        public ModelParametersBase<T>,
        public GWBase<NDIM, T>,
        public U1Base<NDIM, T, NU1FLDS, NS, NC, SCALARU1AXIONCOUPLINGS>,
        public SU2Base<NDIM, T, NSU2FLDS>
  {
  public:
    // NPotTerms stays here — no natural sector base home without adding a template parameter
    static constexpr size_t NPotTerms = NPOTTERMS;

    // --- Field iteration API (centralized) ---

    static constexpr std::array<size_t, FieldsNumbering::maxNum + 1> fieldCounts = {
        NS,          NS,          // fldS (0), piS (1)
        NC,          NC,          // fldCS (2), piCS (3)
        NSU2DOUBLET, NSU2DOUBLET, // fldSU2Doublet (4), piSU2Doublet (5)
        NU1FLDS,     NU1FLDS,     // fldU1 (6), piU1 (7)
        NSU2FLDS,    NSU2FLDS     // fldSU2 (8), piSU2 (9)
    };

    template <int N> static constexpr size_t getNFields(Tag<N>) { return fieldCounts[N]; }

    template <int N> auto getField(Tag<N>)
    {
      if constexpr (N == FieldsNumbering::fldS::value)
        return this->fldS;
      else if constexpr (N == FieldsNumbering::piS::value)
        return this->piS;
      else if constexpr (N == FieldsNumbering::fldCS::value)
        return this->fldCS;
      else if constexpr (N == FieldsNumbering::piCS::value)
        return this->piCS;
      else if constexpr (N == FieldsNumbering::fldSU2Doublet::value)
        return this->fldSU2Doublet;
      else if constexpr (N == FieldsNumbering::piSU2Doublet::value)
        return this->piSU2Doublet;
      else if constexpr (N == FieldsNumbering::fldU1::value)
        return this->fldU1;
      else if constexpr (N == FieldsNumbering::piU1::value)
        return this->piU1;
      else if constexpr (N == FieldsNumbering::fldSU2::value)
        return this->fldSU2;
      else if constexpr (N == FieldsNumbering::piSU2::value)
        return this->piSU2;
    }

    // --> Averages potential
    T potAvI, potAvSI;

    // Initial potential
    T pot0, pot0SI;

    AbstractModel(ParameterParser &parser, const LatticeParameters<T> &par,
                  device::memory::host_ptr<MemoryToolBox<NDIM>> toolBox, T pDt, std::string pName = "")
        : ScalarBase<NDIM, T, NS>(parser, toolBox, par),
          ComplexScalarBase<NDIM, T, NC, CSU1COUPLINGS>(parser, toolBox, par),
          SU2DoubletSectorBase<NDIM, T, NSU2DOUBLET, SU2DOUBLETU1COUPLINGS, SU2DOUBLETSU2COUPLINGS>(parser, toolBox,
                                                                                                    par),
          ScalarU1AxionBase<T, SCALARU1AXIONCOUPLINGS>(parser), NonMinimalCouplingBase<T, NS, NONMINCOUPLINGS>(parser),
          ScaleFactorBase<T>(), ModelParametersBase<T>(par, pDt, std::move(pName)),
          GWBase<NDIM, T>(parser, toolBox, par),
          U1Base<NDIM, T, NU1FLDS, NS, NC, SCALARU1AXIONCOUPLINGS>(toolBox, par),
          SU2Base<NDIM, T, NSU2FLDS>(toolBox, par)
    {
      // Uncomment these exceptions in case you want to run a model with more than one U(1) or SU(2) gauge field (this
      // feature has yet not been tested)
      if constexpr (NDIM != 3) {
        if (this->fldGWs != nullptr)
          throw(RunParametersInconsistent(
              "NDims must be equal to 3 to run GWs. If you want to run with NDim != 3, make sure withGWs = false."));
        if(parser.get<int>("PS_type", 1) != 2)
          throw(RunParametersInconsistent(
            "Spectra for simulations with NDim != 3 emulating the 3D dynamics have not been implemented for PSType=1, only for PSType=2. Abort. If you want to perform a real NDim != 3 simulations comment out this line. You may need to modify the EoM kernels"));
      }
      if constexpr (NU1FLDS > 1)
        throw(NotTested("The physics interface has not been fully tested with NU1 > 1. Abort. If you want to go on "
                        "anyway, uncomment the exception thrown in src/include/CosmoInterface/abstractmodel.h and "
                        "please report any problems."));
      if constexpr (NSU2FLDS > 1)
        throw(NotTested("The physics interface has not been fully tested with NSU2 > 1. Abort. If you want to go on "
                        "anyway, uncomment the exception thrown in src/include/CosmoInterface/abstractmodel.h and "
                        "please report any problems."));
    }

    // AbstractModel should never be copied, so we delete the copy constructor and copy assignment operator.
    AbstractModel(const AbstractModel &) = delete;
    AbstractModel &operator=(const AbstractModel &) = delete;

    // Sometimes, it can be useful to get "any field" of the model. This function implements this in a generic way.
    Field<T, NDIM> getOneField() const
    {
      if constexpr (NS > 0)
        return this->fldS(0_c);
      else if constexpr (NC > 0)
        return this->fldCS(0_c)(0_c);
      else if constexpr (NSU2DOUBLET > 0)
        return this->fldSU2Doublet(0_c)(0_c);
      else if constexpr (NU1FLDS > 0)
        return this->fldU1(0_c)(1_c);
      else if constexpr (NSU2FLDS > 0)
        return this->fldSU2(0_c)(1_c)(1_c);
      else {
        throw(EmptyModel("The model seems empty, cannot return a field. Abort."));
        return this->fldS(0_c);
      }
    }

    template <int N> auto getFluctuationRatio(Tag<N>) { return OneType(); }

    // The "MemoryToolBox" is a shared variable between most instances of the program. It contains many useful
    // informations about
    // the intrinsic parameter of the library. Sometimes, some of the classes need the MemoryToolBox to be created. This
    // function the user to quickly get it from the model.
    device::memory::host_ptr<MemoryToolBox<NDIM>> getToolBox() const { return getOneField().getToolBox(); }

    // set the initial value of the potential and the masses of the fields from the expression of the potential.
    void setInitialPotentialAndMassesFromPotential()
    {
      // This adds the homogeneous components of the fields at one point:
      this->addInitValueOnePoint();
      // It's just a trick to compute the initial potential and masses; it must be removed afterwards with
      // "removeInitValue()"

      device::IdxArray<NDIM> pos0{{}};

      pot0 = device::memory::getAtOnePoint(Potential::potential(static_cast<R &>(*this)), pos0);

      // Compute second derivatives of the potential, giving the mass square.
      ForLoop(j, 0, NS - 1,
              this->masses2S(j) = device::memory::getAtOnePoint(Potential::deriv2S(static_cast<R &>(*this), j), pos0));
      ForLoop(j, 0, NC - 1,
              this->masses2CS(j) = Complexify(
                  device::memory::getAtOnePoint(Potential::deriv2CS(static_cast<R &>(*this), j)(0_c), pos0),
                  device::memory::getAtOnePoint(Potential::deriv2CS(static_cast<R &>(*this), j)(1_c), pos0)););
      ForLoop(j, 0, NSU2DOUBLET - 1,
              this->masses2SU2Doublet(j) = MakeSU2Doublet(
                  a, device::memory::getAtOnePoint(Potential::deriv2SU2Doublet(static_cast<R &>(*this), j)(a), pos0)););

      // This removes the homogeneous component at one point added previously with "addInitValueOnePoint()"
      this->removeInitValue();
    }

    // set only the initial value of the potential from the expression of the potential. Useful if you want to specify
    //  the masses not from the second derivative of the potential; you will still need to compute pot0.
    void setInitialPotentialFromPotential()
    {
      // This adds the homogeneous components of the fields at one point:
      this->addInitValueOnePoint();
      // It's just a trick to compute the initial potential and masses; it must be removed afterwards with
      // "removeInitValue()"

      device::IdxArray<NDIM> pos0{{}};

      // Compute initial potential at t=0
      pot0 = device::memory::getAtOnePoint(Potential::potential(static_cast<R &>(*this)), pos0);

      // This removes the homogeneous component at one point added previously with "addInitValueOnePoint()"
      this->removeInitValue();
    }

  private:
    // This sets the homogeneous components of the fields at a single point.
    void addInitValueOnePoint()
    {
      device::IdxArray<NDIM> pos0{{}};
      ForLoop(j, 0, NS - 1, device::memory::setAtOnePoint(this->fldS(j), pos0, this->fldS0[j] / this->fStar););
      ForLoop(
          j, 0, NC - 1,
          ForLoop(i, 0, 1, device::memory::setAtOnePoint(this->fldCS(j)(i), pos0, this->fldCS0(j)(i) / this->fStar);));
      ForLoop(j, 0, NSU2DOUBLET - 1,
              ForLoop(i, 0, 3,
                      device::memory::setAtOnePoint(this->fldSU2Doublet(j)(i), pos0,
                                                    this->fldSU2Doublet0(j)(i) / this->fStar);));
    }

    // This removes the homogeneous components of the fields at a single point.
    void removeInitValue()
    {
      device::IdxArray<NDIM> pos0{{}};
      ForLoop(j, 0, NS - 1, device::memory::setAtOnePoint(this->fldS(j), pos0, 0.););
      ForLoop(j, 0, NC - 1, ForLoop(i, 0, 1, device::memory::setAtOnePoint(this->fldCS(j)(i), pos0, 0.);));
      ForLoop(j, 0, NSU2DOUBLET - 1,
              ForLoop(i, 0, 3, device::memory::setAtOnePoint(this->fldSU2Doublet(j)(i), pos0, 0.);));
    }
  };
} // namespace TempLat

#endif
