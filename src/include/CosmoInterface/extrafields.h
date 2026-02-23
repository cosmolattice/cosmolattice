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

    FieldCollection<Field<Model::NDim, T>, Model::Ns, true> fldS;
    FieldCollection<Field<Model::NDim, T>, Model::Ns, true> piS;
    FieldCollection<Field<Model::NDim, T>, Model::Ns, true> get(FieldsNumbering::fldS) { return fldS; }
    FieldCollection<Field<Model::NDim, T>, Model::Ns, true> get(FieldsNumbering::piS) { return piS; }

    FieldCollection<ComplexField<Model::NDim, T>, Model::NCs> fldCS;
    FieldCollection<ComplexField<Model::NDim, T>, Model::NCs> piCS;
    FieldCollection<ComplexField<Model::NDim, T>, Model::NCs> get(FieldsNumbering::fldCS) { return fldCS; }
    FieldCollection<ComplexField<Model::NDim, T>, Model::NCs> get(FieldsNumbering::piCS) { return piCS; }

    FieldCollection<SU2Doublet<Model::NDim, T>, Model::NSU2Doublet> fldSU2Doublet;
    FieldCollection<SU2Doublet<Model::NDim, T>, Model::NSU2Doublet> piSU2Doublet;
    FieldCollection<SU2Doublet<Model::NDim, T>, Model::NSU2Doublet> get(FieldsNumbering::fldSU2Doublet)
    {
      return fldSU2Doublet;
    }
    FieldCollection<SU2Doublet<Model::NDim, T>, Model::NSU2Doublet> get(FieldsNumbering::piSU2Doublet)
    {
      return piSU2Doublet;
    }

    VectorFieldCollection<Field<Model::NDim, T>, Model::NU1> fldU1;
    VectorFieldCollection<Field<Model::NDim, T>, Model::NU1> piU1;
    VectorFieldCollection<Field<Model::NDim, T>, Model::NU1> get(FieldsNumbering::fldU1) { return fldU1; }
    VectorFieldCollection<Field<Model::NDim, T>, Model::NU1> get(FieldsNumbering::piU1) { return piU1; }

    VectorFieldCollection<SU2LieAlgebraField<Model::NDim, T>, Model::NSU2> fldSU2;
    VectorFieldCollection<SU2LieAlgebraField<Model::NDim, T>, Model::NSU2> piSU2;
    VectorFieldCollection<SU2LieAlgebraField<Model::NDim, T>, Model::NSU2> get(FieldsNumbering::fldSU2)
    {
      return fldSU2;
    }
    VectorFieldCollection<SU2LieAlgebraField<Model::NDim, T>, Model::NSU2> get(FieldsNumbering::piSU2) { return piSU2; }
  };

  template <typename Model> class ExtraFields
  {
  public:
    using T = typename Model::FloatType;

    /* Put public methods here. These should change very little over time. */
    ExtraFields() {}

    void allocateExtraMemory(Model &model, RunParameters<T> runPars, std::string tag)
    {
      if (RK2NStorageParameters<T>::isRK2n(runPars.eType))
        allFlds1 = std::make_shared<FieldsAsInModel<Model>>(model, runPars, tag);
      else if (model.getU1IC() == InitialConditionsType::PlaneWaves ||
               model.getU1IC() == InitialConditionsType::PlaneWavesZeroB) {
        fldU1IC = std::make_shared<VectorField<Field<Model::NDim, T>>>("U1ICfldU1", model.getToolBox(),
                                                                       runPars.getLatParams());
        piU1IC = std::make_shared<VectorField<Field<Model::NDim, T>>>("U1ICpiU1", model.getToolBox(),
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
    std::shared_ptr<VectorField<Field<Model::NDim, T>>> fldU1IC;
    std::shared_ptr<VectorField<Field<Model::NDim, T>>> piU1IC;
  };

} // namespace TempLat

#endif
