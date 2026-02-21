In this section, our aim is to provide the reader with a better understanding of our physics interface, such as how the fields are actually initialized, how they are evolved, or how the measurements are actually done. In order to do this, we guide the reader through the `main` function of CosmoLattice, highlighting the important class/routines responsible for some specific tasks. We will then explain some of these classes in detail, namely the ones responsible for initializing, evolving and measuring the fields. These are the ones that a user will most likely want to modify to suit their specific purposes.

The `main` function is located in the file `src/cosmolattice.cpp`, which we present now:
```cpp
#include "TempLat/util/stringify.h"
#include STRINGIFY(MODELINCLUDE)
// YourModel.h was loaded during compilation through a variable called MODELINCLUDE in CMake,
// see CMakeList.txt. Here STRINGIFY is just a macro that converts MODELINCLUDE into a string.

using ModelType = TempLat::MODELTYPE;
// We relabel the macro MODELTYPE (which contains the model you decided to study,
// indicated to CMake when compiling), into a type ModelType.

// ------ COSMOLATTICE MAIN ------ //

int main(int argc, char *argv[])
{
```
For completeness, we explain first the top lines in the file, which take care of the model selection. The variable `MODELINCLUDE` is passed at compilation through the `CMake` and contains the path to the model file. It is transformed into a string by the macro `STRINGIFY`, declared in the file  `"TempLat/util/stringify.h"`. Next, on line `10`, we declare for convenience that we are using the namespace `TempLat`, to which all the CosmoLattice functions belong to. Lastly, we redeclare the custom model passed through CMake `MODELTYPE` to `ModelType` in line `15`. After this, we are ready to go into the main function which is executed when the software runs:
```cpp
  // All the classes and functions of CosmoLattice are in the namespace TempLat.
  // Here we load it so that, when using one of its members in the main function,
  // we don't need to specify the namespace TempLat

  SessionGuard guard(argc, argv, false);
  // Instantiating the class SessionGuard is needed to take care of allocation
  // and freeing of MPI and FFT objects. Unless you are an advance user,
  // you can simply forget about this. Keep this line here in either case.
```
Line `21` is the standard `C++` main declaration that accepts arguments from the command line. We then instantiate a `SessionGuard` object, which is in charge of allocating and deallocating internal memory space needed by the library. The file follows as:
```cpp
  ParameterParser parser(argc, argv);
  // We create the ParameterParser used to read the parameters from either:
  // - the input file,
  // - the command line,
  // - or from previous simulations
```
Here we have created a `ParameterParser` called `parser`. As we saw in the previous sections, this object is used throughout the library to declare and read parameters which are parsed from the input file and the command line. The file then follows like:
```cpp
  SimulationManager<ModelType::NDim> manager(parser);
  // The SimulationManager takes care of saving the simulations at the end,
  // backing up or restarting simulations, when appropriate options are passed.
  // It is also in charge of printing the output parameter file.

  if (manager.doWeRestart()) manager.getParams(parser);
  // If restart is set, the 'overridable' parameters are overridden here.
```
Here we have created an object `SimulationManager` called `manager`. This object is used to deal, for instance, with printing the simulation-related information file, or with the backing up of the simulation. It also deals with the fact of whether or not the simulation is restarting from a previous one. In the first case, parameters are retrieved from the restarting simulations, as illustrated on line `39`.
```cpp
  RunParameters<double> runParams(parser);
  runParams.setDoWeRestart(manager.doWeRestart());
  // The RunParameters class holds all relevant (model independent)
  // parameters for a run: Num. of points lattice/side (N),
  // the lattice length side (L) or alternatively the lattice IR cut-off (kIR),
  // the lattice spacing (dx), the time step (dt), the initial and final times,
  // the time interval for (in)frequency measurements, the output directory, etc
  // They are initialised from the parser.
```
We collect all the relevant simulation parameters in a `RunParameters` object called `runParams`. This object receives the information on the parameters, such as the number of points on the lattice, the frequency at which measurements are performed, etc. The file then follows as:
```cpp
  int nGhost = 1;
  // bool changedt = true;
  //  Number of lattice site layers for memory sharing among the neighbouring sub-lattices
  //  split in a parallelised run. By default is set to nGhost = 1, as typically we need only
  //  the closest neighbouring lattice sites to compute a gradient. Setting nGhost > 1
  //  might be required if e.g. higher order gradient or laplacian operators are introduced
  //  in the scalar field equations of motion, or if clover or higher order configurations
  //  of magnetic fields are built in gauge theories. Changing nGhost here requires
  //  re-compiling CosmoLattice.

  auto toolBox = MemoryToolBox<ModelType::NDim>::makeShared(runParams.N, nGhost);
  // The MemoryToolBox contains the memory management and parallelisation machinery
  // of CosmoLattice.  Unless you are an advanced user, you can simply forget about this.
  // Keep this line here in either case.

  bool iAmRoot = toolBox->amIRoot();
  // Boolean to determine whether one processor of a parallel run is the Root processor.
  // We use this to order printing in the console from only the root processor.
```
Here we went on defining quantities and objects useful to run the simulation. The first of these is the so-called number of ghost-cells (see Section [*Parallelization*][subsec_para] for the technical details on this). In the current version of CosmoLattice, we use a spatial discretization that requires only one layer of the so called 'ghost-cells' for parellization purposes. However, if you were to implement and use a spatial lattice derivative that requires to call lattice sites beyond the nearest neighbor positions, you would need to change this number, say to e.g. `nGhost = 2` if your derivatives require the next to nearest neighbour.

Following we create a `MemoryToolBox` object, or more specifically, a pointer to such an object. It holds many internal functions related to memory. For instance, this is where the objects that allow to iterate over the lattice are stored. This object is needed to create fields and some other objects, and as a result, it will be passed around to many functions.

We also retrieve on line `66` whether or not the current process is the root process when running in parallel. The only reason why we do this here is to be able to output in a clear manner some information about the run (typically the running time) in the command line while running in parallel. The file then follows with:
```cpp
  ModelType model(parser, runParams, toolBox);
  // Actual creation of your model. The parser is required to provide
  // the model dependent parameters.

  if (iAmRoot) say << "Model name: " << model.name;
  // Printing the model name from the root processor.
  // You can check in this way, in the console output,
  // that you are running indeed the model you intended.
```
There we just created our model as defined in the model header file we specified, for instance in the `src/models/lphi4.h` of Section [*Definition and declaration of the model*][subsubsec_DefAndDeclModel].
We also print its name, so that we can make sure that the model running is actually the model we wanted. We are then ready to initialize the files in the model, as described in Section [*Initialization of fluctuations*][sec_InitScalar] and [*Initial conditions*][subsubsec_initialConditionsNonAb]:
```cpp
  if (not manager.doWeRestart()) // If this is a new simulation:
  {

    ModelInitializer<double> initializer(model, runParams.lSide, runParams.baseSeed);
    // 1) We create the class responsible for the initialization

    initializer.initialize(model, runParams);
    // 2) We initialize the model.

    t = runParams.t0;
    // 3) We set the initial time as specified in the input parameter file.
    // By default it is zero.

  } else // In case we are restarting from a previous simulation:
```
Here the initialization was taken care by an object of the type `ModelInitializer<double>`, called `initializer`, which we created on line `85` after having checked that we were starting a new simulation and not continuing another one. The `ModelInitializer<double>` class will be explained in detail in Section [*Initializers*][subsec_Initializers]. We also set the initial time. The file now follows with:
```cpp
  {

    if (iAmRoot) say << "Running in restart mode.";
    // We indicate in the console that we are re-starting a previous simulation.

    manager.loadSim(model, t);
    // The model is reloaded from an appropriate file created by a previous simulation.
  }

  // We communicate t0 to the model, in case it needs it internally.
```
This is in case we are restarting from some previous simulation, so the model is reloaded from the saved state. This is taken care of by the simulation manager.

