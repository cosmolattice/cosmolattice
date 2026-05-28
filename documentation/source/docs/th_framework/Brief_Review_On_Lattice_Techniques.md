!!! note
    **Note:** If the reader is already familiar with scalar and gauge field lattice simulations, or they have already read Section 3 of Ref. [@Figueroa_2020rrl], they can skip this section and jump right ahead into Sections [My first model of (singlet) scalar fields](My first model of (singlet) scalar fields.md) or [My first model of gauge fields](My first model of gauge fields.md) of the present document, in order to set up their first scalar or scalar-gauge field lattice simulations, respectively. If the reader is familiar with scalar field lattice simulations but not with gauge lattice field theories, we recommend them to read at least Section [*Lattice gauge invariant techniques*][subsec_LGT]. The present section [Brief review on lattice techniques](Brief review on lattice techniques.md) represents, in any case, a summary of the more extended discussion about basic lattice concepts presented in Section 3 of Ref. [@Figueroa_2020rrl].

### Basic lattice definitions

CosmoLattice simulates the dynamics of interacting fields in a regular cubic lattice of $N^{d}$ points in total, with $N$ the number of lattice sites per dimension, and $d$ the number of spatial dimensions. In in this document we set $d = 3$ unless otherwise specified, as CosmoLattice works by default in 3-spatial dimensions. The complete set of points in a lattice can then be labeled as
```math
\begin{align}
{\bf n} = (n_1,n_2,n_3),  \quad{\rm with}\,\,  n_i = 0,1,...,N-1  , i = 1,2,3 .
\end{align}
```

We note that it is actually such set of points that is collectively referred to as *the lattice*. Alternatively,  we will also refer to them as *the grid*, or even more colloquially, as *the box*. In the case of scalar field theories, CosmoLattice also allows for simulations in $d = 2$ and $d=1$ spatial dimensions\footnote{Let us note that the usability of the dynamics in lower spatial dimensions has not been robustly tested in the same manner as $d=3$, and hence any results obtained with this must be taken with great care. We plan to review this feature and guarantee its robustness in the near future.}, so that the lattice sites would be labelled in such cases as ${\bf n} = (n_1,n_2)$, or ${\bf n} = n_1$, respectively. For convenience, we stick to $d = 3$ in the following discussion. Apart from $N$, the user must also choose a *length side* $L$ for the box. After fixing $N$ and $L$, the smallest possible distance between two sites, the so called *lattice spacing*, is given by
```math
\delta x \equiv \frac{L}{N}  .
```

!!! note
    ***Important to know:*** For serial runs (i.e. jobs running in a single processor) the number of points per dimension $N$ can be arbitrary. For parallelized runs using MPI (i.e. jobs running simultaneously in multiple processors), $N$ must be divisible by the number of processors $n_p$ if the parallelization is in one spatial dimension, whereas $N$ must be divisible by both $n_1$ and $n_2$ when the paralellization is done in two spatial dimensions in a number $n_p = n_1\cdot n_2$ of processors. See Section [Useful features: parallel support, backing up and others](Useful features: parallel support, backing up and others.md) for further clarifications.

A continuum function ${\tt f}(\bf x)$ in space is represented on the lattice by a function $f({\bf n})$, which has the same value as ${\tt f}(\bf x)$ at ${\bf x}={\bf n} \delta x$. We note that whereas in a flat background, positions $\lbrace \bf x \rbrace$ and their corresponding lattice sites $\lbrace \bf n \rbrace$ represent physical spatial coordinates, in an expanding background they will represent *comoving* spatial coordinates. Unless specified otherwise, we consider *periodic boundary conditions*, so that $f({\bf n} + \hat \imath N) = f({\bf n})$, $i  = 1,2$ or $3$, with $\hat 1 \equiv (1,0,0)$, $\hat 2 \equiv (0,1,0)$ and $\hat 3 \equiv (0,0,1)$, unit vectors corresponding to positive displacements of one lattice spacing in each independent direction $x$, $y$ and $z$ on the lattice. The periodic boundary conditions in coordinate space imply that the momenta must be discretized, whereas the discretization of the spatial coordinates implies that any definition of a discrete Fourier transform must be periodic. For each lattice we can always consider a *reciprocal lattice* representing $Fourier$ modes, with its sites labeled as
```math
\begin{align}
\tilde{\bf n} = (\tilde n_1, \tilde n_2, \tilde n_3),  \quad{\rm with}\,
\tilde n_i = -\frac{N}{2}+1, -\frac{N}{2}+2, ... ,-1,0,1, ... , \frac{N}{2} - 1, \frac{N}{2},\,\,\,  i  = 1,2,3 .
\end{align}
```

