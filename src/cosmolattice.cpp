/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

#include "TempLat/util/stringify.h"
#include STRINGIFY(MODELINCLUDE)
// YourModel.h was loaded during compilation through a variable called MODELINCLUDE in CMake,
// see CMakeList.txt. Here STRINGIFY is just a macro that converts MODELINCLUDE into a string.

using namespace TempLat;
// All the classes and functions of CosmoLattice are in the namespace TempLat.
// Here we load it so that, when calling one of its  functions in this file,
// we don't need to specify the namespace TempLat

using ModelType = MODELTYPE;
// We relabel the macro MODELTYPE (which contains the model you decided to study,
// indicated to CMake when compiling), into a type ModelType.

// ------ COSMOLATTICE MAIN ------ //

int main (int argc, char* argv[] ) {

    SessionGuard guard(argc, argv, false );
    // Instantiating the class SessionGuard is needed to take care of allocation
    // and freeing of MPI and FFT objects. Unless you are an advance user,
    // you can simply forget about this. Keep this line here in either case.

    ParameterParser parser(argc, argv);
    // We create the ParameterParser used to read the parameters from either:
    // - the input file,
    // - the command line,
    // - or from previous simulations

    SimulationManager manager(parser);
    // The SimulationManager takes care of saving the simulations at the end,
    // backing up or restarting simulations, when appropriate options are passed.
    // It is also in charge of printing the output parameter file.

    if(manager.doWeRestart()) manager.getParams(parser);
    //If restart is set, the 'overridable' parameters are overridden here.

    RunParameters<double> runParams(parser);
    runParams.setDoWeRestart(manager.doWeRestart());
    // The RunParameters class holds all relevant (model independent)
    // parameters for a run: Num. of points lattice/side (N),
    // the lattice length side (L) or alternatively the lattice IR cut-off (kIR),
    // the lattice spacing (dx), the time step (dt), the initial and final times,
    // the time interval for (in)frequency measurements, the output directory, etc
    // They are initialised from the parser.

    int nGhost = 1;
    //bool changedt = true;
    // Number of lattice site layers for memory sharing among the neighbouring sub-lattices
    // split in a parallelised run. By default is set to nGhost = 1, as typically we need only
    // the closest neighbouring lattice sites to compute a gradient. Setting nGhost > 1
    // might be required if e.g. higher order gradient or laplacian operators are introduced
    // in the scalar field equations of motion, or if clover or higher order configurations
    // of magnetic fields are built in gauge theories. Changing nGhost here requires
    // re-compiling CosmoLattice.

    auto toolBox = MemoryToolBox::makeShared(ModelType::NDim, runParams.N, nGhost);
    // The MemoryToolBox contains the memory management and parallelisation machinery
    // of CosmoLattice.  Unless you are an advanced user, you can simply forget about this.
    // Keep this line here in either case.

    bool iAmRoot = toolBox->amIRoot();
    // Boolean to determine whether one processor of a parallel run is the Root processor.
    // We use this to order printing in the console from only the root processor.

    ModelType model(parser, runParams, toolBox);
    // Actual creation of your model. The parser is required to provide
    // the model dependent parameters.

    if(iAmRoot) say << "Model name: " << model.name;
    // Printing the model name from the root processor.
    // You can check in this way, in the console output,
    // that you are running indeed the model you intended.

    double t = 0;
    //Our time variable. Initialized below.

    if(not manager.doWeRestart()) //If this is a new simulation:
    {

        ModelInitializer<double> initializer(model, runParams.lSide, runParams.baseSeed);
        // 1) We create the class responsible for the initialization

        initializer.initialize(model, runParams);
        // 2) We initialize the model.

        t = runParams.t0;
        // 3) We set the initial time as specified in the input parameter file.
        // By default it is zero.

    }
    else //In case we are restarting from a previous simulation:
    {

        if(iAmRoot) say << "Running in restart mode.";
        // We indicate in the console that we are re-starting a previous simulation.

        manager.loadSim(model, t);
        // The model is reloaded from an appropriate file created by a previous simulation.

    }

    Evolver<ModelType> evolver(model, runParams);
    // Here an algorithm -- evolver -- to solve the field EoM is chosen. The type of evolver
    // is specified by the user in the input parameter file, and here is passed through
    // runParams. Model is passed as well to have access to normalisations.

    Measurer<double> measurer(model, runParams);
    // Creates an object of the class responsible for performing and outputting all the required
    // measurements (averages, energies, spectra...).


    if(iAmRoot) say << "This simulation will run with the following parameters: \n" << parser;
    // Printing in the console all the parameters chosen (both run parameter and specific
    // model parameters)

    manager.createInfoFile(parser, runParams.outFn + model.name, toolBox->getDecomposition(), iAmRoot);
    //Creation of an info file, which lists all parameters and options chosen


   /************************Time evolution*************************/

   for (int i = 0 ; t < runParams.tMax - runParams.dt / 2.0; ++i) {
        //Loop for the time evolution. At each step we advance one time step dt

        if(measurer.areWeMeasuring(i))
        //We proceed to measure
        {
            evolver.sync(model, t - runParams.t0);
            //Some evolvers like staggered leapfrog have fields and momenta which
            //do not live at the same timesteps. Before measuring, we synchronize them.
            measurer.measure(i, t, model);
            // Note that measurer.measure advances automatically conjugate momenta by half step in case
            // the evolver (e.g. leapfrog) required them to have been synchronised previously for
            // a measurement.
        }

        evolver.evolve(model, t - runParams.t0);
        // We evolve the EoM by one time step dt. It needs the time variable in case we want to simulate
        // a fixed background metric.
        t += model.dt;

        if(runParams.boolBackup && (i % runParams.tBackupFreqInt == 0)) manager.backup(parser, model, t, runParams.backupPath);
        // If 'backing-up' is activated, here we create a back-up of the simulation
        // in case of a power shortage. Namely we back-up the simulation at the current time step
        // so that an appropriate file is generated, containing all the relevant information
        // (e.g. field and momentum amplitudes in the lattice). That file can be used as an input
        // to re-start the simulation at the same moment, once we launch another job.

    } //End of time evolution.



    if(runParams.boolSaveEnd)
        // Here we check whether we want to save the simulation details at the end of evolution.
        // If this option is activated, an appropriate file is  created, containing all
        // the relevant information (e.g. field and momentum amplitudes in the lattice,
        // model paratmers, run parameters, etc).  This file will be used as input to re-start
        // the simulation (launching another job) from the end of previously saved simulation.
    {

        CStyleTime mt; mt.now();
        //We create a class that measures the date and time in the current moment

        manager.saveSim(parser, model, t,
           runParams.saveEndPath + model.name + "_DATE_" + mt.date() + "_TIME_" + mt.time() + ".h5");
        //This saves the simulation details, stamping the date and time in the name
    }

    manager.closeInfoFile(toolBox->getNProcesses());
    //We need to close the info file, before we finish the simulation.

    return 0;
}
