#ifndef COSMOINTERFACE_EXTRAFIELDS_H
#define COSMOINTERFACE_EXTRAFIELDS_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Year: 2025

#include "CosmoInterface/fieldsnumbering.h"
#include "CosmoInterface/evolvers/rk2nstorageparameters.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/initializers/initialconditionstype.h"
#include "TempLat/lattice/field/collections/fieldcollection.h"
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/lattice/algebra/su2algebra/su2doublet.h"
#include "TempLat/lattice/algebra/su2algebra/su2liealgebrafield.h"

namespace TempLat
{
  MakeException(ExtraMemoryNotAllocated);

  /** \brief A class which holds extra fields, so that they can be used across the program.
   *
   *
   **/
  template <typename Model> class FieldsAsInModel
  {
  public:
    using T = typename Model::FloatType;

    FieldsAsInModel(Model &model, RunParameters<T> runParams, std::string nameTag)
        : fldS(nameTag + "fldS", model.getToolBox(), runParams.getLatParams()),
          piS(nameTag + "piS", model.getToolBox(), runParams.getLatParams()),
          fldCS(nameTag + "fldCS", model.getToolBox(), runParams.getLatParams()),
          piCS(nameTag + "piCS", model.getToolBox(), runParams.getLatParams()),
          fldSU2Doublet(nameTag + "fldSU2Doublet", model.getToolBox(), runParams.getLatParams()),
          piSU2Doublet(nameTag + "piSU2Doublet", model.getToolBox(), runParams.getLatParams()),
          fldU1(nameTag + "fldU1", model.getToolBox(), runParams.getLatParams()),
          piU1(nameTag + "piU1", model.getToolBox(), runParams.getLatParams()),
          fldSU2(nameTag + "fldSU2", model.getToolBox(), runParams.getLatParams()),
          piSU2(nameTag + "piSU2", model.getToolBox(), runParams.getLatParams())
    {
    }

    FieldCollection<Field<T, Model::NDim>, Model::Ns, true> fldS;
    FieldCollection<Field<T, Model::NDim>, Model::Ns, true> piS;

    FieldCollection<ComplexField<T, Model::NDim>, Model::NCs> fldCS;
    FieldCollection<ComplexField<T, Model::NDim>, Model::NCs> piCS;

    FieldCollection<SU2Doublet<T, Model::NDim>, Model::NSU2Doublet> fldSU2Doublet;
    FieldCollection<SU2Doublet<T, Model::NDim>, Model::NSU2Doublet> piSU2Doublet;

    VectorFieldCollection<Field<T, Model::NDim>, Model::NU1> fldU1;
    VectorFieldCollection<Field<T, Model::NDim>, Model::NU1> piU1;

    VectorFieldCollection<SU2LieAlgebraField<T, Model::NDim>, Model::NSU2> fldSU2;
    VectorFieldCollection<SU2LieAlgebraField<T, Model::NDim>, Model::NSU2> piSU2;

    template <int N> auto get(Tag<N>)
    {
      if constexpr (N == FieldsNumbering::fldS::value)
        return fldS;
      else if constexpr (N == FieldsNumbering::piS::value)
        return piS;
      else if constexpr (N == FieldsNumbering::fldCS::value)
        return fldCS;
      else if constexpr (N == FieldsNumbering::piCS::value)
        return piCS;
      else if constexpr (N == FieldsNumbering::fldSU2Doublet::value)
        return fldSU2Doublet;
      else if constexpr (N == FieldsNumbering::piSU2Doublet::value)
        return piSU2Doublet;
      else if constexpr (N == FieldsNumbering::fldU1::value)
        return fldU1;
      else if constexpr (N == FieldsNumbering::piU1::value)
        return piU1;
      else if constexpr (N == FieldsNumbering::fldSU2::value)
        return fldSU2;
      else if constexpr (N == FieldsNumbering::piSU2::value)
        return piSU2;
    }
  };

  template <typename Model> class ExtraFields
  {
  public:
    using T = typename Model::FloatType;

    /* Put public methods here. These should change very little over time. */
    ExtraFields() = default;

    void allocateExtraMemory(Model &model, RunParameters<T> runPars, std::string tag)
    {
      // TODO: This logic is wrong as it has been before, we should discuss this.

      if (RK2NStorageParameters<T>::isRK2n(runPars.eType))
        allFlds1 = std::make_shared<FieldsAsInModel<Model>>(model, runPars, tag);
      if constexpr (Model::NU1 > 0)
        if (model.getU1IC() == InitialConditionsType::U1::PlaneWaves ||
            model.getU1IC() == InitialConditionsType::U1::PlaneWavesZeroB) {
          fldU1IC = std::make_shared<VectorField<Field<T, Model::NDim>>>("U1ICfldU1", model.getToolBox(),
                                                                         runPars.getLatParams());
          piU1IC = std::make_shared<VectorField<Field<T, Model::NDim>>>("U1ICpiU1", model.getToolBox(),
                                                                        runPars.getLatParams());
        }
    }

    std::shared_ptr<FieldsAsInModel<Model>> getAllFlds1() { return allFlds1; }

    auto fldForPlaneWavesU1()
    {
      if (allFlds1 != nullptr)
        return allFlds1->fldU1(0_c);
      else if (fldU1IC != nullptr)
        return *fldU1IC;
      else {
        throw(ExtraMemoryNotAllocated("Extra memory required by fldForPlaneWaves is not allocated, will crash."));
        return *fldU1IC;
      };
    }

    auto piForPlaneWavesU1()
    {
      if (allFlds1 != nullptr)
        return allFlds1->piU1(0_c);
      else if (piU1IC != nullptr)
        return *piU1IC;
      else {
        throw(ExtraMemoryNotAllocated("Extra memory required by fldForPlaneWaves is not allocated, will crash."));
        return *piU1IC;
      };
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::shared_ptr<FieldsAsInModel<Model>> allFlds1;
    std::shared_ptr<VectorField<Field<T, Model::NDim>>> fldU1IC;
    std::shared_ptr<VectorField<Field<T, Model::NDim>>> piU1IC;
  };

} // namespace TempLat

#endif
