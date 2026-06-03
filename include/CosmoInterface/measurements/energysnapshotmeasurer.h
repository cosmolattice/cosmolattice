#ifndef COSMOINTERFACE_MEASUREMENTS_ENERGYSNAPSHOTSMEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_ENERGYSNAPSHOTSMEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/lattice/IO/fileio.h"
#include "TempLat/util/isincontainer.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "CosmoInterface/definitions/fieldfunctionals.h"
#include "CosmoInterface/definitions/energies.h"
#include "CosmoInterface/definitions/potential.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"

namespace TempLat
{
  /** @brief A class that prints snapshots of energy densities in configuration space.
   *
   **/
  template <typename Model> class EnergySnapshotsMeasurer
  {
  public:
    // Put public methods here. These should change very little over time.
    template <typename runParameters>
    EnergySnapshotsMeasurer(Model &model, runParameters &pars, FilesManager<Model::NDim> &fm,
                            std::vector<std::string> toSave)
        : mRoot(fm.getWorkingDir() + fm.getTag())
    {
      // This checks which energies are specified in the string "toSave" (passed as a parameter), and creates the
      // corresponding h5 files to save the snapshots.

#ifdef HAVE_HDF5
      saveScalar = IsInContainer::check("S", toSave);              // value of the scalar singlets
      saveScalarK = IsInContainer::check("E_S_K", toSave);         // kinetic energy of the scalar singlets
      saveScalarG = IsInContainer::check("E_S_G", toSave);         // gradient energy of the scalar singlets
      saveComplexScalar = IsInContainer::check("CS", toSave);      // modulus of the complex scalars
      saveComplexScalarK = IsInContainer::check("E_CS_K", toSave); // kinetic energy of the complex scalars
      saveComplexScalarG = IsInContainer::check("E_CS_G", toSave); // gradient energy of the complex scalars
      saveSU2DoubletK = IsInContainer::check("E_SU2D_K", toSave);  // kinetic energy of the SU(2) doublets
      saveSU2DoubletG = IsInContainer::check("E_SU2D_G", toSave);  // gradient energy of the SU(2) doublets
      saveU1El = IsInContainer::check("E_A_K", toSave);            // electric energy of the U(1) gauge sector
      saveU1Mag = IsInContainer::check("E_A_G", toSave);           // magnetic energy of the U(1) gauge sector
      saveSU2El = IsInContainer::check("E_B_K", toSave);           // electric energy of the SU(2) gauge sector
      saveSU2Mag = IsInContainer::check("E_B_G", toSave);          // magnetic energy of the SU(2) gauge sector
      savePot = IsInContainer::check("E_V", toSave);               // potential energy
      saveETotal = IsInContainer::check("E", toSave);              // total energy energy

      if (saveScalar || saveScalarK || saveScalarG || saveComplexScalar || saveComplexScalarK || saveComplexScalarG ||
          saveSU2DoubletK || saveSU2DoubletG || saveU1El || saveU1Mag || saveSU2El || saveSU2Mag || savePot ||
          saveETotal)
        fIO.setSaverLimits(pars.snapLower, pars.snapUpper, pars.snapStep);

      auto createIfFresh = [&](const std::string &name) {
        if (fm.prepareOutputFile(name)) {
          fIO.saver.create(name, Exclusive);
          fIO.saver.close();
        }
      };

      if (saveScalar) {
        nameScalar = mRoot + "snapshot_scalar_singlet.h5";
        createIfFresh(nameScalar);
      }
      if (saveScalarK) {
        nameScalarK = mRoot + "kinetic_energy_snapshot_scalar.h5";
        createIfFresh(nameScalarK);
      }
      if (saveScalarG) {
        nameScalarG = mRoot + "gradient_energy_snapshot_scalar.h5";
        createIfFresh(nameScalarG);
      }
      if (saveComplexScalar) {
        nameComplexScalar = mRoot + "snapshot_complex_scalar.h5";
        createIfFresh(nameComplexScalar);
      }
      if (saveComplexScalarK) {
        nameComplexScalarK = mRoot + "kinetic_energy_snapshot_complex_scalar.h5";
        createIfFresh(nameComplexScalarK);
      }
      if (saveComplexScalarG) {
        nameComplexScalarG = mRoot + "gradient_energy_snapshot_complex_scalar.h5";
        createIfFresh(nameComplexScalarG);
      }
      if (saveSU2DoubletK) {
        nameSU2DoubletK = mRoot + "kinetic_energy_snapshot_SU2_doublet.h5";
        createIfFresh(nameSU2DoubletK);
      }
      if (saveSU2DoubletG) {
        nameSU2DoubletG = mRoot + "gradient_energy_snapshot_SU2_doublet.h5";
        createIfFresh(nameSU2DoubletG);
      }
      if (saveU1El) {
        nameU1El = mRoot + "electric_energy_snapshot_U1.h5";
        createIfFresh(nameU1El);
      }
      if (saveU1Mag) {
        nameU1Mag = mRoot + "magnetic_energy_snapshot_U1.h5";
        createIfFresh(nameU1Mag);
      }
      if (saveSU2El) {
        nameSU2El = mRoot + "electric_energy_snapshot_SU2.h5";
        createIfFresh(nameSU2El);
      }
      if (saveSU2Mag) {
        nameSU2Mag = mRoot + "magnetic_energy_snapshot_SU2.h5";
        createIfFresh(nameSU2Mag);
      }
      if (savePot) {
        namePot = mRoot + "potential_energy_snapshot.h5";
        createIfFresh(namePot);
      }
      if (saveETotal) {
        nameETotal = mRoot + "total_energy_snapshot.h5";
        createIfFresh(nameETotal);
      }
#endif
    }

    // This saves the energy snapshots at the corresponding HDF5 files
    template <typename T> void measure(Model &model, T t)
    {
#ifdef HAVE_HDF5
      if (saveScalar) { // kinetic energy of the scalar singlets
        ForLoop(i, 0, Model::Ns - 1, fIO.saver.open(nameScalar);
                fIO.saver.save(t, model.fldS(i), "S_" + std::to_string(i)); fIO.saver.close(););
      }
      if (saveScalarK) { // kinetic energy of the scalar singlets
        ForLoop(i, 0, Model::Ns - 1, fIO.saver.open(nameScalarK); fIO.saver.save(
            t, Energies::kineticS(model, FieldFunctionals::pi2S(model, i)), "E_S_K_" + std::to_string(i));
                fIO.saver.close(););
      }
      if (saveScalarG) { // gradient energy of the scalar singlets
        ForLoop(i, 0, Model::Ns - 1, fIO.saver.open(nameScalarG); fIO.saver.save(
            t, Energies::gradientS(model, FieldFunctionals::grad2S(model, i)), "E_S_G_" + std::to_string(i));
                fIO.saver.close(););
      }
      if (saveComplexScalar) { // kinetic energy of the complex scalars
        ForLoop(i, 0, Model::NCs - 1, fIO.saver.open(nameComplexScalar);
                fIO.saver.save(t, norm(model.fldCS(i)), "CS_" + std::to_string(i)); fIO.saver.close(););
      }
      if (saveComplexScalarK) { // kinetic energy of the complex scalars
        ForLoop(i, 0, Model::NCs - 1, fIO.saver.open(nameComplexScalarK); fIO.saver.save(
            t, Energies::kineticCS(model, FieldFunctionals::pi2CS(model, i)), "E_CS_K_" + std::to_string(i));
                fIO.saver.close(););
      }
      if (saveComplexScalarG) { // gradient energy of the complex scalars
        ForLoop(i, 0, Model::NCs - 1, fIO.saver.open(nameComplexScalarG); fIO.saver.save(
            t, Energies::gradientCS(model, FieldFunctionals::grad2CS(model, i)), "E_CS_G_" + std::to_string(i));
                fIO.saver.close(););
      }
      if (saveSU2DoubletK) { // kinetic energy of the SU(2) doublets
        ForLoop(i, 0, Model::NSU2Doublet - 1,
                auto toSave = Energies::kineticSU2Doublet(model, FieldFunctionals::pi2SU2Doublet(model, i));
                fIO.saver.open(nameSU2DoubletK); fIO.saver.save(t, toSave, "E_SU2D_K_" + std::to_string(i));
                fIO.saver.close(););
      }
      if (saveSU2DoubletG) { // gradient energy of the SU(2) doublets
        ForLoop(i, 0, Model::NSU2Doublet - 1, fIO.saver.open(nameSU2DoubletG);
                fIO.saver.save(t, Energies::gradientSU2Doublet(model, FieldFunctionals::grad2SU2Doublet(model, i)),
                               "E_SU2D_G_" + std::to_string(i));
                fIO.saver.close(););
      }
      if (saveU1El) { // electric energy of the U(1) gauge sector
        ForLoop(i, 0, Model::NU1 - 1, fIO.saver.open(nameU1El); fIO.saver.save(
            t, Energies::electricU1(model, FieldFunctionals::pi2U1(model, i)), "E_A_K_" + std::to_string(i));
                fIO.saver.close(););
      }
      if (saveU1Mag) { // magnetic energy of the U(1) gauge sector
        ForLoop(i, 0, Model::NU1 - 1, fIO.saver.open(nameU1Mag); fIO.saver.save(
            t, Energies::magneticU1(model, FieldFunctionals::B2U1(model, i)), "E_A_G_" + std::to_string(i));
                fIO.saver.close(););
      }
      if (saveSU2El) { // electric energy of the SU(2) gauge sector
        ForLoop(i, 0, Model::NSU2 - 1, fIO.saver.open(nameSU2El); fIO.saver.save(
            t, Energies::electricSU2(model, FieldFunctionals::pi2SU2(model, i)), "E_B_K_" + std::to_string(i));
                fIO.saver.close(););
      }
      if (saveSU2Mag) { // magnetic energy of the SU(2) gauge sector
        ForLoop(i, 0, Model::NSU2 - 1, fIO.saver.open(nameSU2Mag); fIO.saver.save(
            t, Energies::magneticSU2(model, FieldFunctionals::B2U1(model, i)), "E_B_G_" + std::to_string(i));
                fIO.saver.close(););
      }
      if (savePot) {
        if constexpr (Model::NPotTerms == 0)
          throw(FileIOException(
              "You tried to save the potential energy, but your model does not have any potential term. Abort."));
        if constexpr (Model::NPotTerms > 0) {
          fIO.saver.open(namePot);
          fIO.saver.save(t, Potential::potential(model), "E_V");
          fIO.saver.close();
        }
      }
      if (saveETotal) {
        fIO.saver.open(nameETotal);
        fIO.saver.save(t, Energies::totalEnergy(model), "E_Total");
        fIO.saver.close();
      }
#else
      if (saveScalarK || saveScalarG || saveComplexScalarK || saveComplexScalarG || saveSU2DoubletK ||
          saveSU2DoubletG || saveU1El || saveU1Mag || saveSU2El || saveSU2Mag || savePot) {
        std::stringstream ss;
        if (saveScalar) ss << "\n- Value of the scalar singlets.";
        if (saveScalarK) ss << "\n- Kinetic energy of the scalar singlets.";
        if (saveScalarG) ss << "\n- Gradient energy of the scalar singlets.";
        if (saveComplexScalar) ss << "\n- Magnitude of the complex scalars.";
        if (saveComplexScalarK) ss << "\n- Kinetic energy of the complex scalars.";
        if (saveComplexScalarG) ss << "\n- Gradient energy of the complex scalars.";
        if (saveSU2DoubletK) ss << "\n- Kinetic energy of the SU(2) doublets.";
        if (saveSU2DoubletG) ss << "\n- Gradient energy of the SU(2) doublets.";
        if (saveU1El) ss << "\n- Electric energy of the U(1) gauge sector.";
        if (saveU1Mag) ss << "\n- Magnetic energy of the U(1) gauge sector.";
        if (saveSU2El) ss << "\n- Electric energy of the SU(2  ) gauge sector.";
        if (saveSU2Mag) ss << "\n- Magnetic energy of the SU(2) gauge sector.";
        if (savePot) ss << "\n- Potential energy.";
        if (saveETotal) ss << "\n- Total energy.";

        throw(FileIOException("You tried to save an energy snapshot to a file, but the HDF5 library is not available. "
                              "Make sure you have it installed and that you compiled CosmoLattice with it." +
                              ss.str()));
      }
#endif
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    FileIO<Model::NDim> fIO;
    std::string mRoot;

    bool saveScalar = false, saveScalarG = false, saveScalarK = false;
    bool saveComplexScalar = false, saveComplexScalarG = false, saveComplexScalarK = false;
    bool saveSU2DoubletG = false, saveSU2DoubletK = false;
    bool saveU1Mag = false, saveU1El = false;
    bool saveSU2Mag = false, saveSU2El = false;
    bool savePot = false, saveETotal = false;

    std::string nameScalar, nameScalarG, nameScalarK;
    std::string nameComplexScalar, nameComplexScalarG, nameComplexScalarK;
    std::string nameSU2DoubletG, nameSU2DoubletK;
    std::string nameU1Mag, nameU1El;
    std::string nameSU2Mag, nameSU2El;
    std::string namePot, nameETotal;
  };
} // namespace TempLat

#endif