From the point of view of the physics, we have two more classes to instantiate. This is what we do next:
```cpp

  Evolver<ModelType> evolver(model, runParams);
  // Here an algorithm -- evolver -- to solve the field EoM is chosen. The type of evolver
  // is specified by the user in the input parameter file, and here is passed through
  // runParams. Model is passed as well to have access to normalisations.

  Measurer<ModelType, double> measurer(model, runParams);
  // Creates an object of the class responsible for performing and outputting all the required
```
Above, we first create an object called `evolver` of type `Evolver<double>`. This class, as further explained in Section [*Evolvers*][subsec_Evolvers], is in charge of calling the appropriate evolution algorithms. We also create in line `112` a `Measurer<double>` `measurer` in charge of doing and outputing all the standard measurements. This class is explained at great length in Section [*Measurers*][subsec_Measurers]. Before starting the actual simulation, the file continues with:
```cpp
  if (iAmRoot) say << "This simulation will run with the following parameters: \n" << parser;
  // Printing in the console all the parameters chosen (both run parameter and specific
  // model parameters)

  manager.createInfoFile(parser, runParams, model, toolBox->getDecomposition(), iAmRoot);
  // Creation of an info file, which lists all parameters and options chosen
```
Here we print in line `117` the parameters the program is running with. We also mandate the `manager` to create an *information file* which contains these parameters (so their values can be checked after the simulation is concluded). This file also indicates the time at which the simulation started, as well as the type of parallelization that was used. We are now ready to proceed to the time evolution of the system:
```cpp

  for (int i = 0; t < runParams.tMax - runParams.dt / 2.0; ++i) {
    // Loop for the time evolution. At each step we advance one time step dt

    if (measurer.areWeMeasuring(i))
    // We proceed to measure
    {
      evolver.sync(model, t - runParams.t0);
      // Some evolvers like staggered leapfrog have fields and momenta which
      // do not live at the same timesteps. Before measuring, we synchronize them.
      measurer.measure(i, t, model);
      // Note that measurer.measure advances automatically conjugate momenta by half step in case
      // the evolver (e.g. leapfrog) required them to have been synchronised previously for
      // a measurement.
    }

    evolver.evolve(model, t - runParams.t0);
    // We evolve the EoM by one time step dt. It needs the time variable in case we want to simulate
    // a fixed background metric.
    t += model.dt;

    if (runParams.boolBackup && (i % runParams.tBackupFreqInt == 0))
      manager.backup(parser, model, t, runParams.backupPath);
    // If 'backing-up' is activated, here we create a back-up of the simulation
    // in case of a power shortage. Namely we back-up the simulation at the current time step
    // so that an appropriate file is generated, containing all the relevant information
    // (e.g. field and momentum amplitudes in the lattice). That file can be used as an input
    // to re-start the simulation at the same moment, once we launch another job.

  } // End of time evolution.
```
Here we started by checking whether we are performing any measurements at the given time step. If so, we ask the evolvers to synchronize themselves. That means that if a given algorithm works with fields which do not live at integer time step (e.g. in leap frog conjugate momenta live at semi-integer times), before measuring we evolve the corresponding quantities so that they live at integer time steps (together with the rest of field and scale factor variables). Other algorithms like Velocity Verlet do not need this synchronization as, so the previous step is just ignored by them. Next, we let the `evolver` object to `evolve` the system, i.e. this calls the routines to perform one iteration step of the evolution of both the scale factor, the field content, and all their associated derivatives. Finally, we check whether we are require to backup the simulation or not at the given time step. If we do, the `manager` takes care of it. Once the time evolution is done, we are ready to finalize the simulation:
```cpp
  // If this option is activated, an appropriate file is  created, containing all
  // the relevant information (e.g. field and momentum amplitudes in the lattice,
  // model paratmers, run parameters, etc).  This file will be used as input to re-start
  // the simulation (launching another job) from the end of previously saved simulation.
  {

    CStyleTime mt;
    mt.now();
    // We create a class that measures the date and time in the current moment

    manager.saveSim(parser, model, t,
                    runParams.saveEndPath + model.name + "_DATE_" + mt.date() + "_TIME_" + mt.time() + ".h5");
    // This saves the simulation details, stamping the date and time in the name
  }

  manager.closeInfoFile(toolBox->getNProcesses());
  // We need to close the info file, before we finish the simulation.

  return 0;
}
```
We checked on line `158` whether or not we are required to save the simulation at the end of the execution, in case we desire to restart the simulation from the same time in a future run. If so, the `manager` does it on line `169`. Finally we close the information file, printing the time at which the simulation ended and the total time it ran for.

### Evolvers { #subsec_Evolvers }

In this section we present the main routines responsible for the evolution of the fields, showing explicitly how different evolution algorithms are implemented. The relevant classes are located in \path{src/include/evolvers/}. As presented above, the class which is instantiated in the main is the `Evolver<double>`. Its sole purpose is to be able to choose at runtime between different algorithms, which are implemented in their respective classes. We show here the full class:
```cpp
  {
  public:
    // Put public methods here. These should change very little over time.
    using T = typename Model::FloatType;

    Evolver(Model &model, RunParameters<T> &rPar) : type(rPar.eType)
    {
      if (type == LF) {
        lf = std::make_shared<LeapFrog<T>>(model, rPar);
      } else {
        if (!(VelocityVerletParameters<T>::isVerlet(type)))
          throw(EvolverTypeNotInEvolver("The evolver type you specified was not implemented in the Evolver class, "
                                        "which dispatch between different evolvers. Abort."));
        else
          vv = std::make_shared<VelocityVerlet<T>>(model, rPar);
      }
    }

    inline void evolve(Model &model, T tMinust0) const
    {
      if (type == LF) {
        lf->evolve(model, tMinust0);
      } else {
        if (!(VelocityVerletParameters<T>::isVerlet(type)))
          throw(EvolverTypeNotInEvolver("The evolver type you specified was not implemented in the Evolver class, "
                                        "which dispatch between different evolvers. Abort."));
        else
          vv->evolve(model, tMinust0);
      }
    }

    // The next function is used to synchronised all the fields to live
    // at integer time before measurements. Useful for evolvers where
    // this is not naturally the case, such as leapfrog.

    inline void sync(Model &model, T tMinust0) const
    {
      if (type == LF) {
        lf->sync(model, tMinust0);
      } else { // The default evolvers have fields and momenta living at integer times, so no need to sync. for
               // measurements.
        if (!(VelocityVerletParameters<T>::isVerlet(type)))
          throw(EvolverTypeNotInEvolver("The evolver type you specified was not implemented in the Evolver class, "
                                        "which dispatch between different evolvers. Abort."));
        else
          vv->sync(
              model,
              tMinust0); // The sync function is used to set aDot to its correct value in the case of fixed background.
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    std::shared_ptr<LeapFrog<T>> lf;
    std::shared_ptr<VelocityVerlet<T>> vv;
```
The class holds a pointer to any of the implemented algorithm classes, namely  `LeapFrog` and `VelocityVerlet` for the time being, as declared on lines `75` and `76`. The relevant pointer is initialized in the constructor, depending on which algorithm was requested. This is what happens between lines `32` and `41`. The argument `pType` is an `EvolverType` enumerator. These enumerators are defined in the file `src/CosmoInterface/evolvers/evolvertype.h`, and are used to differentiate between evolvers. The `VelocityVerlet` pointer is initialized in the default case because all the different higher-order Velocity-Verlet schemes are implemented in the same class.
This structure is repeated in the `evolve` function on line `43`, which uses the appropriate pointer to call the evolution function\footnote{Note than a more oriented object manner to implement this mechanism would have been to use *polymorphism*. We decided against this option to make it more accessible to users less familiar with C++.}. Note that the `template<class Model>` template argument of the function is the mechanism we use to be able to have the `Evolver` (or as we will see later on any class) to operate on arbitrary models. The last function we have is the `sync` function, which is used to synchronize all the fields (when needed) at integer times. It is used before measuring, and it is useful for instance in the case of staggered leapfrog.

To summarize, the `Evolver<double>` class allows us to choose among evolvers. If the user decides to implement their own evolver, they can simply add it here to be able to choose it at run-time.

The actual characterization of each evolution algorithm is in the `evolvers` files `src/include/CosmoInterface/evolvers/leapfrog.h` and `src/include/CosmoInterface/evolvers/velocityverlet.h`. In the following, we present in detail the leapfrog evolver as an example\footnote{We invite the reader interested in the details of the velocity-verlet algorithm to look them directly in the code, as the structure is pretty similar to the one of the leapfrog method presented here.}
```cpp
   *
   **/
  template <class T> class LeapFrog
  {
  public:
    // Put public methods here. These should change very little over time.

    template <class Model>
    LeapFrog(Model &model, RunParameters<T> &rPar)
        : expansion(rPar.expansion), synced(!rPar.doWeRestart), // If this is the initial time, all fields are a t=0
          fixedBackground(rPar.fixedBackground), // A boolean. If true, expansion is fixed (and not self-consistent)
          aBackground(model, rPar)               // Used for fixed background expansion
    {
      if (synced) model.aDotSI = model.aDotI - model.dt / 2.0 * ScaleFactorKernels::get(model);
      // If this is time t=0 in a new simulation, we set aDotSI at t=-1/2, so we can
      // evolve it by a full time step without changing the algorithm.
    }
```

We see here that the only parameter that our class takes is a boolean, which specifies whether we are considering an expanding universe or not. The structure of the algorithm is really laid out in the `evolve` function:
```cpp
    template <class Model> void evolve(Model &model, T tMinust0)
    {
      /*
       * Leapfrog has momenta living at half-integer steps
       * and coordinates at integer steps.
       *
       * 1. pi_1/2 = pi_(-1/2) + dt * kernel_0  [KICKS]
       * 2. phi_1 = phi_0 + dt * pi_1/2  [DRIFTS]
       *
       * */

      // We start by computing the kicks:

      // First, we check whether our momenta are in a "normal state", namely living at
      // -0.5 (not synced with the fields) or whether they have been synced with the fields
      // for measurement purposes. In this case, they have already been evolved by half
      // a time step and we need to evolve them only by another half time step.
      T weight = synced ? 0.5 : 1.0;

      if (model.Ns > 0) kickScalar(model, weight);
      if (model.fldGWs != nullptr) kickGWs(model, weight);
      if (model.NCs > 0) kickCS(model, weight);
      if (model.NSU2Doublet > 0) kickSU2Doublet(model, weight);
      if (model.NU1 > 0) kickU1Vector(model, weight);
      if (model.NSU2 > 0) kickSU2Vector(model, weight);

      if (expansion) {
        if (!fixedBackground) {
          storeMomentaAverages(model);
          kickScaleFactor(model);
        }
        // We always evolve only the semi-integer scale factor time derivative
        // from semi-sums of momenta at semi_integer time so we don't need to worry
        //  whether or not the field were synced or not.

        // Now we compute the drifts:
        driftScaleFactor(model, tMinust0 + model.dt);
      }

      if (model.Ns > 0) driftScalar(model);
      if (model.fldGWs != nullptr) driftGWs(model);
      if (model.NCs > 0) driftCS(model);
      if (model.NSU2Doublet > 0) driftSU2Doublet(model);
      if (model.NU1 > 0) driftU1Vector(model);
      if (model.NSU2 > 0) driftSU2Vector(model);

      if (expansion && !fixedBackground) storeFieldsAverages(model);

      synced = false;
```

