Unless otherwise specified, we use the following conventions throughout the document:

-  We use natural units $c=\hbar=1$ and metric signature $(-1,+1,+1,+1)$.

-  We use interchangeably the Newton constant $G$, the full Planck mass $M_p \simeq 1.22\cdot 10^{19}$ GeV, and the reduced Planck mass $m_p \simeq 2.44\cdot 10^{18}$ GeV, all related through $M_p^2 = 8\pi m_p^2 = 1/G$.

-  Latin indices $i, j, k, ... = 1,2,3$ are reserved for spatial dimensions, and Greek indices $\alpha, \beta, \mu, \nu,... = 0,1,2,3$ for space-time dimensions. We use the *Einstein convention* of summing over repeated indices *only in the continuum*. However, `on the lattice, unless stated otherwise, repeated indices do not represent summation`.

-  We consider a flat FLRW metric $ds^2 = -a^{2\alpha}(\eta)d\eta^2 + a^2(\eta)   \delta_{ij}   dx^i dx^j$ with $\alpha \in \mathbb{R}$ a constant chosen conveniently in each scenario. For $\alpha = 0$, $\eta$ denotes the *coordinate time* $t$, whereas for $\alpha = 1$, $\eta$ denotes the *conformal time* $\tau = \int {dt'\over a(t')}$. For arbitrary $\alpha$, we will refer to the time variable as the *$\alpha$-time*.

-  We reserve the notation $()^{\cdot}$ for derivatives with respect to cosmic time with $\alpha = 0$, and $()'$ for derivatives with respect to $\alpha$-time with arbitrary $\alpha$.

-  Physical momenta are represented by ${\bf p}$, comoving momenta by ${\bf k}$, the $\alpha$-time Hubble rate is given by $\mathcal{H} = a'/a$, whereas the physical Hubble rate is denoted by $H = \mathcal{H}|_{\alpha = 0}$.

<!-- -  Cosmological parameters are fixed to the CMB values given in [@Aghanim_2018eyx;@Akrami_2018odb].-->

-  Our Fourier transform convention in the continuum is given by
[](){ #eq_FTcont }
```math
\begin{align}\tag{1}
f({\bf x}) = \frac{1}{(2 \pi)^3} \int d^3 {\bf k}   f({\bf k})   e^{+i {\bf k} {\bf x}}    \Longleftrightarrow    f({\bf k}) = \int d^3 {\bf x}   f ( {\bf x})   e^{-i {\bf k} {\bf x}} .
\end{align}
```

-  Our discrete Fourier transform (DFT) is defined by
[](){ #eq_FTdiscreteAux }
```math
\begin{align}\tag{2}
f({\bf n}) \equiv {1\over N^3}\sum_{\tilde n} e^{+i{2\pi\over N} {\bf \tilde n n}} f({\bf \tilde n})   \Leftrightarrow    f({\bf \tilde n}) \equiv \sum_{n} e^{-i{2\pi\over N} {\bf n \tilde n} }f({\bf n}) .
\end{align}
```

-  A scalar field living in a generic lattice site $n = (n_o,{\bf n}) = (n_o,n_1,n_2,n_3)$, i.e. $\phi_n = \phi(n)$, will be simply denoted as $\phi$. If the point is displaced in the $\mu-$direction by one unit lattice spacing/time step, $n + \hat\mu$, we will then use the notation $n+\mu$ or simply by $+\mu$ to indicate this, so that the field amplitude in the new point is expressed as $\phi_{+\mu} \equiv \phi(n+\hat\mu)$.

-  When representing gauge fields on the lattice, it is usually understood that they live in between lattice points, half step away from each lattice site, i.e.~$A_{\mu} \equiv A_{\mu}(n+{1\over2}\hat\mu)$. It follows then that e.g.~$A_{\mu,+\nu} \equiv A_{\mu}\big(n + {1\over2}\hat\mu +  \hat\nu\big)$. 
In the case of links, we will use the notation $U_\mu \equiv U_{\mu,n} \equiv U_\mu(n+{1\over2}\hat\mu)$, and hence $U_{\mu,\pm\nu} = U_{\mu,n\pm\nu} \equiv U_\mu(n + {1\over2}\hat\mu \pm \hat\nu)$. 
In the very specific case of gauge fields coupled to a fluid, however, it is possible to consider an alternative formulation where the gauge fields are \textit{collocated}, i.e.~living at integer lattice sites $A_\mu \equiv A_\mu(n)$.

-  Even though the *lattice spacing* $\delta x$ and the *time step* $\delta t$ do not need to be equal, we may often speak loosely of corrections of order $\mathcal{O}(\delta x)$, independently of whether we are referring to the lattice spacing or the time step. 