A discrete Fourier transform (DFT) is then defined by
[](){ #eq_FTdiscrete }
```math
\begin{align}\tag{16}
f({\bf n}) \equiv {1\over N^3}\sum_{\tilde n} e^{-i{2\pi\over N} {\bf \tilde n n}} f({\bf \tilde n})   \Leftrightarrow    f({\bf \tilde n}) \equiv \sum_{n} e^{+i{2\pi\over N} {\bf n \tilde n} }f({\bf n}) ,
\end{align}
```

from where it follows that Fourier-transformed functions are periodic in the reciprocal lattice, with periodic boundary conditions as $f({\bf\tilde{n}} + {\hat \imath} N) =  f({\bf\tilde{n}})$, with ${ \hat \imath}$ analogous unit vectors as before but defined now in the reciprocal lattice.

From the above discussion, it follows that we can only represent momenta down to a minimum infrared (IR) cut-off
```math
\begin{align}
k_{\rm IR} = \frac{2\pi}{L} = \frac{2\pi}{N\delta x} ,
\end{align}
```

and hence $\tilde{\bf n}$ labels the continuum momentum values ${\bf k} = (\tilde n_1, \tilde n_2, \tilde n_3)  k_{\rm IR}$. There is also a maximum ultraviolet (UV) momentum
that we can capture within each spatial dimension,
```math
\begin{align}
k_{i,\rm UV} = {N\over2}k_{\rm IR} = {\pi\over \delta x} .
\end{align}
```

The maximum momentum we can capture in a 3-dimensional reciprocal lattice is therefore the diagonal of the box,
```math
\begin{align}
k_{\rm max} = \sqrt{k_{1,\rm UV}^2+k_{2,\rm UV}^2+k_{3,\rm UV}^2} = {\sqrt{3}\over2}Nk_{\rm IR} = \sqrt{3}{\pi\over \delta x}  .
\end{align}
```

We note that for given $N$, fixing $k_{\rm IR}$ automatically determines $L$. Fixing $k_{\rm IR}$ can be very useful if one has an *a priori* understanding  of the typical momenta scales expected to be excited in the scenario to be simulated.

Finally, we also note that a time-step $\delta \eta$ must be chosen in order to run any simulation. As a rule of thumb, stability of the solution typically requires $\delta \eta / \delta x < 1 / \sqrt{d}$. Continuum derivatives, either spatial or temporal, need to be replaced on the lattice with different finite expressions that have a correct continuum limit, i.e. approximations to the continuum derivative to some order on the lattice spacing/time step. Simple definitions of a lattice derivative are the *neutral* derivative
[](){ #eq_neutrald }
```math
\tag{17}
[\nabla^{(0)}_\mu f] = \frac{f({n}+\hat\mu) - f({n}-\hat\mu)}{2\delta x ^\mu}  \longrightarrow   \partial_i{\tt f}({x})\big|_{{x} \equiv  {\bf n}\delta x+n_0\delta\eta} + \mathcal{O}(\delta x_\mu^2) ,
```

with $\delta x^\mu$ referring to the lattice spacing $\delta x$ in the case of spatial derivatives, and to the time step $\delta\eta$  in the case of temporal derivatives. The expression to the right-hand side of the arrow indicates where and to what order on the lattice spacing/time step the continuum limit is recovered. Also standard are the *forward* and *backward* derivatives
[](){ #eq_forwardbackwardd }
```math
\begin{align}
\tag{18}
[\nabla^\pm_\mu f] = \frac{\pm f({n}\pm \hat\mu) \mp f({n})}{\delta x^\mu}  \longrightarrow   \left\lbrace\begin{array}{l}
\partial_i{\tt f}({x})\big|_{{x} \equiv  {\bf n}\delta x+n_0\delta\eta} + \mathcal{O}(\delta x_\mu)  \\
\partial_i{\tt f}({x})\big|_{{x} \equiv  ({n} \pm \hat\mu/2)\delta x^\mu} + \mathcal{O}(\delta x_\mu^2)
\end{array}\right. ,
\end{align}
```

which recover the continuum limit to linear or to quadratic order on the lattice spacing/time step, depending on whether we interpret that they live in ${n}$, or in between the two lattice sites involved ${n} \pm \hat\mu/2$. This shows that in order to recover a continuum differential operation on the lattice, not only it is important to use a suitable discrete operator, but also to determine where it 'lives'. To improve accuracy, one can also consider lattice derivatives which involve more points, typically leading to definitions that have a symmetry either around a lattice site or around half-way between lattice sites, see e.g. [@Frolov_2008hy].

Depending on the choice of lattice operator $\nabla_{i}$ for the spatial derivatives, the discrete Fourier transform leads to different *lattice momenta*. The Fourier transform of a derivative $[\nabla_if]$ can be written as [@Figueroa_2020rrl]
```math
\begin{align}
{\nabla_i f}(\tilde{\bf n}) \equiv -i{\bf k}_{\rm L}(\tilde{\bf n}) f(\tilde{\bf n}) ,
\end{align}
```

which leads, for the neutral derivative ([*17*][eq_neutrald]), to
[](){ #eq_neutralMomentum }
```math
\begin{align}
\tag{19}
k_{{\rm Lat},i}^0=\frac{\sin(2\pi \tilde{n}_i/N)}{\delta x} ,
\end{align}
```

and for forward/backward derivatives ([*18*][eq_forwardbackwardd]) to
[](){ #eq_ForwardBackwardMomentum }
[](){ #eq_ForwardBackwardMomentumII }
```math
\begin{align}
\tag{20}
& k_{{\rm Lat},i}^+ = k_{{\rm Lat},i}^- = 2\frac{\sin(\pi \tilde{n}_i/N)}{\delta x} , {\rm if}  {\bf l} = {\bf n} \pm {\hat{\imath}\over2} ,\\
\tag{21}
& k_{{\rm Lat},i}^\pm = \frac{\sin(2\pi \tilde{n}_i/N)}{\delta x} \pm i
\frac{1-\cos(2\pi \tilde{n}_i/N)}{\delta x} , {\rm if}  {\bf l} = {\bf n} .
\end{align}
```

Finally, in order to mimic the *power spectrum* of a continuum function ${\tt f}({\bf x})$, which by definition characterizes its ensemble average $\langle {\tt f}^2 \rangle$ as
[](){ #eq_continuumPSII }
[](){ #eq_continuumPS }
```math
\begin{align}
		&\langle {\tt f}^2 \rangle = \int d\log k \Delta_{\tt f}(k) ,\tag{22}\\
		&\Delta_{\tt f}(k) \equiv {k^3\over 2\pi^2}\mathcal{P}_{\tt f}(k) ,  \langle {\tt f}_{\bf k} {\tt f}_{{\bf k}^{\prime}} \rangle = (2\pi)^3 \mathcal{P}_{\tt f}(k) \delta (\mathbf{k}-\mathbf{k^{\prime}}) , \tag{23}
\end{align}
```

we define on the lattice the expression of a discrete power spectrum as
	[](){ #eq_discretePS }
```math
		 \Delta_f(k) \equiv \frac{k^3(\tilde {\bf n})}{2\pi^2}\left(\frac{\delta x}{N}\right)^3 \big\langle \big|f(\tilde{\bf n})\big|^2\big\rangle_{R(\tilde{\bf n})} , \tag{24}
	
```

where $\langle ( ... ) \rangle_{R(\tilde{\bf n})} \equiv \frac{1}{4\pi|\tilde{\bf n}|^2}\sum_{\tilde{\bf n}^{\prime}\in R(\tilde{\bf n})}( ... )$ is an angular average over a spherical shell of radius $\tilde{\bf n}^{\prime}\in \big[|\tilde{\bf n}|,|\tilde{\bf n}+ \Delta\tilde{\bf n}|\big)$, with $\Delta \tilde{\bf n}$ a given radial binning. In this way, we obtain
[](){ #eq_discretePSII }
```math
\begin{align}
\langle f^2\rangle_V \equiv {1\over N^3}\sum_{\bf n}f^2({\bf n}) = \sum_{\tilde{\bf n} \neq 0}{k_{\rm IR}\over k(\tilde {\bf n})} \Delta_f(k(\tilde {\bf n})) ,\tag{25}
\end{align}
```

with $\langle ( ... ) \rangle_{V}$ representing now a volume average over the lattice, equivalent to the $lhs$ of the continuum expression Eq. ([*22*][eq_continuumPSII]), whilst the $rhs$ of Eq. ([*25*][eq_discretePSII]) mimics exactly the $rhs$ of  Eq. ([*22*][eq_continuumPSII]).

!!! note
    **Important Note:** The definition of the scalar power spectrum ${\Delta}_{\phi} (k)$ on the lattice has evolved since CosmoLattice `version 1.0` was first released. From `version 1.1` and above, different versions of a scalar field power spectrum can be output, see  [`Technical Note I`](../tech_notes/Power%20spectra.md) for an explanation of the different choices. Visit regularly [https://cosmolattice.net/technicalnotes/](https://cosmolattice.net/technicalnotes/) to check for new features (ranging from new definitions, options, algorithms, interactions, etc) incorporated in successive updated versions of CosmoLattice.

### Lattice gauge invariant techniques { #subsec_LGT }

Discretizing a gauge theory requires a special care in order to preserve gauge invariance at the lattice level. It is not enough to recover gauge invariance in the continuum, i.e. in the limit of zero lattice spacing/time step, as gauge invariance is meant to remove spurious transverse degrees of freedom.  If we were to discretize a gauge theory by substituting all ordinary derivatives in the continuum EOM by finite differences like those in Eqs. ([*17*][eq_neutrald]), ([*18*][eq_forwardbackwardd]), the gauge symmetry would not be preserved and the spurious degrees of freedom would propagate on the lattice. Lattice gauge invariant techniques are meant to avoid this type of trouble.

In order to build an action or EOM for any gauge theory that preserves a discretized version of the gauge symmetry, it is customary to define *link* variables as\footnote{$P\exp\lbrace...\rbrace$ means *path-ordered* along the integration trajectory, as the construction of links is based on the definition of a *parallel transporter*, connecting two points in space-time as $U(x,y) = P\exp\left\lbrace-ie\int_{x}^{y}dx^{\mu}A_\mu  \right\rbrace$.}
```math
\begin{align*}
U_{0,n} &\equiv P\exp\left\lbrace-ie\int_{x(n)}^{x(n+\hat0)}dt'A_0 \right\rbrace \approx  e^{-ie\delta t A_0} ,
\\
U_{i,n} &\equiv P\exp\left\lbrace-ie\int_{x(n)}^{x(n+\hat\imath)} dxA_i \right\rbrace \approx e^{-ie\delta x A_i} ,
\end{align*}
\tag{26}
```

where the gauge field $A_\mu$, and hence the link $U_\mu$, is considered to live in the point $n + {\hat\mu\over2}$. We also define $U_{-\mu,n} = U_{\mu,n-\mu}^\dagger \equiv U_\mu^\dagger(n-{1\over 2}\hat\mu)$. In the continuum limit, the gauge fields can be recovered simply from $-i(\mathcal{I}- U_{\mu,n})/(e\delta x^\mu) \longrightarrow  A_\mu\big(n+{1\over2}\hat\mu\big) + \mathcal{O}(\delta x^\mu)$.

!!! note
    ***Important to know:*** To simplify the notation on the lattice, a scalar field living in a generic lattice site $n = (n_o,{\bf n}) = (n_o,n_1,n_2,n_3)$, i.e. $\phi_n = \phi(n)$, will be simply denoted as $\phi$. If the point is displaced in the $\mu-$direction by one unit lattice spacing/time step, $n + \hat\mu$, we use the notation $n+\mu$ or simply by $+\mu$ to indicate this, so that the field amplitude in the new point is expressed as $\phi_{+\mu} \equiv \phi(n+\hat\mu)$. In the case of gauge fields, whenever represented explicitly on the lattice, we will automatically understand that they live in the middle of lattice points, i.e. $A_{\mu} \equiv A_{\mu}(n+{1\over2}\hat\mu)$. It follows then that e.g. $A_{\mu,+\nu} \equiv A_{\mu}\big(n + {1\over2}\hat\mu +  \hat\nu\big)$. In the case of links, we will use the notation $U_\mu \equiv U_{\mu,n} \equiv U_\mu(n+{1\over2}\hat\mu)$, and hence $U_{\mu,\pm\nu} = U_{\mu,n\pm\nu} \equiv U_\mu(n + {1\over2}\hat\mu \pm \hat\nu)$.

One can actually build an action or EOM for any gauge theory, preserving a discretized version of the gauge symmetry, using only link variables and no gauge fields. That is known as the *compact formulation* of lattice gauge theories, and this can be applied to both Abelian and non-Abelian gauge theories. In the case of non-Abelian theories, compact formulations are actually the only way to discretize them while respecting gauge invariance on the lattice. For Abelian gauge theories, however, it is still possible to make use of an explicit representation of the  gauge fields, in the so called *non-compact formulation*. Below we provide both. We introduce standard definitions for $links$, $plaquettes$ and *lattice covariant derivatives*, specialized to both Abelian and non-Abelian gauge groups, setting back $e=g_A Q_A$. We provide also basic definitions, together with useful approximations and expressions (in the case of Abelian theories for both compact and non-compact formulations). All these ingredients, summarized in the $U(1)$ and $SU(2)$ toolkits below, represent all one needs to know in order to discretize gauge theories while preserving the gauge invariance at the lattice level.

!!! note
    **U(1) toolkit**
    _____
    [](){ #eq_U1toolkit }
    
    ```math
    
    \begin{align*}
    &{\rm Links:}  V_{\mu} \equiv e^{-i g_AQ_A \delta x_{\mu} A_{\mu}} = \cos(g_AQ_A\delta x_{\mu} A_{\mu}) - i \sin (g_AQ_A\delta x_{\mu} A_{\mu}) ;  V_{- \mu} \equiv V_{\mu,-\mu}^* ;  V_{\mu}^* V_{\mu} = 1 ;\\
    &{\rm Plaquettes}:  V_{\mu \nu} \equiv V_{\mu} V_{\mu,+\mu} V_{\mu, +\nu}^* V_\nu^* \simeq e^{-i g_AQ_A\delta x_{\mu} \delta x_{\nu} [ F_{\mu \nu} + \mathcal{O}(\delta x)] };  V_{\mu\nu}^* = V_{\nu\mu} ;
    \\
    &{\rm Covariant Derivs.}:  (D_{\mu}^\pm\varphi)({\bf l}) = \pm{1\over \delta x^\mu}(V_{\pm\mu}\varphi_{\pm\mu} - \varphi) , {\bf l} = {\bf n} \pm {1\over2}{\hat\mu}
    \\
    &{\rm Expansions}:
    \left\lbrace
    \begin{array}{rcl}
    (D_{\mu}^\pm\varphi)({\bf l})  & \longrightarrow & (D_{\mu}\varphi)({\bf l}) + \mathcal{O}(\delta x^2)  {\bf l} = {\bf n} \pm {1\over2}{\hat\mu}\\
    \mathcal{R}e\lbrace V_{\mu \nu} \rbrace  & \longrightarrow & 1 - \frac{1}{2} \delta x_{\mu}^2 \delta x_{\nu}^2g_A^2Q_A^2 F_{\mu \nu}^2 + \mathcal{O}(\delta x^5) , {\bf l} = {\bf n} + {1\over2}{\hat\mu} + {1\over2}{\hat\nu}\\ \mathcal{I}m\lbrace V_{\mu \nu} \rbrace & \longrightarrow & - \delta x_{\mu} \delta x_{\nu} g_AQ_AF_{\mu \nu} + \mathcal{O}(\delta x^3) , {\bf l} = {\bf n} + {1\over2}{\hat\mu} + {1\over2}{\hat\nu}
    \end{array}\right.
    \\
    &{\rm Expressions}:
    \left\lbrace
    \begin{array}{l}
    \left.
    \begin{array}{l}
    \sum_n {1\over 4}F_{\mu \nu}^2 \cong -{1\over 2}\sum_n{\mathcal{R}e\lbrace V_{\mu \nu} \rbrace \over \delta x_{\mu}^2 \delta x_{\nu}^2g_A^2Q_A^2} = -{1\over4}\sum_n {(V_{\mu \nu}+V_{\mu \nu}^*)\over \delta x_{\mu}^2 \delta x_{\nu}^2g_A^2Q_A^2} + \mathcal{O}(\delta x^2)\\
    \sum_n {1\over4}F_{\mu \nu}^2 \simeq \sum_n {1\over4}{\mathcal{I}m^2\lbrace V_{\mu \nu} \rbrace \over \delta x_{\mu}^2 \delta x_{\nu}^2g_A^2Q_A^2} = -\sum_n {1\over4}{(V_{\mu \nu}-V_{\mu \nu}^*)^2\over \delta x_{\mu}^2 \delta x_{\nu}^2g_A^2Q_A^2} + \mathcal{O}(\delta x^2)
    \end{array}\right] ({\tt Compact})\\
    \left.
    \begin{array}{l}
    \sum_n {1\over4}F_{\mu \nu}^2 \simeq {1\over4}\sum_n (\Delta^+_\mu A_\nu - \Delta^+_\nu A_\mu)^2 + \mathcal{O}(\delta x^2)
    \end{array}\right] ({\tt Non-Compact})
    \end{array}\right.
    \\
    &{\rm Gauge Trans}
    \left.
    \left\lbrace
    \begin{array}{cll}
    \phi  & \longrightarrow & e^{+ig_AQ_A\alpha}\phi\\
    A_\mu & \longrightarrow & A_\mu - \Delta_\mu^+\alpha\\
    V_{\pm \mu}  & \longrightarrow & V_{\pm \mu}e^{ig_AQ_A(\alpha_{\pm\mu}-\alpha)}
    \end{array}\right.
    \right]  \Longrightarrow   \left\lbrace
    \begin{array}{cll}
    D_\mu^\pm\phi & \longrightarrow & e^{ig_AQ_A\alpha}(D_\mu^\pm\phi)\\
    V_{\mu\nu}  & \longrightarrow & V_{\mu\nu}  {\rm (gauge inv. !)}
    \end{array}\right.
    
    \tag{27}
    \end{align*}
    
    ```

!!! note
    **SU(2) toolkit**
    _____
    [](){ #eq_SU2toolkit }
    [](){ #eq_SUNtoolkit }
    
    ```math
    
    \begin{align*}
    &{\rm Links}:  U_{\mu} \equiv e^{-i g_B Q_B \delta x B_\mu} = e^{-i g_B Q_B \delta x B_{\mu}^a T_a} ;  U_{- \mu} \equiv U_{\mu,-\mu}^{\dagger} ;  U_{\mu}^{\dagger} U_{\mu} = \mathcal{I} \\
    &{\rm Plaquettes}:  U_{\mu \nu} \equiv U_{\mu} U_{\nu,+ \mu} U_{\mu, +\nu}^{\dagger} U_{\nu}^{\dagger} \simeq e^{-ig_B Q_B \delta x_{\mu} \delta x_{\nu} [ G_{\mu \nu}^a T_a + \mathcal{O} (\delta x_{\mu} ) ] } ;   U_{\mu \nu}^\dagger = U_{\nu\mu} \\
    &{\rm Covariant Derivs.}:  (D_{\mu}^\pm\Phi)({\bf l}) = \pm{1\over \delta x^\mu}(U_{\pm\mu}\Phi_{\pm\mu} - \Phi)  \longrightarrow  (D_{\mu}\Phi)({\bf l}) + \mathcal{O}(\delta x^2), {\bf l} = {\bf n} \pm {1\over2}{\hat\mu}
    \\
    &{\rm Expansions}:
    \left\lbrace
    \begin{array}{ccl}
    (D_{\mu}^\pm\Phi)({\bf l}) & \longrightarrow & (D_{\mu}\Phi)({\bf l}) + \mathcal{O}(\delta x^2) , {\bf l} = {\bf n} \pm {1\over2}{\hat\mu}\\
    (U_{\mu \nu} - U_{\mu \nu}^\dagger ) & \longrightarrow & -2ig_B Q_B\delta x_{\mu} \delta x_{\nu}G_{\mu \nu} + \mathcal{O} (\delta x_{\mu}^3) , {\bf l} = {\bf n} + {1\over2}{\hat\mu} + {1\over2}{\hat\nu} \\ {\rm Tr} [ U_{\mu \nu} ] & \longrightarrow & 2 - \frac{\delta x_{\mu}^2 \delta x_{\nu}^2g_B^2 Q_B^2}{4}\sum_a (G_{\mu \nu}^a)^2 + \mathcal{O} (\delta x_{\mu}^5) , {\bf l} = {\bf n} + {1\over2}{\hat\mu} + {1\over2}{\hat\nu}
    \end{array}
    \right.
    \\
    &{\rm Expressions}:
    \left\lbrace
    \begin{array}{l}
    {1\over2}{\rm Tr}[G_{\mu\nu}G^{\mu\nu}] = {1\over4}\sum_a (G_{\mu\nu}^a)^2 \cong -{{\rm Tr} [ U_{\mu \nu} ]\over \delta x_{\mu}^2 \delta x_{\nu}^2 g_B^2 Q_B^2} + \mathcal{O}(\delta x^2) , \\
    G_{\mu \nu} = G_{\mu \nu}^aT_a \simeq \frac{i}{2\delta x_{\mu} \delta x_{\nu}g_B Q_B} (U_{\mu \nu} - U_{\mu \nu}^\dagger) + \mathcal{O}(\delta x^2)  ,\\
    G_{\mu \nu}^a \simeq \frac{1}{\delta x_{\mu} \delta x_{\nu}g_B Q_B} {\rm Tr} [ (i T_a)  (U_{\mu \nu} - U_{\mu \nu}^\dagger ) ] + \mathcal{O}(\delta x^2)
    \end{array}\right.
    \\
    &{\rm Gauge Trans.}
    \left.
    \left\lbrace
    \begin{array}{cll}
    \Phi  & \longrightarrow & \Omega \Phi ,  \Omega \equiv e^{+ig_B Q_B\alpha_aT_a}\\
    U_{\pm \mu}  & \longrightarrow & \Omega  U_{\pm \mu} \Omega^\dagger_{\pm \mu}
    \end{array}\right.
    \right]  \Longrightarrow   \left\lbrace
    \begin{array}{cll}
    D_\mu^\pm\Phi & \longrightarrow & \Omega (D_\mu^\pm\Phi)\\
    U_{\mu\nu}  & \longrightarrow & \Omega  U_{\mu\nu} \Omega^\dagger
    \\
    {\rm Tr}\lbrace U_{\mu\nu} \rbrace  & \longrightarrow & {\rm Tr}\lbrace U_{\mu\nu} \rbrace
    \end{array}\right.
    \tag{28}
    \end{align*}
    
    ```

