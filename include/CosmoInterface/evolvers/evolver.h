#ifndef COSMOINTERFACE_EVOLVERS_EVOLVER_H
#define COSMOINTERFACE_EVOLVERS_EVOLVER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/evolvers/leapfrog.h"
#include "CosmoInterface/evolvers/positionverlet.h"
#include "CosmoInterface/evolvers/rk2nstorage.h"
#include "CosmoInterface/evolvers/velocityverlet.h"

#include "TempLat/util/exception.h"

namespace TempLat
{

  /** @brief A class which interfaces the evolver, so we don't need to use pointers in the main.
   *
   *
   **/

  MakeException(EvolverTypeNotInEvolver);
  MakeException(InvalidEvolverTypeGW);

  template <class Model> class Evolver
  {
  public:
    // Put public methods here. These should change very little over time.
    using T = typename Model::FloatType;

    Evolver(Model &model, RunParameters<T> &rPar, ExtraFields<Model> extraFlds)
        : type(rPar.eType),
          typeGW(rPar.eTypeGW),
          lf( (type == LF || (typeGW == LF && model.fldGWs != nullptr)) ? std::make_shared<LeapFrog<T>>(model, rPar) : nullptr),
          vv(VelocityVerletParameters<T>::isVerlet(type) ? std::make_shared<VelocityVerlet<T>>(model, rPar) : nullptr),
          pv(PositionVerletParameters<T>::isVerlet(type) ? std::make_shared<PositionVerlet<T>>(model, rPar) : nullptr),
          rk2n(RK2NStorageParameters<T>::isRK2n(type) ? std::make_shared<RK2NStorage<Model>>(model, rPar) : nullptr),
          GWsynced(!rPar.doWeRestart)
    {
      // RK2N needs Delta wired up here.
      if (rk2n != nullptr) {
        rk2n->setDelta(extraFlds);
      }

      if (lf == nullptr && vv == nullptr && pv == nullptr && (rk2n == nullptr || !RK2NStorageParameters<T>::isRK2n(type)))
        throw(EvolverTypeNotInEvolver("The evolver type you specified was not implemented in the Evolver class, "
                                      "which dispatch between different evolvers. Abort."));

      if (Model::NU1 > 0 && rPar.withGWs && type == LF)
        throw(InvalidEvolverTypeGW("The evolution of a model with U(1) and GWs requires to use an evolver that correctly synchronizes fields and momenta (as both electric and magnetic fields enter the kernel of GWs). You should use a different evolver for the matter fields (VVn, RK, PV). Abort."));

      if (Model::NU1 > 0 && rPar.withGWs && PositionVerletParameters<T>::isVerlet(type) &&
          PositionVerletParameters<T>::isVerlet(typeGW))
        throw(InvalidEvolverTypeGW("The evolution of a model with U(1) and GWs requires to use an evolver that correctly synchronizes fields and momenta (as both electric and magnetic fields enter the kernel of GWs). If you want to use PV for the matter fields, you should use LF for the GWs. Abort."));

    }

    inline void evolve(Model &model, T tMinust0) const
    {
      if (model.fldGWs != nullptr && typeGW == LF) {
        lf->kickGWs(model, GWsynced ? 0.5 : 1.0);
        GWsynced = false;
      }

      if (type == LF) {
        lf->evolve(model, tMinust0);
      } else if (RK2NStorageParameters<T>::isRK2n(type)) {
        rk2n->evolve(model, tMinust0, EoMKernels);
      } else if (PositionVerletParameters<T>::isVerlet(type)) {
        pv->evolve(model, tMinust0, typeGW == type);
      } else {
        if (!(VelocityVerletParameters<T>::isVerlet(type)))
          throw(EvolverTypeNotInEvolver("The evolver type you specified was not implemented in the Evolver class, "
                                        "which dispatch between different evolvers. Abort."));
        else
          vv->evolve(model, tMinust0, typeGW == type);
      }

      if (model.fldGWs != nullptr && typeGW == LF) lf->driftGWs(model);
    }

    // The next function is used to synchronised all the fields to live
    // at integer time before measurements. Useful for evolvers where
    // this is not naturally the case, such as leapfrog.

    inline void sync(Model &model, T tMinust0) const
    {
      if (typeGW == LF && !GWsynced) {
        lf->kickGWs(model, 0.5);
        GWsynced = true;
      }

      if (type == LF) {
        lf->sync(model, tMinust0);
      } else if (RK2NStorageParameters<T>::isRK2n(type)) {
        rk2n->sync(model, tMinust0);
      } else if (PositionVerletParameters<T>::isVerlet(type)) {
        pv->sync(model, tMinust0);
      } else { // The default evolvers have fields and momenta living at integer times, so no need to sync. for
               // measurements.
        if (!(VelocityVerletParameters<T>::isVerlet(type)))
          throw(EvolverTypeNotInEvolver("The evolver type you specified was not implemented in the Evolver class, "
                                        "which dispatch between different evolvers. Abort."));
        else
          vv->sync(
              model,
              tMinust0); // The sync function is used to set aDot to its correct value in the case of fixed background.
      }

    }

    // To activate and deactivate fields. Can be useful if more than a kernel is defined, or maybe to deactivate GW.
    template <int N> void deactivate(Tag<N> t)
    {
      if (RK2NStorageParameters<T>::isRK2n(type)) {
        rk2n->deactivate(t);
      } else {
        throw(EvolverTypeNotInEvolver("The activate/desactivate function is implemented only for the RK2N evolvers. "
                                      "Go implement it in the others if you need it."));
      }
    }

    template <int N> void activate(Tag<N> t)
    {
      if (RK2NStorageParameters<T>::isRK2n(type)) {
        rk2n->activate(t);
      } else {
        throw(EvolverTypeNotInEvolver("The activate/desactivate function is implemented only for the RK2N evolvers. "
                                      "Go implement it in the others if you need it."));
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    const EvolverType type;
    const EvolverType typeGW;

    std::shared_ptr<LeapFrog<T>> lf;
    std::shared_ptr<VelocityVerlet<T>> vv;
    std::shared_ptr<PositionVerlet<T>> pv;
    std::shared_ptr<RK2NStorage<Model>> rk2n;

    mutable bool GWsynced;

    KernelsTypes::EoM<Model> EoMKernels;
  };

} // namespace TempLat

#endif
