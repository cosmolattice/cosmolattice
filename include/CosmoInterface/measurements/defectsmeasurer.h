#ifndef COSMOINTERFACE_MEASUREMENTS_DEFECTSMEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_DEFECTSMEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/measurements/meansmeasurer.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "TempLat/util/templatvector.h"
#include "CosmoInterface/definitions/energies.h"
#include "CosmoInterface/definitions/hubbleconstraint.h"
#include "CosmoInterface/definitions/fieldfunctionals.h"
#include "CosmoInterface/definitions/defectsmodule/weights.h"
#include "CosmoInterface/definitions/defectsmodule/defectsobservables.h"
#include "CosmoInterface/measurements/abstractmeasurer.h"

namespace TempLat
{
  /** @brief A class which contains measurements of energies and scale factor.
   *
   **/
  template <typename T> class DefectsMeasurer : public AbstractMeasurer
  {
  public:
    using AbstractMeasurer::lastMeas;
    // Put public methods here. These should change very little over time.
    template <typename Model>
    DefectsMeasurer(Model &model, FilesManager<Model::NDim> &filesManager, const RunParameters<T> &rPar, bool append, std::string postfix = "", bool onlyMeasureNorm = false)
        : amIRoot(model.getToolBox()->amIRoot()),
          measureDefectsEnergies(rPar.measureDefectsEnergies && !onlyMeasureNorm),
          measureDefectsStructure(rPar.measureDefectsStructure && !onlyMeasureNorm),
          normOut(filesManager, "norm" + postfix , amIRoot, append, {"t", "norm(phi)", "norm(phi)^2", "var(norm(phi))"}, !Model::DefectsModel || (Model::Ns == 0 && Model::NCs == 1) ),
          defects(filesManager, "defects" + postfix, amIRoot, append, getDefectHeaders(model), !Model::DefectsModel || !((Model::Ns > 0 && Model::NCs == 0) || (Model::Ns == 0 && Model::NCs == 1) || measureDefectsEnergies || measureDefectsStructure)), // Output file for defect observables.
          normSpectraOut(filesManager, "norm"+ postfix, amIRoot, append, rPar, !Model::DefectsModel || !(Model::Ns > 0 && Model::NCs == 0) )
    {
    }

    template <class Model> void measure(Model &model, T t)
    {

      if constexpr (Model::Ns > 0 && Model::NCs == 0) {
        T norm, norm2, varnorm;
        norm = average(sqrt(Total(i, 0, Model::Ns-1, pow<2>(model.fldS(i));)));
        norm2 = average(Total(i, 0, Model::Ns-1, pow<2>(model.fldS(i));));
        varnorm = pow<2>(norm) - norm2;

        normOut.addAverage(t);
        normOut.addAverage(norm);
        normOut.addAverage(norm2);
        normOut.addAverage(varnorm);

        normOut.save(lastMeas);
      }

      if (measureDefectsEnergies || measureDefectsStructure) defects.addAverage(t); // add to file

      // The "energies" functions contain the appropriate scale factor rescaling. Here we compute the energy species by
      // species

      T Etot = 0.;
      T Elag = 0.;
      T Ekin = 0.;
      T Egrad = 0.;
      T Epot = 0.;
      T EB = 0.;
      T EE = 0.;

      if (measureDefectsEnergies) {
        if constexpr (Model::Ns > 0 && Model::NCs == 0) {
          ForLoop(i, 0, Model::Ns - 1,
              Ekin += pow<3>(GetNGrid::get(model) * model.dx * model.aI) * average(Energies::kineticS(model, FieldFunctionals::pi2S(model, i)) * Weights::weightScalarSinglet(model));
              Egrad += pow<3>(GetNGrid::get(model) * model.dx * model.aI) * average(Energies::gradientS(model, FieldFunctionals::grad2S(model, i)) * Weights::weightScalarSinglet(model));
          );
          Epot += pow<3>(GetNGrid::get(model) * model.dx * model.aI) * average(Potential::potential(model) * Weights::weightScalarSinglet(model));
          Etot = Egrad + Ekin + Epot;
          Elag = - Egrad + Ekin - Epot;
          defects.addAverage(Ekin);
          defects.addAverage(Egrad);
          defects.addAverage(Epot);
          defects.addAverage(Etot);
          defects.addAverage(Elag);
        }

        if constexpr (Model::NCs > 0 && Model::Ns == 0) {
          ForLoop(i, 0, Model::NCs - 1,
              Ekin += pow<3>(GetNGrid::get(model) * model.dx * model.aI) * average(Energies::kineticCS(model, FieldFunctionals::pi2CS(model, i)) * Weights::weightComplexScalar(model));
              Egrad += pow<3>(GetNGrid::get(model) * model.dx * model.aI) * average(Energies::gradientCS(model, FieldFunctionals::grad2CS(model, i)) * Weights::weightComplexScalar(model));
          );
          Epot += pow<3>(GetNGrid::get(model) * model.dx * model.aI) * average(Potential::potential(model) * Weights::weightComplexScalar(model));
          defects.addAverage(Ekin);
          defects.addAverage(Egrad);
          defects.addAverage(Epot);
          if constexpr (Model::NU1 > 0) {
            ForLoop(i, 0, Model::NU1 - 1,
              EE += pow<3>(GetNGrid::get(model) * model.dx * model.aI) * average(Energies::electricU1(model, FieldFunctionals::pi2U1(model, i)) * Weights::weightComplexScalar(model));
              EB += pow<3>(GetNGrid::get(model) * model.dx * model.aI) * average(Energies::magneticU1(model, FieldFunctionals::B2U1(model, i)) * Weights::weightComplexScalar(model));
            );
              defects.addAverage(EE);
              defects.addAverage(EB);
          }
          Etot = Egrad + Ekin + Epot + EE + EB;
          Elag = - Egrad + Ekin - Epot + EE - EB;
          defects.addAverage(Etot);
          defects.addAverage(Elag);
        }

      }

      if (measureDefectsStructure) {
          if constexpr (Model::Ns == 1) { defects.addAverage(DefectsObservables::computeAreaParameter(model)); }
          if constexpr (Model::Ns == 2) { defects.addAverage(DefectsObservables::computeWindingNumberLengthScalarSinglet(model)); }
          if constexpr (Model::NCs == 1 && Model::NU1 == 1) { defects.addAverage(DefectsObservables::computeWindingNumberLengthComplexScalarU1(model)); }
      }

      defects.save(lastMeas);

    }

    template<typename Model, typename PowerSpectrumMeasurer>
    void measureSpectra(Model &model, T t, PowerSpectrumMeasurer &PSMeasurer)
    {
      if constexpr (Model::Ns > 0 && Model::NCs == 0) {
        normSpectraOut.save(lastMeas, t,
                            Total(i, 0, Model::Ns-1, PSMeasurer.powerSpectrum(model.fldS(i));),
                            pow(model.aI, 2 * model.alpha - 6) * Total(i, 0, Model::Ns-1, PSMeasurer.powerSpectrum(model.piS(i));)
        );
      }
    }

  private:
    // Returns string with the header of the energies file.
    template <typename Model> std::vector<std::string> getDefectHeaders(Model &model) const
    {
      std::vector<std::string> ret;
      ret.emplace_back("t");
      if (measureDefectsEnergies) {
        if constexpr ((Model::Ns > 0 && Model::NCs == 0) || (Model::NCs > 0 && Model::Ns == 0)) {
          ret.emplace_back("Ekin");
          ret.emplace_back("Egrad");
          ret.emplace_back("Epot");
          if constexpr ((Model::Ns == 0 && Model::NCs > 0) && Model::NU1 > 0) {
            ret.emplace_back("EE");
            ret.emplace_back("EB");
          }
          ret.emplace_back("Etot");
          ret.emplace_back("Elag");
        }
      }

      if (measureDefectsStructure) {
        if constexpr (Model::Ns == 1) { ret.emplace_back("areaDW"); }
        if constexpr (Model::Ns == 2 || (Model::NCs == 1 && Model::NU1 == 1)){ ret.emplace_back("winfindLengthStrings"); }
      }

      return ret;
    }

    /* Put all member variables and private methods here. These may change arbitrarily. */
    const bool amIRoot;
    const bool measureDefectsEnergies;
    const bool measureDefectsStructure;

    MeasurementsSaver<T> normOut;
    MeasurementsSaver<T> defects;
    SpectrumSaver<T> normSpectraOut;
  };

} // namespace TempLat

#endif
