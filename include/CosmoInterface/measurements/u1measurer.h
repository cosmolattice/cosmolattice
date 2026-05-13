#ifndef COSMOINTERFACE_MEASUREMENTS_U1MEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_U1MEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/measurements/meansmeasurer.h"
#include "CosmoInterface/measurements/measurementsIO/spectrumsaver.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "TempLat/util/templatvector.h"
#include "TempLat/util/function.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/lattice/algebra/gaugealgebra/magneticfield.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "CosmoInterface/measurements/powerspectrum.h"
#include "CosmoInterface/definitions/gausslaws.h"
#include "CosmoInterface/measurements/abstractmeasurer.h"
#include "TempLat/lattice/algebra/spatialderivatives/forwdiff.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

namespace TempLat
{
  /** @brief A class which contains standard measurements for the U1 gauge fields.
   *
   *
   **/
  template <typename T> class U1Measurer : public AbstractMeasurer
  {
  public:
    using AbstractMeasurer::lastMeas;
    // Put public methods here. These should change very little over time.
    template <typename Model>
    U1Measurer(Model &model, FilesManager<Model::NDim> &filesManager, const RunParameters<T> &par, bool append)
        : flagChiralPS(par.flagChiralPS)
    {
      bool amIRoot = model.getToolBox()->amIRoot();

      ForLoop(i, 0, Model::NU1 - 1,

              // We create three files for each U(1) gauge field:

              standardNormOut.emplace_back(MeasurementsSaver<T>(
                  filesManager, "norm_U1_" + std::to_string(i), amIRoot, append,
                  MeansMeasurer::headerEB())); // Contains volume-averages of the electric and magnetic fields: norm
                                               // squared, norm to the fourth, and variances

              gauss.emplace_back(
                  MeasurementsSaver<T>(filesManager, "gauss_U1_" + std::to_string(i), amIRoot, append,
                                       {"t", "var(LHS-RHS)_over_var(LHS+RHS)", "var(LHS)",
                                        "var(RHS)"})); // Checks the degree of conservation of the U(1) gauss law.

              spectra.emplace_back(SpectrumSaver<T>(filesManager, "norm_U1_" + std::to_string(i), amIRoot, append,
                                                    par)); // Contains the spectra of the electric and magnetic fields.
              if (flagChiralPS)
              {
                chiralSpectraA.emplace_back(SpectrumSaver<T>(filesManager, "chiral_U1_" + std::to_string(i), amIRoot, append,par));
                chiralSpectraE.emplace_back(SpectrumSaver<T>(filesManager, "chiral_Elec_U1_" + std::to_string(i), amIRoot, append,par));
              }
              );
    }

    // This measures the corresponding averages with MeansMeasurer::measure, and add them to the files.
    // NOTE: For gauge fields, their momenta is defined as pi=a^(alpha-1)*A'_i, with A'_i the electric field.
    template <typename Model> void measureStandard(Model &model, T t)
    {
      ForLoop(i, 0, Model::NU1 - 1,
              MeansMeasurer::measure(standardNormOut(i), norm(model.piU1(i) * pow(model.aI, model.alpha - 1)),
                                     norm(magneticField(model.fldU1(i))), t);
              standardNormOut(i).save(lastMeas);

              gauss(i).addAverage(t); // adds time to the Gauss law file
              auto gaussU1Arr =
                  GaussLaws::checkU1(model, i);   // the function returns a 3-component vector with information of the
                                                  // left and right hand sides of the Gauss law.
              gauss(i).addAverage(gaussU1Arr(0)); // var(LHS - RHS)_over_var(LHS + RHS),
              gauss(i).addAverage(gaussU1Arr(1)); // var(LHS),
              gauss(i).addAverage(gaussU1Arr(2)); // and var(RHS)
              gauss(i).save(lastMeas););
    }

    // This measures the electric and magnetic spectra and adds them to the files.
    template <typename Model, typename PowerSpectrumMeasurer> void measureSpectra(Model &model, T t, PowerSpectrumMeasurer &PSMeasurer)
    {
      ForLoop(
          k, 0, Model::NU1 - 1, const auto &A = model.fldU1(k);
          const auto B1 = forwDiff(A(2_c), 3_c) - forwDiff(A(3_c), 2_c);
          const auto B2 = forwDiff(A(3_c), 1_c) - forwDiff(A(1_c), 3_c);
          const auto B3 = forwDiff(A(1_c), 2_c) - forwDiff(A(2_c), 1_c);
          auto magSpecU1 = PSMeasurer.powerSpectrum(B1) + PSMeasurer.powerSpectrum(B2) + PSMeasurer.powerSpectrum(B3);
          auto elSpecU1 =
              Total(i, 1, Model::NDim, pow(model.aI, 2 * model.alpha - 2) * PSMeasurer.powerSpectrum(model.piU1(k)(i)));
          spectra(k).save(lastMeas, t, elSpecU1, magSpecU1);

          if (flagChiralPS)
          {
            auto Aplus  = Total(i, 1, Model::NDim, PSMeasurer.chiralPowerSpectrumU1(model, k, i, true,  true));
            auto Aminus = Total(i, 1, Model::NDim, PSMeasurer.chiralPowerSpectrumU1(model, k, i, false, true));
            auto Eplus  = Total(i, 1, Model::NDim, pow(model.aI, 2 * model.alpha - 2) * PSMeasurer.chiralPowerSpectrumU1(model, k, i, true,  false));
            auto Eminus = Total(i, 1, Model::NDim, pow(model.aI, 2 * model.alpha - 2) * PSMeasurer.chiralPowerSpectrumU1(model, k, i, false, false));

            chiralSpectraA(k).save(lastMeas, t, Aplus, Aminus);
            chiralSpectraE(k).save(lastMeas, t, Eplus, Eminus);
          });

    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    TempLatVector<MeasurementsSaver<T>> standardNormOut;
    TempLatVector<MeasurementsSaver<T>> gauss;
    TempLatVector<SpectrumSaver<T>> spectra;

    bool flagChiralPS;
    TempLatVector<SpectrumSaver<T>> chiralSpectraA;
    TempLatVector<SpectrumSaver<T>> chiralSpectraE;
  };
} // namespace TempLat

#endif
