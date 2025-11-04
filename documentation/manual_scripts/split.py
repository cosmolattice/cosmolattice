from multiprocessing import util
import sys
import regex as re

from util.regex import global_pass
from util.blocks import get_blocks
from util.bibliography import build_bibliography

from util.default import default_block
from util.math import math_block
from util.figure import figure_block
from util.table import table_block

from util.regex import final_pass


def process_lines(lines, tmpdir="./"):
    lines = "".join(lines)
    lines = global_pass(lines)

    label_dict = {}

    ##############################################
    # Split into blocks
    ##############################################

    blocks = get_blocks(lines)
    for i in range(len(blocks)):
        ################################################
        # process default blocks
        ################################################
        if blocks[i][0] is None:
            blocks[i][1], new_label_dict = default_block(blocks[i][1], tmpdir)
            label_dict.update(new_label_dict)
        ################################################
        # process math blocks
        ################################################
        if blocks[i][0] == "math":
            blocks[i][1], new_label_dict = math_block(blocks[i][1])
            label_dict.update(new_label_dict)
        ################################################
        # process latex blocks
        ################################################
        if blocks[i][0] == "figure":
            blocks[i][1], new_label_dict = figure_block(blocks[i][1], tmpdir)
            label_dict.update(new_label_dict)
        ################################################
        # process table blocks
        ################################################
        if blocks[i][0] == "table":
            blocks[i][1], new_label_dict = table_block(blocks[i][1], tmpdir)
            label_dict.update(new_label_dict)
    lines = "".join([block[1] for block in blocks])

    # Global final pass
    lines = final_pass(lines)

    # Final cleanup of multiple new lines
    while True:
        old_lines = lines
        lines = re.sub(r"```math\n\n", r"```math\n", lines)
        lines = re.sub(r"\n\n```", r"\n```", lines)
        lines = re.sub(r"\\begin\{align\}\n\n", r"\\begin{align}\n", lines)
        lines = re.sub(r"\n\n\\end\{align\}", r"\n\\end{align}", lines)
        lines = re.sub(r"\n\n+", r"\n\n", lines)
        if lines == old_lines:
            break

    return lines, label_dict


def rebuild_labels(lines, label_dict):
    # once all labels are known, we can rebuild the references
    for label_name, object_name in label_dict.items():
        lines = re.sub(
            r"\\ref\{\s*" + re.escape(label_name) + r"\s*\}",
            r"[*" + object_name + r"*][" + label_name.replace(":", "_") + r"]",
            lines,
        )
    return lines