As explained in detail in Ref. [@Figueroa_2020rrl] and outlined on the comments on lines \codeline[V2]{45-46}, the Leapfrog algorithm consists of two parts. First, we need to evolve the conjugate momenta by computing ''kicks", and then we evolve the field variables by means of the ''drifts", using the previously updated momenta. This also reflects itself in the code. The kick function are first called between line `58` and `63`, and then the drift functions are called between line `73` and `81`. In order to evolve the scale factor, we store the averages of the momenta squared after the kicks, and the averages of the fields squared after the drifts, as can be seen on lines `66` and `83`. We note a small subtlety, which manifests itself in line `56`: In order to perform the measurements, we synchronize the momenta to live at integer time steps. After a synchronization, conjugate momenta only need the to be evolved by half a time step, immediately after the call to a measurement. This is what happens when the weight variable is set to $0.5$. The file follows with:
```cpp

    // Function used to synchronize the momentum to the field, by evolving them
    // only by half a time step. Called before performing the measurements,
    //  so everything can be measured at integer time.
    template <class Model> void sync(Model &model, T tMinust0)
    {
      if (!synced) {
        if (model.Ns > 0) kickScalar(model, 0.5);
        if (model.fldGWs != nullptr) kickGWs(model, 0.5);
        if (model.NCs > 0) kickCS(model, 0.5);
        if (model.NSU2Doublet > 0) kickSU2Doublet(model, 0.5);
        if (model.NU1 > 0) kickU1Vector(model, 0.5);
        if (model.NSU2 > 0) kickSU2Vector(model, 0.5);

        if (expansion) {
          if (Model::Ns > 0) model.pi2AvI = Averages::pi2S(model);                         // at t
          if (Model::NCs > 0) model.CSpi2AvI = Averages::pi2CS(model);                     // at t
          if (Model::NSU2Doublet > 0) model.SU2DblPi2AvI = Averages::pi2SU2Doublet(model); // at t
          if (Model::NU1 > 0) model.U1pi2AvI = Averages::pi2U1(model);                     // at t
          if (Model::NSU2 > 0) model.SU2pi2AvI = Averages::pi2SU2(model);                  // at t
          if (!fixedBackground)
            model.aDotI = model.aDotSI + model.dt / 2.0 * ScaleFactorKernels::get(model);
          else
            model.aDotI = aBackground.dot(tMinust0);
        }
      }
```
Here the sync function is responsible of evolving the momenta, so that they live at integer times for a measurement (e.g. of energy outputs). It also computes the resulting momenta averages. Following, all kicks are constructed in the same way:
```cpp
    }

    /********
     * KICKS
     *********/

    template <class Model> void kickScaleFactor(Model &model)
    {
      model.aDotSIM = model.aDotSI;
      model.aDotSI += model.dt * ScaleFactorKernels::get(model);
      model.aDotI = (model.aDotSIM + model.aDotSI) / 2.0;
    }

    template <class Model> void kickScalar(Model &model, T w)
    {
      ForLoop(n, 0, Model::Ns - 1, model.piS(n) += (w * model.dt) * ScalarSingletKernels::get(model, n););
    }

    template <class Model> void kickGWs(Model &model, T w)
    {
      ForLoop(n, 0, Model::NGWs - 1, (*model.piGWs)(n) += (w * model.dt) * GWsKernels::get(model, n););
    }

    template <class Model> void kickCS(Model &model, T w)
    {
      ForLoop(n, 0, Model::NCs - 1, model.piCS(n) += (w * model.dt) * ComplexScalarKernels::get(model, n););
    }

    template <class Model> void kickSU2Doublet(Model &model, T w)
    {
      ForLoop(n, 0, Model::NSU2Doublet - 1,
              model.piSU2Doublet(n) += (w * model.dt) * SU2DoubletKernels::get(model, n););
    }

    template <class Model> void kickU1Vector(Model &model, T w)
    {
      ForLoop(n, 0, Model::NU1 - 1, model.piU1(n) += (w * model.dt) * U1Kernels::get(model, n));
    }

    template <class Model> void kickSU2Vector(Model &model, T w)
    {
      ForLoop(n, 0, Model::NSU2 - 1, model.piSU2(n) += (w * model.dt) * SU2Kernels::get(model, n););
    }

    /********
     * DRIFTS
     *********/

    template <class Model> void driftScaleFactor(Model &model, T tMinust0)
    {
      model.aIM = model.aI;  // at t
      if (fixedBackground) { // if fixed background, the scale factor is given by the power-law function in
                             // fixedbackgroundexpansion.h
        model.aI = aBackground(tMinust0);
```
The file calls the appropriate ''kick" functions defined in the folder `src/include/CosmoInterface/kernels/`. The structure of the kernels and their implementation will be presented in detail in the next section. What is perhaps worth noting here is the use of `ForLoop` to iterate over the field. This is necessary because the iteration needs to happen during compilation, so that the kernel for the appropriate field can be returned by the kernels function.

Following, the drifts are implemented in a very similar way:
```cpp
      } else { // if self-consistent expansion, the scale factor is evolved with the VV algorithm
        model.aI += model.dt * model.aDotSI;      // at t+dt
        model.aSI = (model.aIM + model.aI) / 2.0; // at t+dt/2 (average)
      }
    }
    template <class Model> void driftScalar(Model &model)
    {
      // Here we rescale the canonical momentum appropriately to get
      // the time derivative of the field.
      model.fldS += pow(model.aSI, model.alpha - 3) * (model.dt * model.piS);

      // Note that here we use a compact syntax, without specifying the ForLoop, which
      // will be done internally. Equivalently, one can write the ForLoop explicitly
      // as for the kernels (the ForLoop syntax is used in the kernels as it is more
      // convenient to define kernel function which return the kernel for a specific
      // field).
    }

    template <class Model> void driftGWs(Model &model)
    {
      (*model.fldGWs) += pow(model.aSI, model.alpha - 3) * (model.dt * (*model.piGWs));
    }

    template <class Model> void driftCS(Model &model)
    {
      model.fldCS += pow(model.aSI, model.alpha - 3) * model.dt * model.piCS;
    }

    template <class Model> void driftSU2Doublet(Model &model)
    {
      model.fldSU2Doublet += pow(model.aSI, model.alpha - 3) * model.dt * model.piSU2Doublet;
    }

    template <class Model> void driftU1Vector(Model &model)
    {
      model.fldU1 += pow(model.aSI, model.alpha - 1) * model.dt * model.piU1;
    }

    template <class Model> void driftSU2Vector(Model &model)
    {
      ForLoop(n, 0, Model::NSU2 - 1,
              auto rescaledPi = MakeVector(
                  i, 1, Model::NDim,
                  toSU2(0.5 * pow(model.aSI, model.alpha - 1) * model.dx * model.dt * model.gQ_SU2DblSU2(0_c, n) *
                        model.piSU2(n)(i))); // The 0_c is correct. In our convention, the link is normalized wrt the
                                             // first doublet charge.

              model.fldSU2(n) = (rescaledPi * rescaledPi) * model.fldSU2(n););
      // Here for instance we use the ForLoop again, as this makes it easier to define
      // the rescaled momenta in this case.
    }

    /********
     * FUNCTIONS STORING VOLUME AVERAGES OF COMPOSITE FIELDS AND MOMENTA at different times
     *********/

    template <class Model> void storeMomentaAverages(Model &model)
    {
      if (Model::Ns > 0) {
        model.pi2AvSIM = model.pi2AvSI;                        // at t-dt/2
        model.pi2AvSI = Averages::pi2S(model);                 // at t+dt/2
        model.pi2AvI = 0.5 * (model.pi2AvSIM + model.pi2AvSI); // at t (average)
      }

      if (Model::NCs > 0) {
        model.CSpi2AvSIM = model.CSpi2AvSI;                          // at t-dt/2
        model.CSpi2AvSI = Averages::pi2CS(model);                    // at t+dt/2
        model.CSpi2AvI = 0.5 * (model.CSpi2AvSIM + model.CSpi2AvSI); // at t (average)
      }
      if (Model::NSU2Doublet > 0) {
```
Here we take the opportunity to present some of the versatility of CosmoLattice for introducing a new syntax to perform operations on fields. For instance, we see on line `191` that we do not iterate over the collection of fields. Indeed, with CosmoLattice we can directly operate at the level of collections, and the appropriate `ForLoop` is expanded internally. Of course, we could write all the drifts using `ForLoop` as for the kicks, and the result would be the same. Note also that for the scale factor, we also allow for the possibility of a fixed background expansion. In this case, the scale factor is simply given by a function of time.

