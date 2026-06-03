#ifndef COSMOINTERFACE_EVOLVERS_RK2NSTORAGE_H
#define COSMOINTERFACE_EVOLVERS_RK2NSTORAGE_H
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/evolvers/rk2nstorageparameters.h"
#include "CosmoInterface/evolvers/kernels/kernels.h"
#include "CosmoInterface/evolvers/kernels/kernelstypes.h"
#include "CosmoInterface/definitions/averages.h"
#include "CosmoInterface/definitions/nonminimalcoupling.h"
#include "CosmoInterface/definitions/fixedbackgroundexpansion.h"
#include "CosmoInterface/extrafields.h"

namespace TempLat
{

  /** \brief A class which implements low storage ("2N-storage") explicit RK methods.
   *
   *
   **/
  template <typename Model> class RK2NStorage
  {
  public:
    using T = typename Model::FloatType;

    /* Put public methods here. These should change very little over time. */
    RK2NStorage(Model &model, RunParameters<T> runParams)
        : type(runParams.eType), dt(model.dt), As(RK2NStorageParameters<T>::getAs(type)),
          Bs(RK2NStorageParameters<T>::getBs(type)),Cs(RK2NStorageParameters<T>::getCs(type)), expansion(runParams.expansion), 
          fixedBackground(runParams.fixedBackground), aBackground(model, runParams)
    {
      ForEachField(Model, fld, n, isDefined[fld].emplace_back(true); isDeactivated[fld].emplace_back(false););
    }

    template <int N> void activate(Tag<N> t) { ForLoop(n, 0, Model::getNFields(t) - 1, isDeactivated[t][n] = false;); }

    template <int N> void deactivate(Tag<N> t) { ForLoop(n, 0, Model::getNFields(t) - 1, isDeactivated[t][n] = true;); }

    template <int FLD, int N> bool isActive(Tag<FLD>, Tag<N>) const
    { return !isDeactivated[FLD][N] && isDefined[FLD][N]; }

    void evolve(Model &model, T tMinust0) { evolve(model, tMinust0, KernelsTypes::EoM<Model>()); }

    template <class KernelType> void evolve(Model &model, T tMinust0, KernelType kt)
    {

      /*
       * 2N storage RK ....
       *
       *
       * */

      dt = KernelsTypes::getDt(model, kt);

      kt.cache(model, tMinust0); // To be able to store some temporary info in the kernel type
      const T aStart = model.aI;

      for (size_t i = 0; i < As.size(); ++i) { // loop over operations...

        ForEachField(
            Model, fld, n, if (!isDeactivated[fld][n]) {
              isDefined[fld][n] = delta(i, Delta->get(fld)(n), Kernels::get(fld, model, n, kt));
            });

        if (expansion) sfDefined = deltaScaleFactor(model, i, kt);

        ForEachField(Model, fld, n, if (!isDeactivated[fld][n] && isDefined[fld][n]) { advance(i, fld, model, n); });

        if (expansion) advanceScaleFactor(model, i, tMinust0);

        if (expansion) {
          Averages::setAllAverages(model);
          syncR(model,tMinust0);
        }
        kt.cache(model, tMinust0);
      }

      if (expansion) syncSemiIntegerScaleFactor(model, tMinust0, aStart);
    }

    bool deltaScaleFactor(Model &model, size_t i, KernelsTypes::EoM<Model> kt)
    { 
      if (!fixedBackground) {
        if (i == 0) {
          deltaADot = dt * ScaleFactorKernels::get(model, kt);
          deltaA = dt * model.aDotI;
        } else {
          deltaADot = As[i] * deltaADot + dt * ScaleFactorKernels::get(model, kt);
          deltaA = As[i] * deltaA + dt * model.aDotI;
        }
        return true;
        }
        return false;
    }
    template <class KernelType> bool deltaScaleFactor(Model &model, size_t i, KernelType) { return false; }

    void advanceScaleFactor(Model &model, size_t i, T tMinust0)
    {
       if (!fixedBackground) {      
        if (sfDefined) model.aI += Bs[i] * deltaA;

        if constexpr (not Model::IsNonMinimallyCoupled) {
        if (sfDefined) model.aDotI += Bs[i] * deltaADot;
        } else {
        if (sfDefined) model.piAI += Bs[i] * deltaADot;
        if (sfDefined) model.aDotI = model.piAI * pow(model.aI, model.alpha - 1);
        }
      }
      else
      {
        model.aI = aBackground(tMinust0 + Cs[i] * model.dt);
        if constexpr (Model::IsNonMinimallyCoupled) model.RI = aBackground.R(tMinust0 + Cs[i] * model.dt);
      }
    } 

    void syncSemiIntegerScaleFactor(Model &model, T tMinust0, T aStart)
    {
      model.aIM = aStart;
      if (fixedBackground) {
        model.aI = aBackground(tMinust0 + model.dt);
        model.aSI = aBackground(tMinust0 + model.dt / 2.0);
        model.aDotI = aBackground.dot(tMinust0 + model.dt);
        if constexpr (Model::IsNonMinimallyCoupled) model.RI = aBackground.R(tMinust0 + model.dt);
      } else {
        model.aSI = (model.aIM + model.aI) / 2.0;
      }
    }

    // This function is called before doing the measurements. It is used to set aI, aDotI and RI to its correct value in case
    // of  fixed-background expansion.
    void sync(Model &model, T tMinust0)
    {  
     if(fixedBackground) {
        model.aI = aBackground(tMinust0);  
        model.aDotI = aBackground.dot(tMinust0);

        if constexpr (Model::IsNonMinimallyCoupled) model.RI = aBackground.R(tMinust0);
      }
    }
    // This function is called in the case of NMC evolution to sync the value of R obtained from the volumen averages of the matter fields. 
    void syncR(Model &model, T tMinust0)
    {
      if constexpr (Model::IsNonMinimallyCoupled) {
          if (!fixedBackground)  model.RI = NonMinimalCoupling::R(model);
      }
    }

    void setDelta(ExtraFields<Model> extraFlds) { Delta = extraFlds.getAllFlds1(); }

  private:
    template <class Delta, class Kernel> bool delta(size_t i, Delta delta, Kernel kernel)
    {
      if (i == 0) {
        delta = dt * kernel;
      } else {
        delta = As[i] * delta + dt * kernel;
      }
      return true;
    }

    template <class Delta> bool delta(size_t i, Delta delta, ZeroType kernel) { return false; }

    template <int FLD, int N> void advance(size_t i, Tag<FLD> fld, Model &model, Tag<N> n)
    {
      model.getField(fld)(n) += Bs[i] * Delta->get(fld)(n);
    }

    template <int N> void advance(size_t i, FieldsNumbering::fldSU2 fld, Model &model, Tag<N> n)
    {
      ForLoop(j, 1, Model::NDim, model.fldSU2(n)(j) = exp(Bs[i] * Delta->fldSU2(n)(j)) * model.fldSU2(n)(j););
    }

    /* Put all member variables and private methods here. These may change arbitrarily. */

    EvolverType type;

    T dt; // Has its own dt as we can use it for other things than real time evolution (cooling for instance).

   

    const std::vector<T> As;
    const std::vector<T> Bs;
    const std::vector<T> Cs;

    std::shared_ptr<FieldsAsInModel<Model>> Delta;

    std::array<std::vector<bool>, FieldsNumbering::maxNum + 1> isDefined;
    std::array<std::vector<bool>, FieldsNumbering::maxNum + 1> isDeactivated;

    T deltaA, deltaADot;
    bool sfDefined;

    bool expansion;
    bool fixedBackground;
    
    FixedBackgroundExpansion<T> aBackground;
  };

} // namespace TempLat

#endif
