#ifndef COSMOINTERFACE_EVOLVERS_EVOLVER_H
#define COSMOINTERFACE_EVOLVERS_EVOLVER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/evolvers/leapfrog.h"
#include "CosmoInterface/evolvers/velocityverlet.h"
#include "CosmoInterface/evolvers/rk2nstorage.h"

#include "TempLat/util/exception.h"

namespace TempLat
{

  /** @brief A class which interfaces the evolver, so we don't need to use pointers in the main.
   *
   *
   **/

  MakeException(EvolverTypeNotInEvolver);

  template <class Model> class Evolver
  {
  public:
    // Put public methods here. These should change very little over time.
    using T = typename Model::FloatType;

    Evolver(Model &model, RunParameters<T> &rPar, ExtraFields<Model> extraFlds)
        : type(rPar.eType), lf(type == LF ? std::make_shared<LeapFrog<T>>(model, rPar) : nullptr),
          vv(VelocityVerletParameters<T>::isVerlet(type) ? std::make_shared<VelocityVerlet<T>>(model, rPar) : nullptr),
          rk2n(RK2NStorageParameters<T>::isRK2n(type) ? std::make_shared<RK2NStorage<Model>>(model, type, rPar.expansion) : nullptr)
    {
      // RK2N needs extra memory, allocated in the fields
      if (RK2NStorageParameters<T>::isRK2n(type)) {
        rk2n->setDelta(extraFlds);
      }

      if (lf == nullptr && vv == nullptr && rk2n == nullptr)
        throw(EvolverTypeNotInEvolver("The evolver type you specified was not implemented in the Evolver class, "
                                      "which dispatch between different evolvers. Abort."));
    }

    inline void evolve(Model &model, T tMinust0) const
    {
      if (type == LF) {
        lf->evolve(model, tMinust0);
      } else if (RK2NStorageParameters<T>::isRK2n(type)) {
        rk2n->evolve(model, tMinust0, EoMKernels);
      } else {
        if (!(VelocityVerletParameters<T>::isVerlet(type)))
          throw(EvolverTypeNotInEvolver("The evolver type you specified was not implemented in the Evolver class, "
                                        "which dispatch between different evolvers. Abort."));
        else
          vv->evolve(model, tMinust0);
      }
    }

    // The next function is used to synchronise all the fields to live
    // at integer time before measurements. Useful for evolvers where
    // this is not naturally the case, such as leapfrog.

    inline void sync(Model &model, T tMinust0) const
    {
      if (type == LF) {
        lf->sync(model, tMinust0);
      } else if (RK2NStorageParameters<T>::isRK2n(type)) {
        rk2n->sync(model, tMinust0);
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
      sayMPI << t ;
      if (RK2NStorageParameters<T>::isRK2n(type)) rk2n->deactivate(t);
      if (!RK2NStorageParameters<T>::isRK2n(type) ) throw(EvolverTypeNotInEvolver("The activate/desactivate function is implemented only for the RK2N evolvers. Go implement it in the others if you need it."));
    }

    template <int N> void activate(Tag<N> t)
    {
      if (RK2NStorageParameters<T>::isRK2n(type)) rk2n->activate(t);
      if (!RK2NStorageParameters<T>::isRK2n(type) ) throw(EvolverTypeNotInEvolver("The activate/desactivate function is implemented only for the RK2N evolvers. Go implement it in the others if you need it."));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    const EvolverType type;

    std::shared_ptr<LeapFrog<T>> lf;
    std::shared_ptr<VelocityVerlet<T>> vv;
    std::shared_ptr<RK2NStorage<Model>> rk2n;

    KernelsTypes::EoM<Model> EoMKernels;
  };

} // namespace TempLat

#endif