The last relevant methods to be defined are the ones that store the appropriate averages, which are needed for the scale factor evolution (in the case of self-consistent evolution):
```cpp
        model.SU2DblPi2AvI = 0.5 * (model.SU2DblPi2AvSIM + model.SU2DblPi2AvSI); // at t (average)
      }
      if (Model::NU1 > 0) {
        model.U1pi2AvSIM = model.U1pi2AvSI;                          // at t-dt/2
        model.U1pi2AvSI = Averages::pi2U1(model);                    // at t+dt/2
        model.U1pi2AvI = 0.5 * (model.U1pi2AvSIM + model.U1pi2AvSI); // at t (average)
      }
      if (Model::NSU2 > 0) {
        model.SU2pi2AvSIM = model.SU2pi2AvSI;                           // at t-dt/2
        model.SU2pi2AvSI = Averages::pi2SU2(model);                     // at t+dt/2
        model.SU2pi2AvI = 0.5 * (model.SU2pi2AvSIM + model.SU2pi2AvSI); // at t (average)
      }
    }

    template <class Model> void storeFieldsAverages(Model &model)
    {
      if (Model::Ns > 0) model.grad2AvI = Averages::grad2S(model);                        // at t
      if (model.NCs > 0) model.CSgrad2AvI = Averages::grad2CS(model);                     // at t
      if (model.NSU2Doublet > 0) model.SU2DblGrad2AvI = Averages::grad2SU2Doublet(model); // at t
      if (Model::NU1 > 0) model.U1Mag2AvI = Averages::B2U1(model);                        // at t
      if (Model::NSU2 > 0) model.SU2Mag2AvI = Averages::B2SU2(model);                     // at t
      model.potAvI = average(Potential::potential(model));                                // at t
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    bool expansion;
    bool synced;
    bool fixedBackground;

    FixedBackgroundExpansion<T> aBackground;
  };

} // namespace TempLat

#endif
```
Here we call the `Averages` routines where the appropriate averages over fields squared are defined, see next section. It also uses the averaging function `average` on the model potential directly, returned by `Potential::potential(model)` (which computes the sum of the potential terms defined in the model).

The structure of the Velocity-Verlet evolver is exactly the same, so we invite the reader to have a look at the code for themselves.

### Kernels and other physics formulae { #subsec_Kernels }

CosmoLattice has been designed to be as compartmentalized as possible, so that any potential change in the code has only ''local" consequences and does not propagate throughout the whole code. This is particularly true for the main field operations, and we have also attempted this in the implementation of the physics. All physical formulae that can be potentially needed for more than one routine have been extracted out and implemented in their own separate class. We have already encountered one example of this in the *kernels* for the evolution algorithms, as different *evolvers* may use the same kernels or parts of them. This aspect is also true for the averages of some fields, which are both used in the evolution routines and in the measurements.

Let us first have a look at some of the kernels, to gain a better understanding of the code structure. As an example we will have a look at the complex scalar kernel first:
```cpp
#include "CosmoInterface/definitions/potential.h"
#include "CosmoInterface/definitions/gaugederivatives.h"

namespace TempLat
{
  /** @brief A class that computes the kernels for the complex scalars.
   *
   *
   **/
  class ComplexScalarKernels
  {
  public:
    // Put public methods here. These should change very little over time.
    ComplexScalarKernels() = delete;

    template <class Model, int N> static auto get(Model &model, Tag<N> n)
    {
      // Returns kernel for complex scalars (formed by the covariant laplacian and potential derivative terms):
      return pow(model.aI, 1 + model.alpha) * GaugeDerivatives::covLaplacianCS(model, n) -
             pow(model.aI, 3 + model.alpha) / 2 * Potential::derivCS(model, n);
    }
  };
```
As we can see, the `class ComplexScalarKernels` is extremely simple, and its sole purpose is to contain the complex scalar field's evolution kernel defined in Eq. ([*85*][eq_kernelcomplexscalar]). As we can see on lines `29` and `30`, and correspondingly from the includes on lines `10` and `11`, the kernel uses two `definitions` classes, namely `GaugeDerivatives` and `Potential`. The first one stores the generic gauge covariant derivative expressions and related expressions, while the later is used to compute the potential derivative with respect to the field components, as well as to compute the potential from the different potential terms defined in the model.
```cpp
   **/
  class GaugeDerivatives
  {
  public:
    // Put public methods here. These should change very little over time.
    GaugeDerivatives() = delete;

    // Laplacian and gauge covariant Laplacian

    template <class Model, int N> static auto LaplacianS(Model &model, Tag<N> n)
    {
      return (1.0 / pow<2>(model.dx)) *
             Total(i, 1, Model::NDim, shift(model.fldS(n), i) + shift(model.fldS(n), -i) - 2 * model.fldS(n));
    }

    template <class Model, int N> static auto covLaplacianCS(Model &model, Tag<N> n)
    {
      auto covPlus = Total(i, 1, Model::NDim, U1sForCSCovDerivs(model, n, i) * shift(model.fldCS(n), i));
      auto covMinus = Total(i, 1, Model::NDim, shift(conj(U1sForCSCovDerivs(model, n, i)) * model.fldCS(n), -i));
      return (covPlus + covMinus - (2.0 * Model::NDim) * model.fldCS(n)) / pow<2>(model.dx);
    }

    template <class Model, int N> static auto covLaplacianSU2Doublet(Model &model, Tag<N> n)
    {
      auto covPlus = Total(i, 1, Model::NDim,
                           SU2sForSU2DoubletCovDerivs(model, n, i) *
                               (U1sForSU2DoubletCovDerivs(model, n, i) * shift(model.fldSU2Doublet(n), i)));
      auto covMinus = Total(i, 1, Model::NDim,
                            shift(dagger(SU2sForSU2DoubletCovDerivs(model, n, i)) *
                                      (conj(U1sForSU2DoubletCovDerivs(model, n, i)) * model.fldSU2Doublet(n)),
                                  -i));
      return (covPlus + covMinus - (2.0 * Model::NDim) * model.fldSU2Doublet(n)) / pow<2>(model.dx);
```
Let us start with the `GaugeDerivatives` class from `definitions`. We see that this class has no internal variable, but it is just used to regroup some functions under one hood. In particular, it holds the functions capable of computing the covariant derivatives and covariant Laplacians of the matter/gauge content of the users' models (for conciseness, we show here only the covariant Laplacians). They rely on the functions `U1sForCSCovDerivs`, `U1sForSU2DoubletCovDerivs` and `SU2sForSU2DoubletCovDerivs`, which compute the appropriate combinations of link variables, that depend on which matter fields couple to which gauge field.

This is a good time to emphasize one of the advantages of having implemented  a 'functional' interface through expression templates in CosmoLattice. The expressions computed by these Laplacian functions (as well as most of the other functions in `definitions`) are really abstract expressions (formulae), in the sense that they are left to be evaluated. Calling e.g. `covLaplacianCS` does not lead to any actual computation, it simply creates an abstract expression which can be evaluated later on. In particular, it gives the opportunity to perform some ''analytical" simplifications to the expressions at compilation. For example, a redundant multiplication of the type ''$1 \cdot \phi$" with $\phi$ some field, can be detected at compilation time and replaced by $\phi$ only. This mechanism is used for instance in the `U1sForCSCovDerivs` function.

Following in the same file,

\insertcppcode{src/include
/CosmoInterface/definitions/gaugederivatives.h}{80}{92}{code_files/gaugederivatives.h}
We encounter the function `fold_multiply` in line `87`, which takes vectorial objects as an argument, and returns the multiplication of all the elements inside the object. In this case we use it on an array, created on line `88` by the macro `MakeArray`, which contains either the link `U1Links(model.fldU1(a),i)` or ''$1$", depending on whether the matter field $N$ couples to the $a^{th}$ $U(1)$ gauge field. The constant $1$ is represented by the object `OneType` on line `91`, which is then automatically discarded by the compiler in multiplications. For more information about how this works, we refer the reader to Appendix [Under the Hood: Expression Templates and CosmoLattice](Under the Hood: Expression Templates and CosmoLattice.md).

The rest of the `GaugeDerivatives` class contains similar functions to compute the other covariant derivatives, as well as functions which implement gradients. As they are all implemented in a very similar fashion, we let the reader explore the code by themselves.

All the other functions in `definitions` are implemented in a similar fashion. Rather than going through all the code, we explain below what they are used for.

-  `src/include/CosmoInterface/definitions/averages.h`: Computes the appropriate averages of the momenta squared and the gradient squared, used in the evolution of the scale factor. It relies on the `FieldFunctionals` class, which defines the correct sum over components.

-  `src/include/CosmoInterface/definitions/energies.h`: Contains the correct rescaling of the conjugate momenta and the correct normalization to compute the energy contributions. These methods can be called either with averages, to obtain a single number, or with the `FieldFunctionals`, to obtain the energy distributions over the whole lattice.

-  `src/include/CosmoInterface/definitions/fieldfunctionals.h`: Defines the appropriate sum over component to compute the relevant energy contributions of the different fields species to the Hubble laws.
-  `src/include/CosmoInterface/definitions/gaugederivatives.h`: As reviewed in the text, defines the expressions to compute the gauge covariant derivatives.
-  `src/include/CosmoInterface/definitions/gausslaws.h`: Defines the expressions to compute the Gauss laws in the Abelian and non-Abelian sectors.
-  `src/include/CosmoInterface/definitions/hubblelaws.h`: Defines the expressions of the Hubble constraint.
-  `src/include/CosmoInterface/definitions/mattercurrents.h`: Defines the expressions of the matter currents for the equations of motion. It is implemented in the same spirit as the `GaugeDerivatives`, and it is able to compute the current for generic matter content.
-  `src/include/CosmoInterface/definitions/potential.h`: Computes the potential from the `potentialTerms` defined in the user's model. Also computes, for the complex scalar fields and the $SU(2)$ doublets, the potential derivative with respect to the field's component in terms of the potential derivative with respect the norm.

These functional forms from `definitions` are used all throughout `CosmoInterface`.

### Initializers { #subsec_Initializers }

