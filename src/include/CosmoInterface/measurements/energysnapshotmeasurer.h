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

namespace TempLat {

    /** \brief A class that prints snapshots of energy densities in configuration space.
     *
     **/

    class EnergySnapshotsMeasurer {
    public:
        /* Put public methods here. These should change very little over time. */
        template <typename Model>
        EnergySnapshotsMeasurer(Model& model, FilesManager& fm, std::vector<std::string> toSave) :
        mRoot(fm.getTag()+fm.getWorkingDir())
        {
        	// This checks which energies are specified in the string "toSave" (passed as a parameter), and creates the corresponding h5 files to save the snapshots.
        
            saveScalarK =  IsInContainer::check("E_S_K", toSave);  // kinetic energy of the scalar singlets
            saveScalarG =  IsInContainer::check("E_S_G", toSave);  // gradient energy of the scalar singlets
            saveComplexScalarK =  IsInContainer::check("E_CS_K", toSave);  // kinetic energy of the complex scalars
            saveComplexScalarG =  IsInContainer::check("E_CS_G", toSave);  // gradient energy of the complex scalars
            saveSU2DoubletK =  IsInContainer::check("E_SU2D_K", toSave);  // kinetic energy of the SU(2) doublets
            saveSU2DoubletG =  IsInContainer::check("E_SU2D_G", toSave);  // gradient energy of the SU(2) doublets
            saveU1El =  IsInContainer::check("E_A_K", toSave);  // electric energy of the U(1) gauge sector
            saveU1Mag =  IsInContainer::check("E_A_G", toSave);  // magnetic energy of the U(1) gauge sector
            saveSU2El =  IsInContainer::check("E_B_K", toSave);  // electric energy of the SU(2) gauge sector
            saveSU2Mag =  IsInContainer::check("E_B_G", toSave);  // magnetic energy of the SU(2) gauge sector
            savePot =  IsInContainer::check("E_V", toSave); // potential energy


            if(saveScalarK) {
                nameScalarK = mRoot + "kinetic_energy_snapshot_scalar.h5"; // name of the file
                fIO.saver.create( nameScalarK  );
                fIO.saver.close();
            }
            if(saveScalarG) {
                nameScalarG = mRoot + "gradient_energy_snapshot_scalar.h5"; // name of the file

                fIO.saver.create( nameScalarG );
                fIO.saver.close();
            }
            if(saveComplexScalarK) {
                nameComplexScalarK = mRoot + "kinetic_energy_snapshot_complex_scalar.h5"; // name of the file

                fIO.saver.create( nameComplexScalarK );
                fIO.saver.close();
            }
            if(saveComplexScalarG) {
                nameComplexScalarG = mRoot + "gradient_energy_snapshot_complex_scalar.h5"; // name of the file

                fIO.saver.create( nameComplexScalarG );
                fIO.saver.close();
            }
            if(saveSU2DoubletK) {
                nameSU2DoubletK = mRoot + "kinetic_energy_snapshot_SU2_doublet.h5"; // name of the file

                fIO.saver.create( nameSU2DoubletK );
                fIO.saver.close();
            }
            if(saveSU2DoubletG) {
                nameSU2DoubletG = mRoot + "gradient_energy_snapshot_SU2_doublet.h5"; // name of the file

                fIO.saver.create( nameSU2DoubletG );
                fIO.saver.close();
            }
            if(saveU1El) {
                nameU1El = mRoot + "electric_energy_snapshot_U1.h5"; // name of the file

                fIO.saver.create( nameU1El );
                fIO.saver.close();
            }
            if(saveU1Mag) {
                nameU1Mag = mRoot + "magnetic_energy_snapshot_U1.h5"; // name of the file

                fIO.saver.create( nameU1Mag );
                fIO.saver.close();
            }
            if(saveSU2El) {
                nameSU2El = mRoot + "electric_energy_snapshot_SU2.h5"; // name of the file

                fIO.saver.create( nameSU2El );
                fIO.saver.close();
            }
            if(saveSU2Mag) {
                nameSU2Mag = mRoot + "magnetic_energy_snapshot_SU2.h5"; // name of the file

                fIO.saver.create( nameSU2Mag );
                fIO.saver.close();
            }
            if(savePot) {
                namePot = mRoot + "potential_energy_snapshot.h5"; // name of the file
                fIO.saver.create( namePot );
                fIO.saver.close();
            }

        }

