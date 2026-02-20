from base import *

# For testing purposes

test_string = r"""
\section{TABLETESTER}

\begin{tabular}{ | m{3.7cm} | m{12.8cm}| } \hline
{\bf Parameters} & {\bf Explanation} \\ \hline
{\tt N} & Number of lattice points per dimension.\\ \hline
\tt kIR  & Infrared cutoff of the lattice \textbf{in program units}, i.e.~$\tilde{k}_{\rm IR} \equiv k_{\rm IR} / \omega_*$.  \\ \hline
\tt lSide  & Length of the box \textbf{in program units}, i.e.~$\tilde{L} \equiv L \omega_*$. \\ \hline
\tt dt & Time step of the evolution algorithm \textbf{in program units}, i.e.~$\delta \tilde{\eta}$.  \\ \hline
\tt expansion & Expanding universe or not.  If {\tt false}, the scale factor is fixed to unity and field dynamics occur in Minkowski. If {\tt true} (default value), the scale factor evolves self-consistently according to the Friedmann equations. A fixed background expansion rate can be further specified by the parameter \texttt{fixedBackground}.\\ \hline
\tt evolver & Type of evolution algorithm. Options `VV2', `VV4', `VV6', `VV8', and `VV10' solve the field equations with the velocity-verlet algorithm of the corresponding order, while `LF' solves them with the staggered-leapfrog method. Check \href{https://www.cosmolattice.net/technicalnotes}{\color{blue} https://www.cosmolattice.net/technicalnotes} for addition of new evolvers. \\ \hline
\tt t0 & Initial time of the simulation \textbf{in program units} (set to 0 by default).  \\ \hline
\tt tMax & Final time of the simulation \textbf{in program units}.  \\ \hline
\tt tOutputFreq & Time interval between the printing of \textit{frequent output} in program units.  \\ \hline
\tt tOutputInfreq & Time interval between the printing of \textit{infrequent output} in program units.  \\ \hline
\tt tOutputRareFreq & Time interval between the printing of \textit{very infrequent (rare) output} in program units.  \\ \hline
\tt kCutOff & If specified, the given cutoff (\textbf{in program units}) is imposed in the spectrum of initial fluctuations for all scalar fields: the amplitude of the field modes at larger momenta is set to zero up to machine precision. Not specifying {\tt kCutOff} implies not having an initial cut-off, whereas {\tt kCutOff = 0} implies initially vanishing fluctuations. \\ \hline
\tt fixedBackground & If set to {\tt true}, turns off the self consistent expansion and replace it by a fixed background expansion. \\ \hline
\tt omegaEoS & Barotropic equation of state parameter $\omega\equiv p/\rho$ required for a fixed background expansion. Note that {\bf fractions are not allowed}, so one must write e.g.~for a RD universe, `{\tt omegaEoS=0.333}' instead of `{\tt omegaEoS=1/3}'. \\ \hline
\tt H0 & Initial Hubble rate ({\bf in GeV}) used for the fixed background expansion. \\ \hline
\end{tabular}

  \begin{tabularx}{\textwidth}{|>{\tt\small}c|>{\small}c|>{\small}X|>{\small}X|}
    \hline
    \multicolumn{4}{|>{\tt\small\bf}l|}{Required Tools} \\
    \hline
    \normalfont Name & \multicolumn{2}{>{\small}c|}{Minimal Version} & Notes \\
    \hline
     make & \multicolumn{2}{>{\small}c|}{-} & \\
     \hline
     CMake & \multicolumn{2}{>{\small}c|}{3.0} & \\
     \hline
     \multicolumn{4}{|>{\tt\small\bf}l|}{Required Compiler (one of the following)} \\
     \hline
     \normalfont Name & \multicolumn{2}{>{\small}c|}{Minimal Version} & Notes \\
     \hline
      g++ & \multicolumn{2}{>{\small}c|}{5.0} & Minimal version tested: 5.5\\
     \hline
      clang++ & \multicolumn{2}{>{\small}c|}{3.4}  & Minimal version tested: 3.9 \\
      \hline
      \multicolumn{4}{|>{\tt\small\bf}l|}{Required Libraries} \\
      \hline
      \normalfont Name & \multicolumn{2}{>{\small}c|}{Minimal Version} & Notes \\
      \hline
      fftw3 & \multicolumn{2}{>{\small}c|}{3} & Minimal version tested: 3.3.6. \\
      \hline
      \multicolumn{4}{|>{\tt\small\it}l|}{Optional Libraries} \\
      \hline
      \normalfont Name & Minimal Version & Extra Features & Notes \\
      \hline
      MPI & - & Parallelization & Needs an implementation of \texttt{MPI} compatible with the compiler you chose. The most common open-source ones are \texttt{OpenMPI} and \texttt{MVAPICH} (for \texttt{g++} and \texttt{clang++}).  Works only with one of the  parallel Fourier transforms libraries, see below and main text. \\
      \hline
      fftw3\normalfont, parallel version & 3 & Parallelization in one dimension &\texttt{fftw3} compiled for parallel use. Allows only for parallelization in one dimension. \\
      \hline
      PFFT& - &  Parallelization in $n-1$ dimensions. &External library based on the parallel \texttt{fftw3} library. Needs the parallel \texttt{fftw3}.\\
      \hline
      HDF5 & 5 & Saving of 3D distributions to file. Saving of whole simulations. Restarting simulations and automatic backup. & Needs to be separately compiled to work in parallel, see bulk text.  \\
      \hline
      \multicolumn{4}{|>{\tt\small\it}l|}{Optional Tools} \\
      \hline
      \normalfont Name & Minimal Version & Extra Features & Notes \\
      \hline
      git & - & Easy access to the code and easy way to update your code version. &\\
      \hline
  \end{tabularx}

\begin{tabular}{ | m{3.7cm} | m{12.8cm}| } \hline
{\bf Parameters} & {\bf Explanation} \\ \hline
{\tt N} & Number of lattice points per dimension.\\ \hline
\tt kIR  & Infrared cutoff of the lattice \textbf{in program units}, i.e.~$\tilde{k}_{\rm IR} \equiv k_{\rm IR} / \omega_*$.  \\ \hline
\tt lSide  & Length of the box \textbf{in program units}, i.e.~$\tilde{L} \equiv L \omega_*$. \\ \hline
\tt dt & Time step of the evolution algorithm \textbf{in program units}, i.e.~$\delta \tilde{\eta}$.  \\ \hline
\tt expansion & Expanding universe or not.  If {\tt false}, the scale factor is fixed to unity and field dynamics occur in Minkowski. If {\tt true} (default value), the scale factor evolves self-consistently according to the Friedmann equations. A fixed background expansion rate can be further specified by the parameter \texttt{fixedBackground}.\\ \hline
\tt evolver & Type of evolution algorithm. Options `VV2', `VV4', `VV6', `VV8', and `VV10' solve the field equations with the velocity-verlet algorithm of the corresponding order, while `LF' solves them with the staggered-leapfrog method. Check \href{https://www.cosmolattice.net/technicalnotes}{\color{blue} https://www.cosmolattice.net/technicalnotes} for addition of new evolvers. \\ \hline
\tt t0 & Initial time of the simulation \textbf{in program units} (set to 0 by default).  \\ \hline
\tt tMax & Final time of the simulation \textbf{in program units}.  \\ \hline
\tt fixedBackground & If set to {\tt true}, turns off the self consistent expansion and replace it by a fixed background expansion. \\ \hline
\tt omegaEoS & Barotropic equation of state parameter $\omega\equiv p/\rho$ required for a fixed background expansion. Note that {\bf fractions are not allowed}, so one must write e.g.~for a RD universe, `{\tt omegaEoS=0.333}' instead of `{\tt omegaEoS=1/3}'. \\ \hline
\tt H0 & Initial Hubble rate ({\bf in GeV}) used for the fixed background expansion. \\ \hline
\end{tabular}

\begin{tabular}{ | m{3.7cm} | m{12.8cm}| } \hline
{\bf Parameters} & {\bf Explanation} \\ \hline
\tt kCutOff & If specified, the given cutoff (\textbf{in program units}) is imposed in the spectrum of initial fluctuations for all scalar fields: the amplitude of the field modes at larger momenta is set to zero up to machine precision. Not specifying {\tt kCutOff} implies not having an initial cut-off, whereas {\tt kCutOff = 0} implies initially vanishing fluctuations. \\ \hline
\tt baseSeed & Seed for the random generator of initial field fluctuations. If not specified, the seed will be generated randomly in each simulation.   \\ \hline
\end{tabular}

\begin{tabular}{ | m{3.7cm} | m{12.8cm}| } \hline
{\bf Parameters} & {\bf Explanation} \\ \hline
\tt save\_dir & A copy of the simulation is saved at the specified folder at the end of the simulation, which can be loaded as the starting point of a new one.   \\ \hline
\tt backup\_dir & A copy of the simulation is saved at the specified folder at certain times  \\  \hline
\tt tBackupFreq & Time interval between backups in program units (see above).  \\ \hline
\tt load\_dir & The code checks if there is a previously saved simulation in the specified folder, and in that case, it loads it as the starting point.    \\ \hline
\tt appendToFiles & If {\tt false}, new output files are created when a new simulation starts, overwritting previous files with the same name that could exist in the same location. If {\tt true}, the output of the new simulation will be appended to the previously existing files. \\ \hline
\end{tabular}


\begin{tabularx}{\textwidth}{|>{\tt\small}c|>{\small}X|>{\tt\small}c|>{\small}X|}
  \hline
  \normalfont Function name & Operation & \normalfont File & Notes \\
  \hline
  operator+($\phi, \chi$) & $\phi + \chi$ & add.h & Addition of \texttt{ZeroType} is simplified to no addition. \\
  \hline
  operator-($\phi, \chi$)  & $\phi-\chi$ &subtract.h &  Simplifies to no subtraction if one of the input is of \texttt{ZeroType}. Simplifies $\phi-(-\chi)$ to $\phi+\chi$. Handles \texttt{HalfType} and \texttt{OneType} appropriately.\\
  \hline
  operator-($\phi$)  & $-\phi$ &unaryminus.h &  \texttt{-ZeroType} returns \texttt{ZeroType}. Simplifies $--\phi$ to $\phi$.\\
  \hline
  operator*($\phi, \chi$) & $\phi  \chi$ &multiply.h & Multiplication by \texttt{ZeroType} returns \texttt{ZeroType} and multiplication by \texttt{OneType} is simplified away.\\
  \hline
  operator/($\phi, \chi$) & $\phi/\chi$ & divide.h & \texttt{ZeroType}$/\chi$ is simplified to \texttt{ZeroType} and $\chi/$\texttt{OneType} is simplified to \texttt{OneType}.  \\  \hline
  safeDivide($\phi,\chi$) & $\phi/\chi$ & divide.h & Same as \texttt{operator/} except that if $\chi$ is a field, it checks point by point that the divisor is not too small and discards the division if this is so. Useful when dividing by some fields which fluctuates around $0$.  \\  \hline
  abs($\phi$) &$ |\phi|$ & absolutevalue.h&\\
  \hline
  arg($\phi, \chi$) &$\mathrm{arg}(\phi+ i \chi)$ & arg.h&\\
  \hline
  asinh($\phi$) & $\mathrm{asinh}(\phi)$ & asinh.h &  \\
  \hline
  conj($\phi$) & $\phi^*$ & complexconjugate.h &  Useful for the Fourier modes, as they are complex even for a scalar field.\\
  \hline
  cosh($\phi$) & $\mathrm{cosh}(\phi)$ & cosh.h &  \texttt{cosh}(ZeroType) is simplified to OneType.\\
  \hline
  cos($\phi$) & $\mathrm{cos}(\phi)$ & cosine.h &  \texttt{cos}(ZeroType) is simplified to OneType.\\
  \hline
  DiracDelta($\phi$) & $\delta(\phi)$ & diracdeltafunction.h &  \\ \hline
  exp($\phi$) & $\mathrm{exp}(\phi)$ & divide.h & \texttt{exp(ZeroType)} is simplified to \texttt{OneType}.\\
  \hline
  heaviside($\phi$) & $\theta(\phi)$ &heavisidestepfunction.h & \texttt{heaviside(ZeroType)}  and \texttt{heaviside(OneType)} are simplified to \texttt{OneType}.\\
  \hline
  log($\phi$) & $\log(\phi)$ &log.h & \texttt{log(OneType)}  is simplified to \texttt{ZeroType}.\\
  \hline
   pow<N>($\phi$) & $\underbrace{\phi\cdot\dots\cdot \phi}_{N-times}$ &pow.h & Computes the $\mathtt{N}^{th}$ power of $\phi$ by expanding the multiplication, for performance reasons. \texttt{N} must be an integer known at compile time.\\
   \hline
    pow($\phi, \chi$) & $\phi^\chi$ &pow.h & Computes arbitrary power, $\chi$ can even be a field. \texttt{pow(}$\phi$\texttt{, ZeroType)} returns \texttt{OneType} and \texttt{pow(ZeroType, }$\phi$\texttt{)} returns \texttt{ZeroType}.\\
    \hline
     shift<I>($\phi$)  &\texttt{shift<I>(}$\phi$\texttt{)}$|_{\vec n}=\phi|_{\vec n + \hat I}$  &shift.h & Shift the object by a unit vector in the $I^{th}$ direction. \\
    \hline
    shift($\phi$,Tag<I>)  & \texttt{shift(}$\phi$\texttt{, Tag<I>)}$|_{\vec n}=\phi|_{\vec n + \hat I}$ &shift.h & Shift the object by a unit vector in the $I^{th}$ direction. Same as above, different notation.   \\
      \hline
      shift<I,J,K,...>($\phi$)  & \texttt{shift<I,J,...>(}$\phi$\texttt{)}$|_{\vec n}$ \ $=\phi|_{(n_1+I,n_2+J,...)}$& shift.h & Allows to define an object shifted by an arbitrary vector. \\
   \hline
     sin($\phi$)  & $\sin(\phi)$ &sine.h &  \texttt{sin(ZeroType)} returns  \texttt{ZeroType}.\\
    \hline
    sinh($\phi$)  & $\sinh(\phi)$ &sinh.h &  \texttt{sinh(ZeroType)} returns  \texttt{ZeroType}.\\
    \hline
    sqrt($\phi$)  & $\sqrt{\phi}$ &squareroot.h &  Returns \texttt{pow(}$\phi$\texttt{,0.5}. \texttt{sqrt} of \texttt{ZeroType} resp. \texttt{ OneType} returns \texttt{ZeroType} resp. \texttt{ OneType}. \\
    \hline
      safeSqrt($\phi$) & $\sqrt{\phi}$ & squareroot.h & Same as \texttt{sqrt} except that if $\phi$ is a field, it checks point by point that it is positive and discards results (returns $0$) if not. Useful when taking square roots of fields which fluctuates close to $0$.  \\  \hline
    tanh($\phi$)  & $\tanh(\phi)$ &tanh.h &   \texttt{tanh(ZeroType)} returns  \texttt{ZeroType}.\\
    \hline

\end{tabularx}



\begin{longtable}{|r|l|}
\hline
{\bf folder name(s)} & {\bf brief description of each folder}\\\hline
\vspace*{-0.45cm}\, & \, \\\hline\vspace*{-0.3cm}\, & \, \\
{\tt dependencies} & scripts to install external libraries \\
{\tt docs} & documentation files\\
{\tt src} & source code (contains {\tt cmake, models, tests, include})\vspace*{-0.3cm}\\ \, & \, \\\hline
\, & \, \vspace*{-0.3cm} \\
{\tt cmake} & files for compilation\\
{\tt models} & model files\\
{\tt tests} & files for testing purposes\\
{\tt include} & libraries for lattice operations ({\tt TempLat}) and field dynamics ({\tt CosmoInterface})\vspace*{-0.3cm}\\
\, & \, \\\hline
\, & \, \vspace*{-0.3cm}\\

{\tt TempLat} & library for lattice operations \tiny{(contains~{\tt fft, lattice, parallel, parameters, session, util})}\\
{\tt CosmoInterface} & library for field dynamics \tiny{(contains {\tt definitions, evolvers, initializers, measurements})}
\vspace*{-0.3cm}\\
\, & \, \\\hline
\, & \, \vspace*{-0.3cm}\\
{\tt fft} & library to handle Fourier transformations\\
{\tt lattice} & library for basic lattice definitions and field operations\\
{\tt parallel} & library for parallelization routines\\
{\tt parameters} & library for parsing parameters from the command-line/files.\\
{\tt session} & library for taking care of initialization and destruction of external libraries \\
{\tt util} & library for basic useful operations \vspace*{-0.3cm}\\
\, & \, \\\hline
\, & \, \vspace*{-0.3cm}\\
{\tt definitions} & library for basic field definitions (EOM terms, energy terms, etc)\\
{\tt evolvers} & library for evolution algorithms\\
{\tt initializers} & library for initialization algorithms\\
{\tt measurements} & library for observables (energy densities, field spectra, etc)\vspace*{-0.3cm}\\\, & \, \\\hline
\end{longtable}



"""
print("Testing processing function...")
text, labels = process_lines(test_string)
print("Processed text:")
print(text)
print("Labels found:")
print(labels)