Another important aspect of the lattice simulations is the initialization. In this section, we will have a closer look at how this initialization happens in the code. First, there is a class `ModelInitializer` that synchronizes the initialization of each field type, while the specific initialization happens in dedicated classes.
```cpp
   * for all fields and parameters.
   *
   **/
  template <typename T> class ModelInitializer
  {
  public:
    // Put public methods here. These should change very little over time.
    template <class Model> ModelInitializer(Model &model, T pLSide, std::string pSeed) : fg(model, pLSide, pSeed) {}

    /**
     * @brief Initializes the model's fields and scale factor, according to the RunParameters and the model's field
     * content.
     *
     * @param model The model to initialize.
     * @param rPar The run parameters, which determine the initial conditions for the fields and scale factor.
     */
    template <class Model> void initialize(Model &model, RunParameters<T> &rPar)
    {
      // Initialize scale factor:
      if (rPar.expansion) ScaleFactorInitializer::initializeScaleFactor(model, rPar);

      // Initialize scalar singlets:
      if constexpr (Model::Ns > 0) ScalarSingletInitializer::initializeScalars(model, fg, rPar.kCutoff);

      // Initialize GWs:
      if (model.fldGWs != nullptr) GWsInitializer::initializeGWs(model);

      // Initialize the SU2 gauge fields and SU2 doublets:
      //  --> Note: It is important to initialize SU2 first, as the doublet contributes to the U1 currents.
      if constexpr (Model::NSU2Doublet > 0) SU2Initializer::initializeSU2(model, fg, rPar.kCutoff);

      // Initialize the U1 gauge fields and complex scalars:
      if constexpr (Model::NCs > 0 || (Model::NSU2Doublet > 0 && Model::NU1 > 0))
        U1Initializer::initializeU1(model, fg, rPar.kCutoff);

      Averages::setAllAverages(model);
      if (rPar.expansion) {
        // For consistency, correct the scale factor time-derivative with the fluctuations.
        // Relevant only for higher order evolvers.
        auto hubbleLaw = HubbleConstraint::get(model);
        model.aDotI = sqrt(hubbleLaw[2]);
      }
    }
```
As previously claimed, the only purpose of the `ModelInitializer` class is to call the specific initializers in the correct order and to initialize the required averages and Hubble laws; this is what happen in the `initialize` function between lines `36` and `60`. It also holds a dedicated object `FluctuationsGenerator<T>`, which is used by the specific class to generate random Gaussian fluctuations.
```cpp
    ScaleFactorInitializer() = delete;

    template <class Model, typename T> static void initializeScaleFactor(Model &model, RunParameters<T> &rPar)
    {
      // If fixed background, the initial Hubble parameter H0 is given by the user
      if (rPar.fixedBackground) {
        model.aDotI = rPar.H0 / model.omegaStar;
        // H0 is in GeV, so we transform to program variables
      }

      // If self-consistent expansion, the initial Hubble parameter must be computed
      // via the 1st Friedmann eqn:
      else {
        T kin = 0; // initial kinetic energy
        ForLoop(i, 0, Model::Ns - 1,
                kin += Energies::kineticS(model, pow<2>(model.piS0(i) / model.fStar / model.omegaStar)););
        ForLoop(i, 0, Model::NCs - 1,
                kin += Energies::kineticCS(model, norm2(model.piCS0(i) / model.fStar / model.omegaStar)););
        ForLoop(i, 0, Model::NSU2Doublet - 1,
                kin +=
                Energies::kineticSU2Doublet(model, norm2(model.piSU2Doublet0(i) / model.fStar / model.omegaStar)););
        // Note: piS0(i), piCS0(i), piSU2Doublet0(i) are given in GeV^2,
        // so we divide by fStar*omegaStar to transform to program variables

        model.aDotI = model.fStar / Model::MPl * sqrt((kin + model.pot0) / 3.0); // 1st Friedmann eqn

        // Note: Initially, the gradients are 0, so the scale factors receives
        //  only contributions from the kinetic energies and the potential.
      }
    }
  };
} // namespace TempLat

#endif
```
We start with the the `ScaleFactorInitializer`. The only method it contains is `initializeScaleFactor`, shown above. We initialize the scale factor from the homogeneous initial values of the fields, assuming that the gauge fields have zero initial homogeneous components.
```cpp
    /**
     * @brief Initializes the scalar singlet fields and their derivatives with Gaussian fluctuations, according to the
     * model's parameters and the given cutoff scale.
     * The initial homogeneous components are set, respectively, to
     *   - `model.fldS += model.fldS0 / model.fStar;`
     *   - `model.piS += model.piS0 / model.fStar / model.omegaStar;`
     *
     * @param model The model to initialize.
     * @param fg The fluctuations generator to use for creating the initial fluctuations.
     * @param kCutOff The cutoff scale for the fluctuations.
     */
    template <class Model, typename T>
    static void initializeScalars(Model &model, const FluctuationsGenerator<T> &fg, T kCutOff)
    {
      if constexpr (Model::Ns > 0) {
        // We set fluctuations to the scalar singlets:
```
The `ScalarSingletInitializer` is also very simple and holds only the `initializeScalars` methods. There, on line `30`, we set the initial fluctuations of the scalar fields using the `FluctuationsGenerator`. We then add on lines `37` and `38` the homogeneous initial conditions.

The `FluctuationsGenerator` generator class implements Gaussian fluctuations of the type described in Section [*Initialization of fluctuations*][sec_InitScalar]:
```cpp
    {
      FourierSite<Model::NDim> ntilde(f.getToolBox());
      // Fourier lattice site, see eq.(57) of arXiv:2006.15122v2
      auto k = ntilde.norm() * f.getKIR();
      // comoving momentum (in program units), obtained by multiplying it by k_IR
      auto omega = omega_k(k, mass2, f.toString());
      // mode frequency

      auto Hcut = heaviside(kCutOff - k);
      // function that sets to zero all modes over a certain cutoff

      // Returns the rms of the (real and imaginary) parts of the fluctuations
      // (see Sec. 7.1. of arXiv:2006.15122 for a derivation)
      return Hcut * (model.omegaStar / model.fStar * pow(lSide / pow<2>(f.getDx()), 1.5)) * pow(2 * omega, -0.5) /
             sqrt(2);
      // Here 1/sqrt{2omega_k} characterises rms of |phi_k|, but since |phi_k|^2 =
      //  Re(phi_k)^2 + Im(phi_k)^2, hence there is extra 1/sqrt{2} as this 'return' is
      //  in reality the rms of either Re(phi_k) or Im(phi_k).
    }

    // Returns the amplitude of the (left- or right-moving) waves,
    // which follows a Gaussian distribution:
    template <class Model>
    auto getNormedFluctuations(Model &model, Field<Model::NDim, T> f, T mass2, std::string mySeed, T kCutOff) const
    {
      auto fFluctuationNorm = getFluctuationsNorm(model, f, mass2, kCutOff); // norm
      return fFluctuationNorm * RandomGaussianField<Model::NDim, T>(baseSeed + mySeed + f.toString(),
                                                                    f.getToolBox()); // baseSeed is given in input file
    }

    // Sums left-moving and right-moving waves, both following a Gaussian distribution
    template <class Model> void gaussianFluctuations(Model &model, Field<Model::NDim, T> f, T mass2, T kCutOff) const
    {
      auto fLeft = getNormedFluctuations(model, f, mass2, "Random left", kCutOff);
      // left wave
      auto fRight = getNormedFluctuations(model, f, mass2, "Random right", kCutOff);
      // right wave

      f.inFourierSpace() = (fLeft + fRight) / sqrt(2);
      // the sum (which is also Gaussian) is imposed to the field in momentum space
      // extra 1/sqrt{2} is due to having summed Left and Right movers.
      f.inFourierSpace().setZeroMode(0);
      // sets the zero mode to 0
    }

    // This does the same as the previous function, but also sets fluctuations to the time-derivatives
    template <class Model>
    void conjugateGaussianFluctuations(Model &model, Field<Model::NDim, T> f, Field<Model::NDim, T> p, T mass2, T aDot,
                                       T kCutOff) const
    {
      auto fLeft = getNormedFluctuations(model, f, mass2, "Random left", kCutOff);
      auto fRight = getNormedFluctuations(model, f, mass2, "Random right", kCutOff);

      f.inFourierSpace() = (fLeft + fRight) / sqrt(2);
      f.inFourierSpace().setZeroMode(0); // sets the zero mode to 0

      FourierSite<Model::NDim> ntilde(f.getToolBox()); // Fourier lattice site, see eq.(57) of arXiv:2006.15122v2
      auto k = ntilde.norm() * f.getKIR(); // comoving momentum (in program units), obtained by multiplying it by k_IR
      auto omega = omega_k(k, mass2, f.toString()); // mode frequency (defined below)

      p.inFourierSpace() = Constants::I<T> * omega * (fLeft - fRight) / sqrt(2) -
                           aDot * f.inFourierSpace(); // derived in Sec. 7.1. of arXiv:2006.15122
      p.inFourierSpace().setZeroMode(0);              // sets the zero mode to 0
    }

    std::string getBaseSeed() const { return baseSeed; }
```
The most important function here is `getNormedFluctuations`, defined between lines `62` and `67`. It first computes the correct normalization factor in momentum space by calling the `getFluctuationsNorm` function, and returning a correctly normalized `RandomGaussianField`. This `RandomGaussianField` operates in Fourier space returning Gaussian distributed random modes at every point of the Fourier lattice.
To facilitate the initialization, it also provides functions to directly compute the amplitudes as described in Eqs. \eqref{eq:fpr_influct}-\eqref{eq:fpr_influct2}.
The function `gaussianFluctuations` initializes fluctuations of only one given field, while `conjugateGaussianFluctuations` sets the fluctuations of a field and its conjugate momentum. As we will see shortly in the rest of the initializers, this class exists because Gaussian fluctuations enter in some way or another in the initialization of all the different type of matter fields.

