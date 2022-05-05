#ifndef COSMOINTERFACE_SIMULATIONMANAGER_H
#define COSMOINTERFACE_SIMULATIONMANAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "CosmoInterface/definitions/averages.h"
#include "TempLat/util/cstyletime.h"
#include "TempLat/lattice/IO/fileio.h"
#include "CosmoInterface/abstractmodel.h"
#include "CosmoInterface/runparameters.h"
#include "TempLat/util/conditionaloutput/conditionalfilestream.h"

namespace TempLat {


    /** \brief A class which handles start and stop of a simulation. Also used to create the info file.
     *
     *
     **/


    class SimulationManager {
    public:
        /* Put public methods here. These should change very little over time. */

        // This is where we decide whether the simulation is running in restart mode or not. We start by
        // reading the "load_file" argument. If the user provides one, then it means they want to restart the simulation.
        SimulationManager(ParameterParser& parser) :
        restart(parser.get<std::string>("load_file", Constants::defaultString)()) {
            boolRestart = restart != Constants::defaultString;

            // If we are not restarting, we simply delete "load_file" from the parameter parser.
            if(restart == Constants::defaultString) parser.erase("load_file");
            // We set the parser in "override mode", this mechanisms allows the user to specify new parameters over the old saved ones.
            if(boolRestart) parser.activateOverride();
        }

        // TODO: A potential test to do is to save the averages in the h5 as well and compute them upon loading
        // and crash if they are different.


        // In restart mode, we start by loading the former parameters in the paameter parser.
        void getParams(ParameterParser& parser)
        {
          fIO.loader.open(restart);
          fIO.loader.load(parser);
          fIO.loader.close();
        }

        // Function which loads from file all the field variables when running in restart mode.
        template<typename T, class Model>
        void loadSim(Model& model, T& t0)
        {
            fIO.loader.open(restart);

            ForLoop(i, 0, Model::Ns - 1,
                    fIO.load(model.fldS(i));
                            fIO.load(model.piS(i));
            );
            
            if (model.fldGWs != nullptr) {
                ForLoop(i, 0, Model::NGWs - 1,
                    fIO.load((*model.fldGWs)(i));
                    fIO.load((*model.piGWs)(i));
            ); }

            ForLoop(i, 0, Model::NCs -1,
                    fIO.load(model.fldCS(i));
                            fIO.load(model.piCS(i));
            );

            ForLoop(i, 0, Model::NSU2Doublet -1,
                    fIO.load(model.fldSU2Doublet(i));
                            fIO.load(model.piSU2Doublet(i));
            );

            ForLoop(i, 0, Model::NU1 -1,
                    fIO.load(model.fldU1(i));
                            fIO.load(model.piU1(i));
            );

            ForLoop(i, 0, Model::NSU2 - 1,
                    fIO.load(model.fldSU2(i));
                            fIO.load(model.piSU2(i));
            );

            fIO.loader.load(model.aSI, "aSI");
            fIO.loader.load(model.aI, "aI");
            fIO.loader.load(model.aIM, "aIM");
            fIO.loader.load(model.aDotSI, "aDotSI");
            fIO.loader.load(model.aDotI, "aDotI");
            fIO.loader.load(model.aDotSIM, "aDotSIM");
            fIO.loader.load(t0, "tSave");
            fIO.loader.close();

            Averages::setAllAverages(model);

        }


        // Function which saves all field variables to disk when asked.
        template<typename T, class Model>
        void saveSim(ParameterParser& parser, Model& model, T time ,std::string tag = "")
        {
            fIO.saver.create(tag);
            ForLoop(i, 0,Model::Ns - 1,
            fIO.save(model.fldS(i));
                fIO.save(model.piS(i));
            );
            
            if (model.fldGWs != nullptr) {
                ForLoop(i, 0, Model::NGWs - 1,
            fIO.save( (*model.fldGWs)(i));
                fIO.save( (*model.piGWs)(i));
            );}

            ForLoop(i, 0, Model::NCs -1,
                            fIO.save(model.fldCS(i));
                            fIO.save(model.piCS(i));
            );

            ForLoop(i, 0, Model::NSU2Doublet -1,
                            fIO.save(model.fldSU2Doublet(i));
                            fIO.save(model.piSU2Doublet(i));
            );

            ForLoop(i, 0, Model::NU1 -1,
                    fIO.save(model.fldU1(i));
                    fIO.save(model.piU1(i));
            );

            ForLoop(i, 0, Model::NSU2 - 1,
                    fIO.save(model.fldSU2(i));
                            fIO.save(model.piSU2(i));
            );


            fIO.saver.save(model.aSI, "aSI");
            fIO.saver.save(model.aI, "aI");
            fIO.saver.save(model.aIM, "aIM");
            fIO.saver.save(model.aDotSI, "aDotSI");
            fIO.saver.save(model.aDotI, "aDotI");
            fIO.saver.save(model.aDotSIM, "aDotSIM");

            fIO.saver.save(time, "tSave");


            fIO.saver.save(parser);
            fIO.saver.close();
        }


        // Function which performs the automatic backup of the simulation.
        template<typename T, class Model>
        void backup(ParameterParser& parser, Model& model, T time, std::string folder)
        {
            std::string oldStr(folder + model.name + "_backup.h5");
            std::string newStr(folder + model.name + "_backup.h5~");
            rename(oldStr.c_str(), newStr.c_str()); //Keep the last backup in case something goes wrong after.
            saveSim(parser, model, time, oldStr);
        }

        // Tells the outside part of the program wheter we are in restart mode or not.
        bool doWeRestart() const
        {
            return boolRestart;
        }


        // Create an info file, with all the simulation parameters as well as the parallelization grid the simulation
        // was run on.
        void createInfoFile(ParameterParser& par, const std::string& modelName, const std::vector<int>& dec, bool amIRoot)
        {
            info = std::make_unique<ConditionalFileStream>(modelName + ".infos",amIRoot);
            (*info) << "Parameters: \n" << par<< "\n";
            CStyleTime mt;
            mt.now();
            if(!boolRestart) (*info) << "The simulation started on the " << mt.date(" ") << " around " << mt.approxTime("") << ".\n";
            else (*info) << "The simulation RE-started on the " << mt.date(" ") << " around " << mt.approxTime("") << ".\n";
            (*info) << "It will be running on a grid of (";
            for (size_t i = 0; i < dec.size() - 1; ++i) (*info) << dec[i] << ",";
            (*info) << dec.back() << ") cores.";
            info->flush();
        }

        // Finalize the info file by printing some execution time information.
        void closeInfoFile(int nprocesses)
        {
            CStyleTime mt;
            mt.now();
            (*info) << "The simulation finished on the " << mt.date(" ") << " around " << mt.approxTime("") << ".\n";
            long long int ms = TimeSpent::getProcessAge();

            auto ss = ms / 1000;
            auto msDec = ms % 1000;

            char msBuf[6];
            snprintf(msBuf, 6, "%03lli", msDec );
            (*info) << "The timer indicates that it ran for " << ss << "." << msBuf << "s.\n";
            (*info) << "As it ran on " <<nprocesses <<" cores, this corresponds to " << ms / 1000.0 / 3600.0  * nprocesses << " core hours.\n";
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        // Manager to load and save fields to disk.
        FileIO fIO;
        std::string restart;
        bool boolRestart;
        // Info file.
        std::unique_ptr<ConditionalFileStream> info;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/simulationmanager_test.h"
#endif


#endif
