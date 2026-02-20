#ifndef COSMOINTERFACE_EXTRAFIELDS_H
#define COSMOINTERFACE_EXTRAFIELDS_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Year: 2025

#include "CosmoInterface/fieldsnumbering.h"
#include "CosmoInterface/evolvers/rk2nstorageparameters.h"
#include "CosmoInterface/runparameters.h"

namespace TempLat {


    /** \brief A class which holds extra fields, so that they can be used across the program.
     *
     *
     **/
    MakeException(ExtraMemoryNotAllocated);

    template<typename Model>
    class FieldsAsInModel{
    public:

        using T = typename Model::FloatType;

        FieldsAsInModel(Model& model, RunParameters<T> runParams, std::string nameTag) :
        fldS(nameTag + "fldS", model.getToolBox(), runParams.getLatParams()),
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

        FieldCollection<Field<Model::NDim, T>, T, Model::Ns, true> fldS;
        FieldCollection<Field<Model::NDim, T>, T, Model::Ns, true> piS;
        FieldCollection<Field<Model::NDim, T>, T, Model::Ns, true> get(FieldsNumbering::fldS) { return fldS; }
        FieldCollection<Field<Model::NDim, T>, T, Model::Ns, true> get(FieldsNumbering::piS) { return piS; }

        FieldCollection<ComplexField<Model::NDim, T>, T, Model::NCs> fldCS;
        FieldCollection<ComplexField<Model::NDim, T>, T, Model::NCs> piCS;
        FieldCollection<ComplexField<Model::NDim, T>, T, Model::NCs> get(FieldsNumbering::fldCS) { return fldCS; }
        FieldCollection<ComplexField<Model::NDim, T>, T, Model::NCs> get(FieldsNumbering::piCS) { return piCS; }

        FieldCollection<SU2Doublet<Model::NDim, T>, T, Model::NSU2Doublet> fldSU2Doublet;
        FieldCollection<SU2Doublet<Model::NDim, T>, T, Model::NSU2Doublet> piSU2Doublet;
        FieldCollection<SU2Doublet<Model::NDim, T>, T, Model::NSU2Doublet> get(FieldsNumbering::fldSU2Doublet) { return fldSU2Doublet; }
        FieldCollection<SU2Doublet<Model::NDim, T>, T, Model::NSU2Doublet> get(FieldsNumbering::piSU2Doublet) { return piSU2Doublet; }

        VectorFieldCollection<Field<Model::NDim, T>, T, Model::NDim, Model::NU1> fldU1;
        VectorFieldCollection<Field<Model::NDim, T>, T, Model::NDim, Model::NU1> piU1;
        VectorFieldCollection<Field<Model::NDim, T>, T, Model::NDim, Model::NU1> get(FieldsNumbering::fldU1) { return fldU1; }
        VectorFieldCollection<Field<Model::NDim, T>, T, Model::NDim, Model::NU1> get(FieldsNumbering::piU1) { return piU1; }

        VectorFieldCollection<SU2LieAlgebraField<Model::NDim, T>, T, Model::NDim, Model::NSU2> fldSU2;
        VectorFieldCollection<SU2LieAlgebraField<Model::NDim, T>, T, Model::NDim, Model::NSU2> piSU2;
        VectorFieldCollection<SU2LieAlgebraField<Model::NDim, T>, T, Model::NDim, Model::NSU2> get(FieldsNumbering::fldSU2) { return fldSU2; }
        VectorFieldCollection<SU2LieAlgebraField<Model::NDim, T>, T, Model::NDim, Model::NSU2> get(FieldsNumbering::piSU2) { return piSU2; }
     };

    template<typename Model>
    class ExtraFields {
    public:
        using T = typename Model::FloatType;

        /* Put public methods here. These should change very little over time. */
        ExtraFields() {

        }

        void allocateExtraMemory(Model& model, RunParameters<T> runPars, std::string tag)
        {
            if (RK2NStorageParameters<T>::isRK2n(runPars.eType))
                allFlds1 = std::make_shared<FieldsAsInModel<Model>>(model, runPars, tag);
        }

        std::shared_ptr<FieldsAsInModel<Model>> getAllFlds1()
        {
            return allFlds1;
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::shared_ptr<FieldsAsInModel<Model>> allFlds1;

    };



} /* TempLat */

#endif