The next initializer we want to present is the one for the $SU(2)$ sector:
```cpp
    // INITIALIZATION: SU(2) GAUGE FIELDS
    // --> Note: aDot has to be initialized before calling this function.

    template <class Model, typename T> static void initializeSU2(Model &model, FluctuationsGenerator<T> &fg, T kCutOff)
    {
      // 1. We set the homogeneous components and fluctuations of the SU(2) doublets.
      initializeSU2Doublet(model, fg.getBaseSeed(), fg, kCutOff);

      // 2. We now impose fluctuations to the TIME-DERIVATIVES of the non-Abelian
      // gauge fields (the amplitudes are set exactly to 0 at all lattice points).
      // This is done by imposing the Gauss constraint in momentum space, where
      // the current (given by the SU2 doublet components) sources the gauge fields.
      if constexpr (Model::NSU2 > 0) {
        FourierSite<Model::NDim> ntilde(model.getToolBox());

        const size_t N = GetNGrid::get(model); // Reads N (number of points per dimension)

        // We define keffm as the backward (complex) lattice momentum, consistent with
        // choosing the backward spatial derivative in the Gauss constraint
        // const auto &toolBox = model.fldSU2Doublet(0_c)(0_c).getToolBox();
        auto expIK =
            MakeVector(i, 1, Model::NDim, complexPhase(-2.0 * Constants::pi<T> / N * ntilde(i))); // e^(-2*pi*k/N)
        auto keffm = MakeVector(i, 1, Model::NDim, 1_c - expIK(i));
        auto keffm2 = Total(i, 1, Model::NDim, norm2(keffm(i)));

        // We compute the total SU2 current.
        // As a trick, we use model.fldSU2(1_c) to store it temporarily.
        ForLoop(n, 0, Model::NSU2 - 1, auto j0a = model.fldSU2(n)(1_c)(1_c);
                auto su2density = MatterCurrents::SU2ChargeDensity(model, n);

                ForLoop(a, 1, 3,
                        j0a = -model.dx * su2density(a); // SU2 current

                        // We set the time-derivatives of the gauge fields via
                        // the Gauss constraint in momentum space.
                        ForLoop(i, 1, Model::NDim,
                                model.piSU2(n)(i)(a).inFourierSpace() =
                                    asFourier(conj(keffm(i)) * (1 / keffm2)) * j0a.inFourierSpace();
                                //  Set the zero mode to 0.
                                model.piSU2(n)(i)(a).inFourierSpace().setZeroMode(0);

                        ););

                // We set the amplitude of the gauge links to unity (gauge fields to 0).
                ForLoop(i, 1, Model::NDim, ForLoop(a, 1, 3, model.fldSU2(n)(i)(a) = 0;);););
      }
    }

    // INITIALIZATION: SU2 doublets
    // --> Note: aDot has to be initialized before calling this function.
    template <class Model, typename T>
    static void initializeSU2Doublet(Model &model, std::string baseSeed, FluctuationsGenerator<T> &fg, T kCutOff)
    {
      // 1. We set fluctuations to the SU2 doublets:
      addFluctuationsSU2DoubletFromPhases(model, baseSeed, fg, model.aDotI, kCutOff);
```
The main function above is `initializeSU2`. It first calls the `initializeSU2Doublet` functions, which are responsible to initialize the matter sector in a way that is compatible with the Gauss law, see later on. Once this is done, it sets the initial fluctuations of the gauge fields by inverting the Gauss law, as recalled in Section [*Initial conditions*][subsubsec_initialConditionsNonAb] and described in detail in Ref. [@Figueroa_2020rrl]. First, on line `67` we compute the matter current in real space. Then, on line `75` we invert the Gauss law in Fourier space. The current is computed in Fourier space by calling the `inFourierSpace()` method of the `Field` class. Lastly, we set the gauge links to unity on line `82`. Let us know look at how the matter fluctuations are imposed:
```cpp
      // 2. We set the initial homogeneous components of the fields and derivatives.
      //    model.fldSU2Doublet(i) and piSU2Doublet(i) are introduced in physical
      //    (dimensionful variables), so we transform them to program variables
      //    by dividing them by f_* and f_* omega_* respectively.
      ForLoop(i, 0, Model::NSU2Doublet - 1, model.fldSU2Doublet(i) += model.fldSU2Doublet0(i) / model.fStar;
              model.piSU2Doublet(i) += model.piSU2Doublet0(i) / model.fStar / model.omegaStar;);
      model.SU2DblPi2AvI = Averages::pi2SU2Doublet(model);
      model.SU2DblPi2AvSI = model.SU2DblPi2AvI;
    }

    // Sets fluctuations to SU2 doublet components
    template <class Model, typename T>
    static void addFluctuationsSU2DoubletFromPhases(Model &model, std::string baseSeed, FluctuationsGenerator<T> &fg,
                                                    T aDot, T kCutOff)
    {
      // When imposing initial fluctuations to the SU2 doublets,
      // one must ensure that the Gauss constraints are preserved initially.
      // Here we follow the procedure described in arXiv:2006.15122
      // which guarantees precisely this.
      // In particular, we impose a sum of left-moving and right-moving waves
      // to the four components in momentum space, as for scalar singlets.
```
We see that the  `initializeSU2Doublet` is in charge of first imposing the random fluctuations by calling `addFluctuationsSU2DoubletFromPhases`. Then, the homogeneous components are added to the fields.
```cpp
      // the wave amplitudes and phases.
      // Note that these fluctuations do not exactly follow a Gaussian distribution.

      auto toolBox = model.getToolBox();

      using RRF = RandomRayleighField<Model::NDim, T>;
      using RUF = RandomUniformUnitaryField<Model::NDim, T>;

      ForLoop(
          i, 0, Model::NSU2Doublet - 1,

          // 1. Random amplitudes for the (left-moving and right-moving) waves:
          auto amplitudes = MakeArray(
              a, 0, 3,
              fg.getFluctuationsNorm(model, model.fldSU2Doublet(i)(a), model.masses2SU2Doublet(i)(a), kCutOff) *
                  RRF(baseSeed + "norm" + model.fldSU2Doublet(i)(a).toString(), toolBox));

          // 2. Phases of the waves
          // phases of left-moving waves (for each of the four components), set randomly:
          auto leftPhases =
              MakeArray(a, 0, 3, RUF(baseSeed + "phaseLeft" + model.fldSU2Doublet(i)(a).toString(), toolBox));

          // the phase of the right-moving wave of the 0 component is also set randomly:
          auto rightPhase0 = RUF(baseSeed + "phaseRight" + model.fldSU2Doublet(i)(0_c).toString(), toolBox);
          // the phases of the right-moving waves of
          // the other components (1,2,3) are functions of the other ones:

          auto rightPhases = MakeArray(a, 0, 3, rightPhase0 * leftPhases(a) * conj(leftPhases(0_c)));
          // adds redundantly the 0 phase.

          // 3. Fluctuations for the amplitudes of the SU(2) doublet components:
          ForLoop(a, 0, 3,
                  model.fldSU2Doublet(i)(a).inFourierSpace() =
                      amplitudes(a) * (leftPhases(a) + rightPhases(a)) / sqrt(2) / sqrt(2);
                  // Second sqrt(2) comes from normalisation of SU2 doublet
                  model.fldSU2Doublet(i)(a).inFourierSpace().setZeroMode(0);
                  // Sets the zero mode to 0.
          );

          // 4. Fluctuations for the time-derivatives of the SU(2) doublet components:

          FourierSite<Model::NDim> ntilde(model.getToolBox()); auto k = ntilde.norm() * model.kIR;
          auto omega0 = fg.omega_k(k, model.masses2SU2Doublet(i)(0_c), model.fldSU2Doublet(i)(0_c).toString());
          // effective frequency

          ForLoop(a, 0, 3,
                  model.piSU2Doublet(i)(a).inFourierSpace() =
                      -Constants::I<T> * omega0 * amplitudes(a) * (leftPhases(a) - rightPhases(a)) / sqrt(2) / sqrt(2) -
                      aDot * model.fldSU2Doublet(i)(a).inFourierSpace() / sqrt(2);

                  model.piSU2Doublet(i)(a).inFourierSpace().setZeroMode(0);
                  // Sets the zero mode to 0.

          ););
    }
  };
} // namespace TempLat

#endif
```

What is implemented in the function `addFluctuationsSU2DoubletFromPhases` simply corresponds to the procedure given in Eqs ([*77*][eq_fpr_influct3])-([*80*][InConstr_2]). The independent amplitudes are created first, and then these are used to construct the dependent ones afterwards. All of this is then used to initialize the fluctuations of the fields and momenta.

Once the $SU(2)$ sector is initialized, the last part which needs to be initialized is the $U(1)$ sector. This is taken care of by the `U1Initializer`. It works in a similar was a the `SU2Initializer`, so we let the interested reader to go directly to explore the code located at: `src/include/CosmoInterface/initializers/u1initializer.h`.

### Measurers { #subsec_Measurers }