# For testing purposes
if len(sys.argv) != 4:
    test_string = r"""
\section{Useful features: parallel support, backing up and others}\label{sec:UsefulFeatures}

There is clearly a vast number of physical scenarios that can be implemented in \CLns, and in order to optimize the ``physics output" from many different scenarios, we have made available a number of powerful technical features in the code. The most relevant one is the possibility of directly running any model written in \CL with multiple processors in parallel, without any modification whatsoever of the code. As we will shortly show, all it takes for the user to run their model on potentially hundreds or even thousands of processors, is a simple flag passed to the \texttt{CMake}. Before explaining this, we give in Section \ref{subsec:para} a brief explanation about what parallelization means, as well as describe what happens technically at the computation level. Any user not interested in these technical details may want to skip directly to Sections~\ref{subsubsec:para1D} and \ref{subsubsec:para2D}, where we simply explain how they can activate the parallelization option in \CLns.

Another useful feature provided by \CL is the possibility of saving up and restarting simulations, as well as the possibility of having an automatic backup every given number of iterations. This feature can also be enabled through a \texttt{CMake} flag; we elaborate about it in Section~\ref{subsec:hdf5spec}. Using the same external library (\texttt{HDF5}), we also provide the user with the possibility of saving spectra in \texttt{HDF5} format, which has the advantage of being more structured than the default text files. We explain this also in Section~\ref{subsec:hdf5spec}.

\subsection{Parallelization}
\label{subsec:para}

As we increase the size of our lattice simulations, we quickly encounter computational limitations. These are of two types. First, we are limited by the real duration (as counted by hours/days/etc by ourselves) that it takes to run a given simulation. Every time the number of points/dimension $N$ of a lattice is doubled, the execution time increases roughly by a factor $\sim 2^d$, with $d$ the number of dimensions. That is, the execution time scales with the volume of the lattice. Secondly, the memory (RAM) needed to perform the simulation also scales with the volume, as every time $N$ is doubled, the amount of required RAM memory increases by a factor $8$. Lack of memory is often a more severe limitation than the execution time, as longer execution times may be compensated by more patience (at least to some extent), while the limit on memory can not.%is a physical impediment.

Both of these hindrances can be sharply mitigated by a simple idea: the use of more than one computer to perform the simulations. This is what we mean by parallelization. In spirit, it works as follows: given $n_c$ ``computers" (or ``cores", as we will refer to them), you can split your lattice into $n_p$ smaller sub-lattices. Then, instead of evolving the whole lattice on a single process, you can evolve the $n_p$ smaller sub-lattices on one or several of your $n_c$ cores, and then combine their results whenever needed. In theory, this would speed up your simulation by a factor $n_p$, and give you access to $n_p$ times more memory.

Of course, most of the problems suitable for lattice simulations involve some spatial derivatives or some kind of finite range interaction, and as a result,
the system evolved over the original lattice is not equivalent to the $n_p$ systems over smaller lattices. In the case of systems of equations which involve an interaction between neighboring sites\footnote{Of course, the idea presented here also works and is practical for interactions between sites that are a few sites apart.}, in order to be able to solve the system over the whole original lattice but splitting the evolution over the smaller lattices, it is enough for every sub-lattice to be aware of the values of the fields in the sites of its neighboring lattices, those directly adjacent to their own sides. This is clarified in Fig.~\ref{fig:1d}, where we consider the one-dimensional case and explain the case with two cores. The physical lattice $\Lambda$ consist of the field values $\phi_0$ to $\phi_7$. To perform the computation, we can subdivide it into two smaller lattices $\Lambda_1$ and $\Lambda_2$. The first one, which contains the field values $\phi_0$ to $\phi_3$, is assigned to the first process. The second, containing the values $\phi_4$ to $\phi_7$, is assigned to the second process. Now imagine that, in order to solve our system of equations, we need to compute a gradient, which we write as a forward derivative [recall Eq.~(\ref{eq:forwardbackwardd})]. When our first process tries to evaluate it around site $3$, it needs to compute $\frac{\phi_4-\phi_3}{\delta x}$, i.e.~it requires the information on the value of $\phi_4$, which belongs however to the adjacent sub-lattice.

To solve this problem, we introduce {\it ghost cells}. When two sub-lattices have a common boundary, the boundary values are stored in both sub-lattices, and whenever they are modified, the new values are communicated to their neighboring sub-lattice; boundaries are ``exchanged". Very explicitly, in our one-dimensional example of Fig.~\ref{fig:1d}, we can add an extra site (the ghost cell) to $\Lambda_1$ containing $\phi_4$, and an extra site to $\Lambda_2$ containing $\phi_3$. Whenever $\phi_3$ is modified in $\Lambda_1$, it needs to be communicated to $\Lambda_2$, and whenever $\phi_4$ is modified in $\Lambda_2$, it needs to be communicated to $\Lambda_1$. Of course, since we use periodic boundary conditions, the same needs are in place with respect the boundaries $\phi_0$ and $\phi_7$.

In higher spatial dimensions, the geometry of the boundaries to be exchanged might become more complicated, but the intrinsic idea remains the same. This is the parallelization idea implemented in \CLns, based on the use of the {\it Message Passing Interface} (\texttt{MPI}), a standard library to program the boundary exchanges. We discuss in the next sections two different parallelization strategies, and how the user can choose between one or another when using \CLns.


\begin{figure}
  \centering
  \begin{tikzpicture}

  \draw (0,0) -- (7,0);
  \foreach \x in{0,...,7}
  {
      \node at (\x ,0){$\times$};
      \node at (\x ,0.4){$\phi_\x$};
  }
  \draw[dashed](3.5,-1)--(3.5,1);

  \draw [decorate,decoration={brace,mirror,amplitude=10pt},xshift=0cm,yshift=-0.pt]
(0,0) -- (3.5,0) node [black,midway,yshift=-0.6cm]
{\footnotesize $\Lambda_1$};
\draw [decorate,decoration={brace,mirror,amplitude=10pt},xshift=0cm,yshift=-0.pt]
(3.5,0) -- (7,0) node [black,midway,yshift=-0.6cm]
{\footnotesize $\Lambda_2$};
\draw [decorate,decoration={brace,amplitude=10pt},xshift=0cm,yshift=-0.pt]
(0,0.75) -- (7,0.75) node [black,midway,yshift=0.7cm]
{\footnotesize $\Lambda$};

\node at (3.5,-1.5){\small Boundary};


\draw[->, color=gray] (-0.3,-0.2) to[out=200,in=160] (-0.3,-4.8);
\node[color=gray] at (-0.6,-2.15){\small Add};
\node[color=gray] at (-0.6,-2.65){\small ghost cells};

\draw (0,-5) -- (3,-5);
\draw[dotted] (3,-5) -- (4,-5);
\draw (4,-4.5) -- (7,-4.5);
\draw[dotted] (3,-4.5) -- (4,-4.5);
\foreach \x in{0,...,3}
{
    \node at (\x ,-5){$\times$};
    \node at (\x ,-5.4){$\phi_\x$};
}
\node[color=gray] at (4 ,-5){$\times$};
\node[color=gray] at (4 ,-5.4){$\phi_4$};
\draw [decorate,decoration={brace,mirror,amplitude=10pt},xshift=0cm,yshift=-0.pt]
(0,-5.7) -- (4,-5.7) node [black,midway,yshift=-0.6cm]
{\footnotesize $\Lambda_1$};

\draw [decorate,decoration={brace,amplitude=10pt},xshift=0cm,yshift=-0.pt]
(3,-3.8) -- (7,-3.8) node [black,midway,yshift=0.6cm]
{\footnotesize $\Lambda_2$};
\foreach \x in{4,...,7}
{
    \node at (\x ,-4.5){$\times$};
    \node at (\x ,-4.){$\phi_{\x}$};
}
\node[color=gray] at (3 ,-4.5){$\times$};
\node[color=gray] at (3 ,-4.){$\phi_{3}$};
\draw[dashed](3.5,-6)--(3.5,-4);

  \end{tikzpicture}
  \caption{Parallelization of a one-dimensionnal lattice (line). The physical lattice $\Lambda$ is split into two sublattices $\Lambda_1$ and $\Lambda_2$. In order to be able to consistently solve systems of equations involving derivatives, the sublattices need to know about the value of the fields just over the boundary. This is achieved by introducing ``ghost cells" to store this information. Every time a boundary value is modified, the two lattices need to ``exchange their boundary"; $\Lambda_1$ communicates the new value of $\phi_3$ to $\Lambda_2$ while $\Lambda_2$ communicates the new value of $\phi_4$ to $\Lambda_1$. Were we to consider periodic boundary conditions, we would also need ghost-cells and boundary exchange around sites $0$ and $7$.}
  \label{fig:1d}
\end{figure}


\begin{figure}
  \centering
  \begin{tikzpicture}
    \foreach \x in{0,...,3}
    {
        \draw (\x ,0,3) -- (\x ,3,3);
        \draw (\x ,3,3) -- (\x ,3,0);

    }

    \draw (3,0 ,3) -- (3,0 ,0);
    \draw (0,0 ,3) -- (3,0 ,3);
    \draw (0,3 ,3) -- (3,3 ,3);



      \draw (3,0,0 ) -- (3,3,0 );
      \draw (0,3,0 ) -- (3,3,0 );

      \foreach \x in{1,2,3}
      {
        \node at (\x-0.5,1.5,3){$\Lambda_\x$};
      }


      \node at (0.9,4,0){Parallelization in one direction};

    \newcommand{\xshift}{6.5}
  \foreach \x in{0,...,3}
  {   \draw (\xshift,\x ,3) -- (\xshift+3,\x ,3);
      \draw (\xshift+\x ,0,3) -- (\xshift+\x ,3,3);
      \draw (\xshift+3,\x ,3) -- (\xshift+3,\x ,0);
      \draw (\xshift+\x ,3,3) -- (\xshift+\x ,3,0);
  }
    \draw (\xshift+3,0,0 ) -- (\xshift+3,3,0 );
    \draw (\xshift,3,0 ) -- (\xshift+3,3,0 );


    \draw[-] (-2.8,0,2) -- (-2.8,1,2);
    \draw[-] (-2.8,0,2) -- (-1.8,0,2);
    \draw[-] (-2.8,0,2) -- (-2.8,0,3);
    \node at (-2.9,-0.1,3.1) {$z$};
    \node at (-1.6,0,2) {$x$};
    \node at (-2.8,1.2,2) {$y$};

    \foreach \x in{1,2,3}
    {
      \node at (\x+\xshift-0.5,2.5,3){$\Lambda_{1\x}$};
    }
    \foreach \x in{1,2,3}
    {
      \node at (\x+\xshift-0.5,1.5,3){$\Lambda_{2\x}$};
    }
    \foreach \x in{1,2,3}
    {
      \node at (\x+\xshift-0.5,0.5,3){$\Lambda_{3\x}$};
    }

    \node at (\xshift+0.9,4,0){Parallelization in two directions};


  \end{tikzpicture}
  \caption{\textbf{Left:} Parallelization in one direction. The physical lattice is split into slices along the $x$ direction, and each process deals with the fields' evolution in a separate slice. The boundary region between slices needs to be exchanged between cores to be able to perform operations involving neighboring points, such as computing spatial derivatives. In our current implementation, the number of points in the $x$ direction should be a multiple of the number of cores used. \textbf{Right:} Parallelization in two directions. The physical lattice is divided along the $x$ and $y$ directions into parallelepipeds as drawn on the figure. Boundaries also need to be exchanged, for instance to compute spatial derivatives. In the current implementation, the number of points/dimension $N$ needs to be a multiple of the number of cores. }
  \label{fig:parageo}
\end{figure}



\subsubsection{Parallelization in one direction: \texttt{MPI}}
\label{subsubsec:para1D}

As briefly discussed in the previous section, the parallelization of ``local" operations, like solving finite difference systems,  is relatively straightforward. However, this is not the case for ``non-local" operations such as Fourier transforms. Typical simulations performed through the \texttt{CosmoInterface} require Fourier transforms, in order to e.g.~setting the initial fluctuations of the fields, or computing their spectra. \CL relies on the standard \texttt{fftw3} library to perform Fourier transforms. In its current version, this library does allow to parallelize multi-dimensional Fourier transforms, but only along a single direction. As a result, if one is not willing to use any extra library besides \texttt{fftw3}, the parallelization of a lattice simulations can only be done along one spatial direction. This leads to the decomposition presented in the left-hand side of Fig.~\ref{fig:parageo}. Note that in the current implementation of \CLns, the linear size $N$ of the lattice must be an integer multiple of the number of cores you want to use. For instance, in Fig.~\ref{fig:parageo}, as we want to use three cores, $N$ must be a multiple of three.

It is very easy to activate this parallelization procedure in \CLns. Assuming you have installed \texttt{MPI} and a properly compiled version of \texttt{fftw3} (see Appendix~\ref{app:Installation} for more information, installation instructions for these libraries and guidance to use them on HPC clusters), you simply need to pass an extra flag \texttt{-DMPI=ON} to \texttt{CMake} before compiling your model:
\begin{shell-sessioncode}
 cmake -DMPI=ON -DMODEL=lphi4 ../
 make cosmolattice
\end{shell-sessioncode}
Of course, if you want to compile any other model (including the ones with gauge fields), you simply need to replace \texttt{lphi4} by the name of your model, as explained in Sections~\ref{sec:MyFirstModelScalars} and \ref{sec:MyFirstModelGauge}.

After having successfully compiled \CLns, you can run it with \texttt{nc} cores with \texttt{nc}$\geq 1$. Of course you need to have access to such number of CPU's; a typical laptop will have between one and four, whereas you can use even thousands of cores on a HPC cluster. This is done as follows,
\begin{shell-sessioncode}
mpirun -n nc lphi4 input=...
\end{shell-sessioncode}
Note that if you are using a high-performance-computation (HPC) cluster, you will typically have to use another command to run your parallel jobs.

\subsubsection{Parallelization in two directions: \texttt{MPI} and \texttt{PFFT}}
\label{subsubsec:para2D}

If we are willing to use some extra external libraries to compute Fourier transforms, we can actually overcome the limitation of \texttt{fftw3} and use a parallelization across multiple spatial directions. In the current implementation of \CLns, we use the \texttt{PFFT} library~\cite{Pi13}, see again Appendix~\ref{app:Installation} for installation instructions. This in principle allows us to parallelize the simulation in all directions. In practice, because of the overload due to the boundary exchanges, it is often a good compromise to parallelize in all dimensions except one, which involves less cores, but also less boundaries. We depict the resulting parallelization strategy for the case of three spatial dimensions in the right-hand side of Fig.~\ref{fig:parageo}. In this case, the number of sites/dimension $N$ of the lattice needs to be divisible by the number of cores used in each parallelized direction. In practice, %in the \texttt{CosmoInterface} case where
when all directions have the same number of points, \texttt{N} needs to be an integer multiple of the number of cores.

To switch to this parallelization setting, again assuming you have a working installation of \texttt{MPI}, \texttt{fftw3} and now \texttt{PFFT} (see Apendix~\ref{app:Installation}), you simply need to pass the extra flag \texttt{-DPFFT=ON} to \texttt{CMake}, before compiling your favorite model
\begin{shell-sessioncode}
 cmake -DMPI=ON -DPFFT=ON -DMODEL=lphi4 ../
 make cosmolattice
\end{shell-sessioncode}
Note that this flag must be used together with the \texttt{-DMPI=ON} flag.

Nothing changes in this case to execute a run, as you can send a job again using the command:
\begin{shell-sessioncode}
mpirun -n nproc lphi4 input=...
\end{shell-sessioncode}
(or whichever is the equivalent command needed in your HPC cluster).

\subsubsection{Performances}

Before explaining some of the other features of the code, we want to show how good \CL can do as a parallel code. As an example, we will study how the execution time of the \texttt{lphi4SU2U1} model scales as a function of the used number of cores. Be aware that this kind of study has to be considered with care, as the quantitative results may depend on the type of hardware used, the actual state of the cluster when performed, the compiler, or the \texttt{MPI} implementation. Having noted this, we will show that the \CL parallelization performs very well, and that the possibility of having a Fourier transform in more than one dimension provides a significant advantage when a large number of cores are required, let it be because of memory or execution time requirements.

\begin{figure}
  \centering
  \includegraphics{speed_up}
  \caption{Speed up factor in parallelized simulations as a number of cores, as test on the \texttt{Gacrux} cluster from the EPFL HPC center SCITAS, Switzerland.}
  \label{fig:speedup}
\end{figure}

For simplicity, we choose a relatively small lattice with $N=112$ points/dimension, which we ran for $250$ time iterations. We perform $25$ ``frequent" measurements (mean values) and $6$ ``infrequent" measurements (spectra). We performed this benchmark on the \texttt{Gacrux} cluster\footnote{One node is made of two Intel Broadwell processors running at $2.6$ GHz, with $14$ cores each (hence $28$ codes/node). As node connectivity, it uses Infiniband EDR.} from the
\'Ecole polytechnique f\'ed\'erale de Lausanne (EPFL) HPC center SCITAS. We show the results in Fig.~\ref{fig:speedup}, where we plot the speed-up of the program as a function of the number of cores. In particular, we show the speed-up factor $S$, which is defined as the execution time in one core $T_1$ divided by the execution time in $n$ cores $T_n$, i.e.~$S \equiv T_1/T_n$. It is important to remark that our test case gives too much importance to the initialization of the fields relative to their evolution ($250$ time steps is orders of magnitude smaller than in a realistic simulation). The initialization functions are dominated by Fourier transforms, which are not expected to scale as good as the evolution routines. In any case, we obtain very satisfying speed-ups. Perhaps the most interesting feature of this figure is the comparison between the one-direction parallelization strategy via \texttt{fftw3} and the two-directions parallelization strategy via \texttt{PFFT}. It appears that up $\frac{N}{\# cores}\gtrsim 2$, both strategies perform equally well. The maximum number of cores we can have with the first strategy is $\# cores = N$. In this case, we already see it being outperformed by the second strategy. But more importantly, it increases the maximum number of cores we can use. For instance, in Fig.~\ref{fig:speedup} we show our benchmark running on up to $392$ cores with good performances.

To conclude this benchmark, we can attempt to make a more quantitative description of the goodness of the performance of \CLns, restricting our attention to the results obtained on one node; the one obtained on more nodes is harder to analyze as they can be relatively sensitive to hardware-dependent performance fluctuations. They are also sensitive to the efficiency of inter-nodes communications, which require some modeling beyond the scope of this section.

If we want to quantify how much of our code is actually parallelized, we can use a relation referred to as Amdhal's law \cite{conf/afips/Amdahl67}. Assume $\alpha\%$ of your code is parallelized. The execution time $T_1$ on a single core can then be written as $T_1=\alpha T_1 + (1-\alpha) T_1$. On $n_{cores}$ cores, it becomes $T_{n}=\left(\frac{\alpha}{n_{cores}} +1-\alpha\right)T_1$. Amdhal's law is the prediction of the speed-up you get from this relation,
\begin{equation}
  S=\frac{1}{\frac{\alpha}{n_{cores}} +1-\alpha} \ .
\end{equation}
By fitting our data on one node, we obtain $\alpha\approx 0.99$, which means that effectively $99\%$ of \CL is parallelized. Note that in actual simulations, we expect this number to be even better, as the invested amount of time in the fields' initialization will be even more subdominant with respect to their evolution. Note also that we performed this benchmark with the full matter content available to \CL lattice. We expect similar results for the scalar sector alone.


\subsection{Saving three-dimensional field distributions, backups and other options: \texttt{HDF5}}
\label{subsec:hdf5spec}

When running long simulations, it may come very handy to be able to stop them and restart them later on, or to have some kind of automatic backup in case some problem happens to the hardware you are using. In order to implement this type of features, we need to be able to save the field distributions to a file. For the sake of portability, the current version of \CL uses the \texttt{HDF5} library to perform this task in a binary format. This means that, if you want to use one of the features that involve saving a three-dimensional distribution of some fields to a file, you will need to have a working \texttt{HDF5} library installed (see Appendix~\ref{app:Installation} on how to do this). Assuming you have such installation, activating these features in \CL is as simple as using another \texttt{CMake} flag:
\begin{shell-sessioncode}
 cmake -DHDF5=ON -DMODEL=lphi4 ../
 make cosmolattice
\end{shell-sessioncode}
We will now survey what features this unlocks.

\subsubsection{Saving a simulation to disk}
After having activated \texttt{HDF5}, we can now save runs to disk. This is simply done via the argument \texttt{save\_dir}, which you can add to your input file, or simply pass it through the command line. For instance,
\begin{shell-sessioncode}
./lphi4 input=input.in save_dir=./
\end{shell-sessioncode}
will save this \texttt{lphi4} run at the end in the current folder. It is going to create a file named \texttt{lphi4\_DATE\_d**\_m**\_y**\_TIME\_h**\_m**\_s**.h5}, where \texttt{lphi4} is the model name, and the $**$ symbols will be replaced by the actual date and time.

You do not need to know anything about the actual content of the file in order to restart your simulation from there. However, thanks to the standardized \texttt{HDF5} format, you can easily go and explore the content of the file with your favorite data visualization tool, be it {\tt gnuplot}, {\tt Mathematica}, {\tt Matlab}, {\tt python}, {\tt Julia} or other, as long as it supports \texttt{HDF5}. To simplify, a \texttt{HDF5} file mimics a folder/file structure, folders being designated as ``groups" and files as ``datasets". In this case, every field is stored in a separate dataset. For simplicity, we also store the values of the scale factor, its time derivative, and the final time as separate datasets.

\subsubsection{Restarting a saved simulations}

Once a saved simulation file has been created, it is straightforward to restart the simulation from the same time when you stopped it and saved it. To do so, you only need to call your executable with the ``load\_dir" parameter set to the simulation file you want to restart from. It will also read the parameters of the previous simulation and use them. Except for the lattice size $N$, the length side $L$, and the infrarred and ultraviolet cutoffs $k_{\rm IR}$ and $k_{\rm UV}$, you can override the other parameters by either specifying them through the console line or in an output file. Not that if you try to override a parameter that you are not allowed to, the program will not crash, but simply ignore your changes.

To be concrete, let us assume that the above simulation was saved at the time \texttt{tMax=200}. If we want to continue the simulation, we can simply relaunch it with a different parameter (say  \texttt{tMax=500}) as follows,
\begin{shell-sessioncode}
./lphi4 load_dir=lphi4_DATE_d**_m**_y****_TIME_h**_m**_s**.h5 tMax=500
\end{shell-sessioncode}
As mentioned above, you can also use an input file as usual.

Note that when you run in restart mode, assuming you have not moved the previous output files, the new results will be overwrite the previous files. You can change this behavior by setting explicitly the parameters \texttt{appendToFiles} to \texttt{true}.


\subsubsection{Automatic backup}

With the ``start and stop" mechanism presented in the section above, it is natural to implement an automatic way of backing up the simulation to disk, in order to be able to recover from some hardware failure. This option is turned on by specifying the parameter \texttt{tBackupFreq}. Then, every \texttt{tBackupFreq} amount of program time, the simulation will write itself to disk in a file name \texttt{ModelName.backup} (\texttt{lphi4.backup} for instance). If a backup file is already present, it will first rename it to \texttt{ModelName.backup\~} before creating the backup. This extra amount of precaution allows you not to loose the whole simulation in case your hardware crashes while you are backing-up. By default, the backup file is saved in the same folder than the measurements. You can change this behaviour by specifying the \texttt{backup\_dir} parameter.

\textit{Note:} In the current implementation of \CLns, the saving of three-dimensional field configuration has not particularly been optimized for performances. As such, it is a good idea not to use a backing up frequency that is too high. You can determine what ``too high" means by trial and error, seeing how much the backing up affects performance on your hardware.

\subsubsection{Saving three dimensional energy distributions}

\CL is also capable of saving three-dimensional distributions of arbitrary observables in a file. At present, the user can save three-dimensional distributions of the various energy components of the system by adding different flags to the \texttt{energy\_snapshot} parameter in the parameter file. The different flags are indicated in Appendix \ref{App:TableParameters}. Let us show an example: suppose we are running the \texttt{lphi4SU2U1} gauge model and we want to save to file the scalars kinetic energies and the $SU(2)$ electric energy. We would then run
\begin{shell-sessioncode}
./lphi4SU2U1 input=input.in energy_densities="E_S_K E_B_K"
\end{shell-sessioncode}
Again, as usual, this parameter can go in the input file (in which case you do not need the quotes surrounding the arguments).

\subsubsection{A more user-friendly format for the spectra}
\label{subsubsec:hdf5spectra}

If you want to compute the field spectra with a very fine resolution binning on large lattices, the corresponding text files storing them may occupy a significant amount of disk memory. However, if you compile \CL with \texttt{HDF5}, you have access to a new way of storing the spectra. %The way they are stored in the normal text files may be a bit cumbersome to read. Moreover,
In particular, this problem can be mitigated if the spectra are saved in \texttt{HDF5} format, which are binary files. Furthermore, thanks to the internal structure of \texttt{HDF5} files, spectra at different times can easily be retrieved. Our \texttt{HDF5} spectra files are structured as follows. First, every time is its own group (``folder"). Inside this group, there will be a dataset called \texttt{momBinAverage}, which contains the average momentum in a bin, as well as another one called \texttt{momBinMultiplicity} which tells you how many values where binned in this bin. Then, there is a dataset for each of the $n$ spectra saved in the given file, named \texttt{spectAverage\_i} with \texttt{i}$=0,1,\dots,n-1$. This information is always printed for the default spectra verbosity, but if you choose a higher verbosity, you will also get a dataset containing the variance, minimum and maximum values of the momenta and spectra bins.

"""
    print("Testing processing function...")
    print(process_lines(test_string))
    print("Usage: python split.py inputfile outputfolder tmpdir")
    sys.exit(1)

