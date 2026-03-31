#ifndef COSMOINTERFACE_MEASUREMENTS_SCALARMEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_SCALARMEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/measurements/meansmeasurer.h"
#include "CosmoInterface/measurements/measurementsIO/spectrumsaver.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "TempLat/util/templatvector.h"
#include "CosmoInterface/measurements/powerspectrum.h"
#include "CosmoInterface/measurements/occupationnumber.h"
#include "CosmoInterface/measurements/abstractmeasurer.h"

namespace TempLat
{
  /** @brief A class which contains standard measurements for scalar singlets.
   *
   **/
  template <typename T> class ScalarSingletMeasurer : public AbstractMeasurer
  {
  public:
    using AbstractMeasurer::lastMeas;
    // Put public methods here. These should change very little over time.
    template <typename Model>
    ScalarSingletMeasurer(Model &model, FilesManager<Model::NDim> &filesManager, const RunParameters<T> &par,
                          bool append)
        : ONMeasurer(par), flagON(par.flagON), PSType(par.powerSpectrumType)
    {
      bool amIRoot = model.getToolBox()->amIRoot();
      // We create two files for each singlet, one containing
      // the volume averages (mean, rms, etc), and another for the spectra
      // Files are created with the MeasurementsSaver function:
      // the second parameter is added to the file name; the fifth one
      // adds a header to the file
      ForLoop(i, 0, Model::Ns - 1,
              standardOut.emplace_back(
                  MeasurementsSaver<T>(filesManager, model.fldS(i), amIRoot, append, MeansMeasurer::header()));
              // File for volume-averages
              spectraOut.emplace_back(SpectrumSaver<T>(filesManager, model.fldS(i), amIRoot, append, par));
              // File for spectra
              if(flagON) {
                ONOut.emplace_back(
                  SpectrumSaver<T>(filesManager, "ON_scalar_" + std::to_string(i), amIRoot, append, par)
                );
                // File for occupation number
              }
      );
    }

    // The following function measures the corresponding averages
    // with MeansMeasurer::measure, and adds them to the files.
    // NOTE: For scalar fields, the momenta is defined as pi=a^(3-alpha)*f',
    // with f' the time-derivative; see eq.(220) of arXiv:2006.15122v2.
    template <typename Model> void measureStandard(Model &model, T t)
    {
      ForLoop(i, 0, Model::Ns - 1,
              MeansMeasurer::measure(standardOut(i), model.fldS(i), model.piS(i) * pow(model.aI, model.alpha - 3), t);
              standardOut(i).save(lastMeas););
    }

    // The following function measures the spectra of the norm and its time-derivative.
    template <typename Model, typename PowerSpectrumMeasurer> // The occupation number is only measured if the user indicates it. The current version
                              // requires one additional field to measure it (JBB, Nov 2023).
    void measureSpectra(Model &model, T t, PowerSpectrumMeasurer &PSMeasurer)
    {
      ForLoop(i, 0, Model::Ns - 1,
              spectraOut(i).save(lastMeas, t,
                                 PSMeasurer.powerSpectrum(model.fldS(i)),
                                 pow(model.aI, 2 * model.alpha - 6) * PSMeasurer.powerSpectrum(model.piS(i))
              );
              if (flagON) ONOut(i).save(lastMeas, t, ONMeasurer.occupationNumber(model, i));
      );
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    TempLatVector<MeasurementsSaver<T>> standardOut;
    TempLatVector<SpectrumSaver<T>> spectraOut;
    TempLatVector<SpectrumSaver<T>> ONOut;

    OccupationNumberMeasurer ONMeasurer;
    bool flagON;

    const int PSType;
  };

} // namespace TempLat

#endif