		 // This saves the energy snapshots at the corresponding HDF5 files
        template <class Model, typename T>
        void measure(Model& model, T t)
        {
            if(saveScalarK) {   // kinetic energy of the scalar singlets
                ForLoop(i, 0, Model::Ns -1,
                                fIO.saver.open( nameScalarK );
                                fIO.saver.save(t, Energies::kineticS(model,FieldFunctionals::pi2S(model,i)), "E_S_K_" + std::to_string(i));
                                fIO.saver.close();
                        );
            }
            if(saveScalarG) {   // gradient energy of the scalar singlets
                ForLoop(i, 0, Model::Ns -1,
                                fIO.saver.open( nameScalarG );
                                fIO.saver.save(t, Energies::gradientS(model,FieldFunctionals::grad2S(model,i)), "E_S_G_" + std::to_string(i));
                                fIO.saver.close();
                                );
            }
            if(saveComplexScalarK) {   // kinetic energy of the complex scalars
                ForLoop(i, 0, Model::NCs -1,
                                fIO.saver.open( nameComplexScalarK );
                                fIO.saver.save(t, Energies::kineticCS(model,FieldFunctionals::pi2CS(model,i)), "E_CS_K_" + std::to_string(i));
                                fIO.saver.close();
                                );
            }
            if(saveComplexScalarG) {   // gradient energy of the complex scalars
                ForLoop(i, 0, Model::NCs -1,
                                fIO.saver.open( nameComplexScalarG );
                                fIO.saver.save(t, Energies::gradientCS(model,FieldFunctionals::grad2CS(model,i)), "E_CS_G_" + std::to_string(i));
                                fIO.saver.close();
                                );
            }
            if(saveSU2DoubletK) {   // kinetic energy of the SU(2) doublets
                ForLoop(i, 0, Model::NSU2Doublet -1,
                                auto toSave = Energies::kineticSU2Doublet(model,FieldFunctionals::pi2SU2Doublet(model,i));
				                fIO.saver.open( nameSU2DoubletK );
                                fIO.saver.save(t, toSave, "E_SU2D_K_" + std::to_string(i));
                                fIO.saver.close();
                                );
            }
            if(saveSU2DoubletG) {   // gradient energy of the SU(2) doublets
                ForLoop(i, 0, Model::NSU2Doublet -1,
                                fIO.saver.open( nameSU2DoubletG );
                                fIO.saver.save(t, Energies::gradientSU2Doublet(model,FieldFunctionals::grad2SU2Doublet(model,i)),"E_SU2D_G_" + std::to_string(i));
                                fIO.saver.close();
                                );
            }
            if(saveU1El) {   // electric energy of the U(1) gauge sector
                ForLoop(i, 0, Model::NU1 -1,
                                fIO.saver.open( nameU1El );
                                fIO.saver.save(t, Energies::electricU1(model,FieldFunctionals::pi2U1(model,i)), "E_A_K_" + std::to_string(i));
                                fIO.saver.close();
                                );
            }
            if(saveU1Mag) {   // magnetic energy of the U(1) gauge sector
                ForLoop(i, 0, Model::NU1 -1,
                                fIO.saver.open( nameU1Mag );
                                fIO.saver.save(t, Energies::magneticU1(model,FieldFunctionals::B2U1(model,i)), "E_A_G_" + std::to_string(i));
                                fIO.saver.close();
                                );
            }
            if(saveSU2El) {   // electric energy of the SU(2) gauge sector
                ForLoop(i, 0, Model::NSU2 -1,
                                fIO.saver.open( nameSU2El );
                                fIO.saver.save(t, Energies::electricSU2(model,FieldFunctionals::pi2SU2(model,i)), "E_B_K_" + std::to_string(i));
                                fIO.saver.close();
                                );
            }
            if(saveSU2Mag) {   // magnetic energy of the SU(2) gauge sector
                ForLoop(i, 0, Model::NSU2 -1,
                                fIO.saver.open( nameSU2Mag );
                                fIO.saver.save(t, Energies::magneticSU2(model,FieldFunctionals::B2U1(model,i)), "E_B_G_" + std::to_string(i));
                                fIO.saver.close();
                                );
            }
            if(savePot) {
                fIO.saver.open( namePot );
                fIO.saver.save(t, Potential::potential(model), "E_V");
                fIO.saver.close();
            }
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        FileIO fIO;
        std::string mRoot;

        bool saveScalarG, saveScalarK;
        bool saveComplexScalarG, saveComplexScalarK;
        bool saveSU2DoubletG, saveSU2DoubletK;
        bool saveU1Mag, saveU1El;
        bool saveSU2Mag, saveSU2El;
        bool savePot;

        std::string nameScalarG, nameScalarK;
        std::string nameComplexScalarG, nameComplexScalarK;
        std::string nameSU2DoubletG, nameSU2DoubletK;
        std::string nameU1Mag, nameU1El;
        std::string nameSU2Mag, nameSU2El;
        std::string namePot;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/energysnapshotsmeasurer_test.h"
#endif


#endif