The measurements are built in a similar way as the initializers. All the measurements are synchronized by the `measurer` class:
```cpp

  template <typename Model, typename T = double> class Measurer
```
```cpp
    }

    template <typename R> void measure(int n, R t, Model &model)
    {
      bool isInitialTime = (n == 0);

      // Frequent output (averages):
      if ((n % outputFreq == 0)) {

        if (!expansion) {
          model.potAvI = average(Potential::potential(model));
        }
        //"Standard" measurementsIO (mean, rms etc) of fields and momentum

        scalarSingletMeasurer.measureStandard(model, t);
        // Means, rms, etc, of scalar singlet
        complexScalarMeasurer.measureStandard(model, t);
        // Means, rms, etc, of complex scalar
        su2DoubletMeasurer.measureStandard(model, t);
        // Means, rms, etc, of SU(2) doublet
        u1Measurer.measureStandard(model, t);
        // Means, rms, etc, of U(1) gauge field
        su2Measurer.measureStandard(model, t);
        // Means, rms, etc, of SU(2) gauge field
        energiesMeasurer.measure(model, t, isInitialTime);
        // Energy contributions and conservation check
        scaleFactorMeasurer.measure(model, t);
        // Scale factor and derivatives
        // gwsMeasurer.measureStandard(model,t, TestTransTrace);
        // Transversality and tracelessness test of GWs

        filesManager.flush();
      }

      // Infrequent output (spectra):
      if (n % infreqOutputFreq == 0) {

        scalarSingletMeasurer.measureSpectra(model, t, PSMeasurer);
        // Scalar singlet spectra
        gwsMeasurer.measureSpectra(model, t, GWsPSMeasurer);
        // GWs spectra
        complexScalarMeasurer.measureSpectra(model, t, PSMeasurer);
        // Complex scalar spectra
        su2DoubletMeasurer.measureSpectra(model, t, PSMeasurer);
        // SU(2) doublet spectra
        u1Measurer.measureSpectra(model, t, PSMeasurer);
        // Electric and magnetic spectra, U(1) gauge sector
        su2Measurer.measureSpectra(model, t, PSMeasurer);
        // Electric and magnetic spectra, SU(2) gauge sector

        if (!filesManager.getUseHDF5()) {
          spectraTime.addAverage(t);
          // Each time infrequent output is computed, time is added to the file
          // average_spectra_time.txt
          spectraTime.save();
        }
      }
      // Rare output (snapshots):
      if (n % rareOutputFreq == 0) {
        energySnapshotsMeasurer.measure(model, t);
        // 3D snapshots of energy contributions and fields
      }
      if (n % verbOutputFreq == 0 && amIRoot) {
        // If in root processor, print update message in terminal at frequent times.
        model.getToolBox()->resetVerbose();
        sayMPI << "Step " << n << " done. Current time:" << t << "\n";
      } else
        model.getToolBox()->unsetVerbose();
    }
```
It possesses a single method `measure`, displayed above, whose aim is to coordinate the different measurements and call field specific classes which perform field specific measurements. We start with what we call ''frequent  measurements". These call the different field and energy measurers and ask them to compute their respective observable at a ''frequent" rate defined by the user in the input parameter file. As we will see, it mostly consists in field averages and variances together with the Gauss laws and energy conservation/Hubble constraints. Once this is done, we move on to the ''infrequent measurements", in line `101`, and proceed in a similar fashion. Infrequent measurements mostly consists in fields' spectra measured at a more infrequent rate determined by the user (again in the input parameter file). Also, when using `txt` output, we store the ''infrequent" times in a file, on line `120`, to facilitate the data analysis. After this, we proceed with the ''rare measurements", corresponding to the most resource consuming. This is where for instance the three-dimensional snapshots of energy densities are measured.
Finally, the measurer is also used to print out some information to the console, see line `129`.

To understand better what measurements are performed, we will inspect specific measurers:
```cpp
    GWsMeasurer<T> gwsMeasurer;
    ComplexScalarMeasurer<T> complexScalarMeasurer;
    SU2DoubletMeasurer<T> su2DoubletMeasurer;
    U1Measurer<T> u1Measurer;
    SU2Measurer<T> su2Measurer;
    EnergiesMeasurer<T> energiesMeasurer;
    ScaleFactorMeasurer<T> scaleFactorMeasurer;
    EnergySnapshotsMeasurer<Model> energySnapshotsMeasurer;
```

We show above the different measurers that can be used by CosmoLattice. All the field measurers are similar, so we will only present in details of `ScalarSingletMeasurer` and `SU2Measurer`. We will then move on to the `EnergiesMeasurer` and the `EnergySnapshotMeasurer`:
```cpp
  {
  public:
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
      );
    }

    // The following function measures the corresponding averages
    // with MeansMeasurer::measure, and adds them to the files.
    // NOTE: For scalar fields, the momenta is defined as pi=a^(3-alpha)*f',
    // with f' the time-derivative; see eq.(220) of arXiv:2006.15122v2.
```
We start by looking here at how our `ScalarSingletMeasurer` is initialized. It contains two arrays, namely `standardOut` and `spectraOut` which contains objects which can save respectively mean values and spectra, one for each scalar field. Here, the files are created by passing the fields and the file names are automatically generated through the fields' names.
```cpp
    {
      ForLoop(i, 0, Model::Ns - 1,
              MeansMeasurer::measure(standardOut(i), model.fldS(i), model.piS(i) * pow(model.aI, model.alpha - 3), t);
              standardOut(i).save(););
    }

    // The following function measures the spectra of the norm and its time-derivative.
    template <typename Model> // The occupation number is only measured if the user indicates it. The current version
                              // requires one additional field to measure it (JBB, Nov 2023).
    void measureSpectra(Model &model, T t, PowerSpectrumMeasurer &PSMeasurer)
    {
```

The averages are actually computed in the `measureStandard` function. As the names suggest, the measurements we want to perform are very standard and will be the same for all the fields. As such they are performed by an external class, called `MeansMeasurer`, whose `measure` function takes a `MeasurementsSaver`, a field, the corresponding velocity and the time at which the measurements have to be conducted.
```cpp

    template <typename S, typename Q, typename T> static void measure(MeasurementsSaver<T> &ms, S fld, Q mom, T t)
    {
      auto phi = average(fld);
      auto phi2 = average(pow<2>(fld));
      auto pi = average(mom);
      auto pi2 = average(pow<2>(mom));

      ms.addAverage(t);
      ms.addAverage(phi);
      ms.addAverage(pi);
      ms.addAverage(phi2);
      ms.addAverage(pi2);

      auto rmsPhi2 = phi2 - pow<2>(phi);
      auto rmsPi2 = pi2 - pow<2>(pi);
```

As we see now, this functions simply computes the average of the field, its average square and its variance, as well for the velocity, and adds it to the `MeasurementsSaver`.

After the averages are measured, we move on to the power spectra.
```cpp
          if (flagON) {
            spectraOut(i).save(t, PSMeasurer.powerSpectrum(model.fldS(i)),
                               pow(model.aI, 2 * model.alpha - 6) * PSMeasurer.powerSpectrum(model.piS(i)),
                               ONMeasurer.occupationNumber(model, i));
          } else {
            spectraOut(i).save(t, PSMeasurer.powerSpectrum(model.fldS(i)),
                               pow(model.aI, 2 * model.alpha - 6) * PSMeasurer.powerSpectrum(model.piS(i)));
          });
    }
```

This is taken care of by the `measureSpectra` function of the `ScalarSingletMeasurer`. There, for each field, we compute its power spectrum as defined in Eq. ([*24*][eq_discretePS]), we compute the power spectrum of its associated velocity and the occupation number, defined in Eq. ([*44*][eq_OccuppationNum]). They are all saved in the same file. As noted in the comment, in the current implementation, it is better to perform the scale factor re-scaling of the momentum outside the `powerSpectrum` function. Indeed, when called on a `Field`, the function does not allocate extra memory to perform the Fourier transform. It does do that for any argument which is not purely a `Field`, as it would be that case had we called for instance `powerSpectrum(piS(i) * pow(model.aI,model.alpha - 3))`.

Let us move briefly to the `SU2Measurer`, to highlight some features not used in the `ScalarSingletMeasurer`. First, at initialization, we do not create files for individual quantities. We will store the averages of the norms of the  fields. As such, the `MeasurementsSaver` are created with customs name. In the case of the $SU(2)$ sector, we are also interested in checking the conservation of Gauss' law and as a result create another file to store them. The spectra are computed also only for the norms.
```cpp
    SU2Measurer(Model &model, FilesManager<Model::NDim> &filesManager, const RunParameters<T> &par, bool append)
    {
      bool amIRoot = model.getToolBox()->amIRoot();

      // We create three files for each SU(2) gauge field:
      ForLoop(i, 0, Model::NSU2 - 1,

              standardNormOut.emplace_back(MeasurementsSaver<T>(filesManager, "norm_SU2_" + std::to_string(i), amIRoot,
                                                                append, MeansMeasurer::headerEB()));
              // Contains volume-averages of the electric and magnetic fields:
              // norm squared, norm to the fourth, and variances

              gauss.emplace_back(
                  MeasurementsSaver<T>(filesManager, "gauss_SU2_" + std::to_string(i), amIRoot, append,
                                       {"t", "var(LHS-RHS)_over_var(LHS+RHS)", "var(LHS)",
                                        "var(RHS)"})); // Checks the degree of conservation of the SU(2) gauss law.

              spectra.emplace_back(SpectrumSaver<T>(filesManager, "norm_SU2_" + std::to_string(i), amIRoot, append,
                                                    par)); // Contains the spectra of the electric and magnetic fields.

      );
    }

    // This measures the corresponding averages with MeansMeasurer::measure, and add them to the files.
    // NOTE: For gauge fields, their momenta is defined as pi=a^(alpha-1)*B'_i, with A'_i the electric field.
```
The measurements of the means are performed in the `measureStandard` function:
```cpp
    template <typename Model> void measureStandard(Model &model, T t)
    {
      ForLoop(i, 0, Model::NSU2 - 1, auto B = sqrt(FieldFunctionals::B2SU2(model, i));
              auto E = pow(model.aI, 1 * model.alpha - 1) * sqrt(FieldFunctionals::pi2SU2(model, i));
              MeansMeasurer::measure(standardNormOut(i), E, B, t); standardNormOut(i).save();
              gauss(i).addAverage(t); // adds time to the Gauss law file
              auto gaussArr = GaussLaws::checkSU2(model, i);
              // the function returns a 3-component vector with information
              // of the left and right hand sides of the Gauss law.

              gauss(i).addAverage(gaussArr(0)); // var(LHS - RHS)_over_var(LHS + RHS),
              gauss(i).addAverage(gaussArr(1)); // var(LHS)
              gauss(i).addAverage(gaussArr(2)); // and var(RHS)
              gauss(i).save(););
    }

    // This measures the electric and magnetic spectra and adds them to the files.
    template <typename Model> void measureSpectra(Model &model, T t, PowerSpectrumMeasurer &PSMeasurer)
    {
      ForLoop(k, 0, Model::NSU2 - 1,
```
We see here that we can use again the `MeansMeasurer` to measure the mean values and variances of the electric and magnetic fields. We also measure the Gauss law and how well it is satisfied. The first component of the `gaussArr` contains the violation degree of the Gauss law, while the second and third contain respectively the left-hand and right-hand sides of the Gauss law.

