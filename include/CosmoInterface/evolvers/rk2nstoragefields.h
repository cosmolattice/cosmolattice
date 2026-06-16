#ifndef COSMOINTERFACE_EVOLVERS_RK2NSTORAGEFIELDS_H
#define COSMOINTERFACE_EVOLVERS_RK2NSTORAGEFIELDS_H
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
#include "CosmoInterface/extrafields.h"
#include "CosmoInterface/fieldsnumbering.h"

namespace TempLat
{

  /** \brief A class which implements low storage ("2N-storage") explicit RK methods.
   *
   *
   **/
  template <typename Model> class RK2NStorageFields
  {
  public:
    using T = typename Model::FloatType;

    /* Put public methods here. These should change very little over time. */
    RK2NStorageFields(Model &model, EvolverType eType)
        : type(eType), dt(model.dt), As(RK2NStorageParameters<T>::getAs(type)),
          Bs(RK2NStorageParameters<T>::getBs(type))
    {
      ForEachConfigurationField(Model, fld, n, isDefined[fld].emplace_back(true););
    }

    template <class KernelType> void evolve(Model &model, T tMinust0, KernelType kt)
    {

      dt = KernelsTypes::getDt(model, kt);

      kt.cache(model, tMinust0); // To be able to store some temporary info in the kernel type

      for (size_t i = 0; i < As.size(); ++i) { // loop over operations...
        ForEachConfigurationField(
            Model, fld, n,
            isDefined[fld][n] = delta(i, model.getField(TempLat::FieldsNumbering::getComplementaryFieldId(fld))(n), Kernels::get(fld, model, n, kt));
        );

        ForEachField(Model, fld, n, if (isDefined[fld][n]) { advance(i, fld, model, n); });

        kt.cache(model, tMinust0);
      }
    }

  private:
    template <class Field, class Kernel> bool delta(size_t i, Field extraFld, Kernel kernel)
    {
      if (i == 0) {
        extraFld = dt * kernel;
      } else {
        extraFld = As[i] * extraFld + dt * kernel;
      }
      return true;
    }

    template <class Delta> bool delta(size_t i, Delta delta, ZeroType kernel) { return false; }

    template <int FLD, int N> void advance(size_t i, Tag<FLD> fld, Model &model, Tag<N> n)
    {
      model.getField(fld)(n) += Bs[i] * model.getField(TempLat::FieldsNumbering::getComplementaryFieldId(fld))(n);
    }

    template <int N> void advance(size_t i, FieldsNumbering::fldSU2 fld, Model &model, Tag<N> n)
    {
      ForLoop(j, 1, Model::NDim, model.fldSU2(n)(j) = exp(Bs[i] * model.getField(TempLat::FieldsNumbering::getComplementaryFieldId(fld))(n)) * model.fldSU2(n)(j););
    }

    /* Put all member variables and private methods here. These may change arbitrarily. */

    EvolverType type;

    T dt; // Has its own dt as we can use it for other things than real time evolution (cooling for instance).

    const std::vector<T> As;
    const std::vector<T> Bs;

    std::array<std::vector<bool>, FieldsNumbering::maxNum + 1> isDefined;

    T deltaA, deltaADot;
    bool sfDefined;

    bool expansion;
  };

} // namespace TempLat

#endif