input = sys.argv[1]
outputfolder = sys.argv[2]
tmpdir = sys.argv[3]

section_sets = []

with open(input, "r", encoding="utf-8") as file:
    section_name = "preamble"
    lines = []
    label = None

    all_lines = file.readlines()

    label_dict = {}

    for i, line in enumerate(all_lines):
        # if the line contains "\\section", start a new file
        if "\\section" in line or r"\begin{thebibliography" in line:
            if section_name != "preamble":
                print(f"---- Processing section: {section_name}")
                # first, write the previous section to a file
                lines, new_label_dict = process_lines(lines, tmpdir)
                section_sets.append((section_name, lines, label))
                label_dict.update(new_label_dict)

            # reset lines for the new section
            if r"\begin{thebibliography" in line:
                section_name = "Bibliography"
                lines = []
            else:
                section_name = line.split("{")[1].split("}")[0].strip()
                section_name = global_pass(section_name)
                # see if this or the next line contains a label for the section
                label_search = re.search(r"\\label\{(.+?)\}", line)
                if not label_search:
                    next_line = all_lines[i + 1]
                    label_search = re.search(r"\\label\{(.+?)\}", next_line)
                if label_search:
                    label = label_search.group(1)
                else:
                    label = None
                lines = ["## " + section_name + "\n"]

                # print(f"New section found: {section_name}")
        else:
            lines.append(line)

    # Last section, assert that this is Bibliography
    assert section_name == "Bibliography"
    print(f"---- Processing Bibliography")
    bibrules_outer = build_bibliography(input, tmpdir, "\n".join(lines))

    for section_name, lines, label in section_sets:
        for old, new in bibrules_outer.items():
            lines = re.sub(old, new, lines)

        for _section_name, _lines, _label in section_sets:
            if _label:
                lines = re.sub(
                    r"\\ref\{\s*" + re.escape(_label) + r"\s*\}",
                    r"[" + _section_name + r"](" + _section_name + r".md)",
                    lines,
                )

        lines = rebuild_labels(lines, label_dict)

        # print(f"Writing section: {section_name}")
        # first, write the previous section to a file
        with open(
            f"{outputfolder}/{section_name}.md", "w", encoding="utf-8"
        ) as outfile:
            outfile.write(lines)

    # Write the navigation file
    with open(f"{tmpdir}/nav.yml", "w", encoding="utf-8") as navfile:
        for section_name, lines, label in section_sets:
            navfile.write('      - "' + section_name + '": "' + section_name + '.md"\n')

    toc = []
    for section_name, lines, label in section_sets:
        toc.append(f"- [{section_name}]({section_name}.md)\n")
    # Write the index file
    with open(f"{outputfolder}/index.md", "w", encoding="utf-8") as indexfile:
        indexfile.write("# Cosmolattice Manual" + "\n\n")
        indexfile.write(
            "This website has been generated from the [CosmoLattice user manual](https://arxiv.org/abs/2102.01031)\n"
        )
        indexfile.write("## Table of contents" + "\n\n")
        indexfile.writelines(toc)
        indexfile.write("## About" + "\n\n")
        indexfile.write("...")