Finally, electric and magnetic spectra are computed in the `measureSpectra` function:
```cpp
              auto ESU2 = pow(model.aI, model.alpha - 1) * safeSqrt(FieldFunctionals::pi2SU2(model, k));
              auto magSpecSU2 = PSMeasurer.powerSpectrum(BSU2); auto elSpecSU2 = PSMeasurer.powerSpectrum(ESU2);

              spectra(k).save(t, elSpecSU2, magSpecSU2););
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    TempLatVector<MeasurementsSaver<T>> standardNormOut;
    TempLatVector<MeasurementsSaver<T>> gauss;

    TempLatVector<SpectrumSaver<T>> spectra;
```
Contrary to the scalar case, since we are anyhow measuring composite field expressions, it does not matter where we do the momentum rescaling. For readability we do it in an auxiliary field variable `ESU2`.

The `EnergyMeasurer` is also built in very similar way. It contains two `MeasurementsSaver`, one to store the different energy components and one to store the energy conservation check in the case without expansion or the Hubble law check in the case with expansion. The measurements are then performed in the `measure` function.
```cpp
    {
      energies.addAverage(t); // add to file
      T Etot = 0;             // stores total energy
      T Egrad = 0;            // auxiliary variable, stores grad energy
      T Ekin = 0;             // auxiliary variable, stores kinetic energy

      // The "energies" functions contain the appropriate scale factor rescaling. Here we compute the energy species by
      // species

      // Scalar singlets
      ForLoop(i, 0, Model::Ns - 1, Ekin = average(Energies::kineticS(model, FieldFunctionals::pi2S(model, i)));
              Egrad = average(Energies::gradientS(model, FieldFunctionals::grad2S(model, i)));
              Etot += Ekin + Egrad; // add to total energy
              energies.addAverage(Ekin); energies.addAverage(Egrad););

      // Complex scalars
      ForLoop(i, 0, Model::NCs - 1, Ekin = average(Energies::kineticCS(model, FieldFunctionals::pi2CS(model, i)));
              Egrad = average(Energies::gradientCS(model, FieldFunctionals::grad2CS(model, i)));
              Etot += Ekin + Egrad; // add to total energy
              energies.addAverage(Ekin); energies.addAverage(Egrad););
      
      ...

      if (!fixedBackground) { // Energy cannot be checked if expansion is fixed

        // We now check energy conservation:
        energyCons.addAverage(t);
        if (saveEtot) Etot0 = Etot; // Saves the initial total energy before the first iteration

        if (expansion) { // If self-consistent expansion, energy conservation is checked via the first Friedmann
                         // equation
          auto hubbleLaw = HubbleConstraint::get(model);
          energyCons.addAverage(hubbleLaw[0]);
          energyCons.addAverage(hubbleLaw[1]);
          energyCons.addAverage(hubbleLaw[2]);
        } else { // If no expansion, energy must be approximately constant during the evolution
          energyCons.addAverage(abs(1.0 - Etot / Etot0));
        }

        energyCons.save();
      }
    }
```

One by one field species, we compute their energy contributions (this is why we do not used the stored averages, as these are summed over field species). We save them one after another in a file. We also compute the total energy of the system. Above we show explicitly only the scalar contributions, the reader can see the other contribution directly in the file of the code. After this, we also compute the contribution from the potential, term by them. We store finally the total energy of the system in the last column of the file.

The last distinct type of measurer is the `EnergySnapshotMeasurer`. In the current implementation, it can only be used with the `HDF5` library. When asked, it prints the three-dimensional distribution of the requested energy components. Below we show the code only for the scalar sectors, other being similar.
```cpp
    EnergySnapshotsMeasurer(Model &model, FilesManager<Model::NDim> &fm, std::vector<std::string> toSave)
        : mRoot(fm.getTag() + fm.getWorkingDir())
    {
      // This checks which energies are specified in the string "toSave" (passed as a parameter), and creates the
      // corresponding h5 files to save the snapshots.

#ifdef HAVE_HDF5
      saveScalarK = IsInContainer::check("E_S_K", toSave);         // kinetic energy of the scalar singlets
```

First, upon initialization, it determines which energy components it needs to save to file.
Then, again shown only for the scalar sector, if needed, the binary files which will contain the snapshots are created.
Finally, the energy snapshots are taken in the `measure` function:
```cpp
      if (saveScalarK) { // kinetic energy of the scalar singlets
        ForLoop(i, 0, Model::Ns - 1, fIO.saver.open(nameScalarK); fIO.saver.save(
            t, Energies::kineticS(model, FieldFunctionals::pi2S(model, i)), "E_S_K_" + std::to_string(i));
                fIO.saver.close(););
      }
      if (saveScalarG) { // gradient energy of the scalar singlets
        ForLoop(i, 0, Model::Ns - 1, fIO.saver.open(nameScalarG); fIO.saver.save(
```
Species by species, we measure their energy contribution and save the three-dimensional distribution to file.

#### Measurements Input/Output

To conclude the section devoted to measurements, we want to briefly present the structure of the classes responsible to output the measurements to file, so that a user who wants to modify them or add its own output format, can do so easily. They are located in the `src/include/CosmoInterface/measurements/measurementsIO/` folder. They are of two types: the `MeasurementsSaver`'s, to save mean values, and the `SpectrumSaver`'s,to save spectra to file. They are designed in a similar way than the `Evolver`'s, in the sense that they both have an ''interface" class, called `MeasurementsSaver` and `MeasurementsSaver`, which redirect to the appropriate ''implementation" class, which is in charge of actually saving the output. In this way, it is straightforward to implement different output format without having to modify any other part of the code. These interfaces work together with a helper class called `FilesManager`, which has global information over all the measurement files.

Let us first discuss the `MeasurementsSaver` interface. In the current state of the code, there is only one ''standard" implementation to save the measurements, so strictly speaking there would be no need for an interface; we kept this design for future development purposes. To create a `MeasurementsSaver`, you have two different options, one which takes a `std::string` as input and names the file this way, and another that takes a `Field` variable, naming the files according to its name (this constructor can also accept algebraic expression and name the file accordingly).
```cpp
                      const std::vector<std::string> &headers = {}, bool dontCreate = false)
```

Both methods take the same arguments. First, the `FilesManager`, which is created and stored in the `Measurer` class. Its purpose is to collect generic information about the outputing procedure. This is where the choice between different interfaces should be stored. Even if it is not implemented yet, it could also be of use to create a folder structure to save the different outputs. The `amIRoot` parameter tells the `MeasurementsSaver` whether it belongs to the root `MPI` process or not, as only the root process is allowed to save to file (in the standard implementation at least). The `appendMode` specify whether the old measurements files potentially present (by mistake or when restarting a simulation) should be appended to or overwritten. The `headers` file is an array, by default empty, containing the headers for the file. Lastly, the `dontCreate` allows to call one of the `MeasurementsSaver` saver constructors without actually creating a file; this is useful to switch between different physical scenarios which do not need the same output (e.g. expanding versus non expanding universe).

The use of the `MeasurementsSaver` is intended to be very straightforward. It relies on the method `addAverage`, which registers a value to be stored, and a `save` function which actually saves all the registered values to file.
```cpp
      } else
    {
```

The `SpectrumSaver` interface works in a similar fashion, except that in this case, two interfaces are already implemented: the standard one and the `HDF5` one (see Section [*A more user-friendly format for the spectra*][subsubsec_hdf5spectra]). Filenames are also constructed either from a `std::string` or a `Field`'s name.
```cpp
                  bool dontCreate = false)
    template <size_t NDim>
```
Its use is also straightforward and based on a single `save` function.
```cpp
    {
      if (useHDF5) {
```
It takes as arguments a time $t$, and an arbitrary number of spectra. It then forwards them to the correct implementation (standard or `HDF5`) and save them to a file. The $\dots$ syntax is the modern way of `C++` to create functions for an arbitrary number of arguments. To learn about the actual implementation, we invite the interested reader explore the files in the `std` and `hdf5` sub-folders.

