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
#include "CosmoInterface/measurements/powerspectrumgws.h"





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
    GWsMeasurer(Model &model, FilesManager<Model::NDim> &filesManager, const RunParameters<T> &par, bool append)
    {
      bool amIRoot = model.getToolBox()->amIRoot();
      // We create a file containing the spectra
      spectraOut.emplace_back(
          SpectrumSaver<T>(filesManager, "gws", amIRoot, append, par, !model.fldGWs)); // File for spectra
    }

    template <typename Model> void measureSpectra(Model &model, T t, GWPowerSpectrumMeasurer<T> &GWPSMeasurer)
    {
      if (model.fldGWs != nullptr) spectraOut(0).save(lastMeas, t, GWPSMeasurer.powerSpectrumGW(model));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    TempLatVector<SpectrumSaver<T>> spectraOut;

  };

} // namespace TempLat

#endif
