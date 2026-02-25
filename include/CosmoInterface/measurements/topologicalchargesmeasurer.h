#ifndef COSMOINTERFACE_MEASUREMENTS_TOPOLOGICALCHARGESMEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_TOPOLOGICALCHARGESMEASURER_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Year: 2024

#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/measurements/meansmeasurer.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "TempLat/util/templatvector.h"
#include "CosmoInterface/definitions/fieldfunctionals.h"
#include "TempLat/lattice/algebra/gaugealgebra/fieldstrength.h"

namespace TempLat
{
  /** \brief A class to measure gauge fields topological charges.
   *
   *
   * Unit test: make test-topologicalchargesmeasurer
   **/
  template <typename T> class TopologicalChargesMeasurer
  {
  public:
    /* Put public methods here. These should change very little over time. */
    template <class Model>
    TopologicalChargesMeasurer(Model &model, FilesManager<Model::NDim> &filesManager, const RunParameters<T> &par,
                               bool append)
        : amIRoot(model.getToolBox()->amIRoot())
    {
      if constexpr (Model::NU1 > 0) {
        topologicalCharges = std::make_unique<MeasurementsSaver<T>>(filesManager, "topological_charges", amIRoot,
                                                                    append, getTopologyHeaders(model));
      }
    }

    template <class Model> void measure(Model &model, T t)
    {
      if constexpr (Model::NU1 > 0) {
        topologicalCharges->addAverage(t); // add to file

        // U1 gauge fields
        ForLoop(i, 0, Model::NU1 - 1, auto EB = average(FieldFunctionals::EBU1(model, i));
                auto EB2 = average(pow<2>(FieldFunctionals::EBU1(model, i))); topologicalCharges->addAverage(EB);
                topologicalCharges->addAverage(EB2););

        // SU2 gauge fields
        /*ForLoop(i, 0, Model::NSU2 - 1, auto EB = average(FieldFunctionals::TrEBSU2(model, i));
                auto EB2 = average(pow<2>(FieldFunctionals::TrEBSU2(model, i))); topologicalCharges->addAverage(EB);
                topologicalCharges->addAverage(EB2););*/

        topologicalCharges->save();
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    // Returns string with the header of the energies file.
    template <typename Model> std::vector<std::string> getTopologyHeaders(Model &model) const
    {
      std::vector<std::string> ret;
      ret.emplace_back("t");
      ForLoop(i, 0, Model::NU1 - 1, ret.emplace_back("<EB>_U1_" + std::to_string(i));
              ret.emplace_back("<(EB)^2>_U1_" + std::to_string(i)););
      // ForLoop(i, 0, Model::NSU2 - 1, ret.emplace_back("<Tr(EB)>_SU2_"); ret.emplace_back("<Tr(EB)^2>_SU2_"););

      return ret;
    }

    const bool amIRoot;

    std::unique_ptr<MeasurementsSaver<T>> topologicalCharges;
  };

} // namespace TempLat

#endif
