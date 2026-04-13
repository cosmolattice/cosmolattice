#ifndef COSMOINTERFACE_MEASUREMENTS_GWSMEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_GWSMEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros, Adrien Florio, Nicolás Layza,  Year: 2022

#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "CosmoInterface/measurements/measurementsIO/measurementssaver.h"
#include "TempLat/util/templatvector.h"
#include "CosmoInterface/measurements/abstractmeasurer.h"
#include "CosmoInterface/measurements/powerspectrum.h"
#include "CosmoInterface/measurements/unbinnedpowerspectrum.h"

namespace TempLat
{
  /** @brief A class which contains standard measurements for the GWs sector.
   *
   *
   * Unit test: ctest -R test-gwsmeasurer
   **/
  template <typename T> class GWsMeasurer : public AbstractMeasurer
  {
  public:
    using AbstractMeasurer::lastMeas;
    // Put public methods here. These should change very little over time.
    template <typename Model>
    GWsMeasurer(Model &model, FilesManager<Model::NDim> &filesManager, const RunParameters<T> &par, bool append):
    amIRoot(model.getToolBox()->amIRoot()),
    energyOut(filesManager, "energies_gws", amIRoot, append, getGWEnergyHeaders(model), model.fldGWs == nullptr),
    spectraOut(filesManager, "energy_gws", amIRoot, append, par, model.fldGWs == nullptr),
    PRJType(par.GWprojectorType)
    { }

    template <typename Model, typename PowerSpectrumMeasurer> void measureSpectra(Model &model, T t, PowerSpectrumMeasurer &PSMeasurer)
    {
      if (model.fldGWs != nullptr) {
        auto GWspectrum = PSMeasurer.powerSpectrumGW(model, PRJType);
        spectraOut.save(lastMeas, t, GWspectrum);
        energyOut.addAverage(t);
        auto fracEgws = GWspectrum.integrate(PRJType == 1);
        energyOut.addAverage(fracEgws);
        energyOut.addAverage(fracEgws * Energies::rho(model));
        energyOut.save(lastMeas);
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    template <typename Model> std::vector<std::string> getGWEnergyHeaders(Model &model) const
    {
      std::vector<std::string> ret;
      ret.emplace_back("t");
      ret.emplace_back("EGW / Ematter");
      ret.emplace_back("EGW");

      return ret;
    }

    bool amIRoot;
    MeasurementsSaver<T> energyOut;
    SpectrumSaver<T> spectraOut;
    size_t PRJType;

  };

} // namespace TempLat

#endif
