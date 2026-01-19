from base import *

# For testing purposes

test_string = r"""
\section{U(1) toolkit}

\begin{mdframed}
\begingroup
\allowdisplaybreaks
\begin{center}
%\vspace*{0.5cm}
----- U(1) toolkit -----
\end{center}
%\vspace*{-0.5cm}
%\begin{empheq}[box=\fbox]{eqnarray}
\begin{eqnarray}
%\boxed{
&&{\rm Links:}~ V_{\mu} \equiv e^{-i g_AQ_A \dx_{\mu} A_{\mu}} = \cos(g_AQ_A\dx_{\mu} A_{\mu}) - i \sin (g_AQ_A\dx_{\mu} A_{\mu}) ;~~~ V_{- \mu} \equiv V_{\mu,-\mu}^* ;~~~ V_{\mu}^* V_{\mu} = 1\,;\vspace*{0.4cm}\nonumber\\
&&{\rm Plaquettes}:~ V_{\mu \nu} \equiv V_{\mu} V_{\mu,+\mu} V_{\mu, +\nu}^* V_\nu^* \simeq e^{-i g_AQ_A\dx_{\mu} \dx_{\nu} [ F_{\mu \nu} + \mathcal{O}(\dx)] };~~~ V_{\mu\nu}^* = V_{\nu\mu}\,;
\vspace*{0.4cm}\nonumber\\
&&{\rm Covariant~Derivs.}:  (D_{\mu}^\pm\varphi)({\bf l}) = \pm{1\over \dx^\mu}(V_{\pm\mu}\varphi_{\pm\mu} - \varphi)\,,~~{\bf l} = {\bf n} \pm {1\over2}{\hat\mu}\,
\vspace*{0.6cm}\nonumber\\
&&{\rm Expansions}:
\left\lbrace
\begin{array}{rcl}
(D_{\mu}^\pm\varphi)({\bf l})  & \longrightarrow & (D_{\mu}\varphi)({\bf l}) + \mathcal{O}(\dx^2)\,~~{\bf l} = {\bf n} \pm {1\over2}{\hat\mu}\vspace*{0.2cm}\\
\mathcal{R}e\lbrace V_{\mu \nu} \rbrace  & \longrightarrow & 1 - \frac{1}{2} \dx_{\mu}^2 \dx_{\nu}^2g_A^2Q_A^2 F_{\mu \nu}^2 + \mathcal{O}(\delta x^5)\,,~~{\bf l} = {\bf n} + {1\over2}{\hat\mu} + {1\over2}{\hat\nu}\vspace*{0.2cm}\\ \mathcal{I}m\lbrace V_{\mu \nu} \rbrace & \longrightarrow & - \dx_{\mu} \dx_{\nu} g_AQ_AF_{\mu \nu} + \mathcal{O}(\delta x^3)\,,~~{\bf l} = {\bf n} + {1\over2}{\hat\mu} + {1\over2}{\hat\nu}
\end{array}\right.
\vspace*{0.6cm}\\
&&{\rm Expressions}:
\left\lbrace
\begin{array}{l}
\left.
\begin{array}{l}
\sum_n {1\over 4}F_{\mu \nu}^2 \cong -{1\over 2}\sum_n{\mathcal{R}e\lbrace V_{\mu \nu} \rbrace \over \dx_{\mu}^2 \dx_{\nu}^2g_A^2Q_A^2} = -{1\over4}\sum_n {(V_{\mu \nu}+V_{\mu \nu}^*)\over \dx_{\mu}^2 \dx_{\nu}^2g_A^2Q_A^2} + \mathcal{O}(\dx^2)\vspace*{0.2cm}\\
\sum_n {1\over4}F_{\mu \nu}^2 \simeq \sum_n {1\over4}{\mathcal{I}m^2\lbrace V_{\mu \nu} \rbrace \over \dx_{\mu}^2 \dx_{\nu}^2g_A^2Q_A^2} = -\sum_n {1\over4}{(V_{\mu \nu}-V_{\mu \nu}^*)^2\over \dx_{\mu}^2 \dx_{\nu}^2g_A^2Q_A^2} + \mathcal{O}(\dx^2)
\end{array}\right]~~({\tt Compact})\vspace*{0.5cm}\\
\left.
\begin{array}{l}
\sum_n {1\over4}F_{\mu \nu}^2 \simeq {1\over4}\sum_n (\Delta^+_\mu A_\nu - \Delta^+_\nu A_\mu)^2 + \mathcal{O}(\dx^2)
\end{array}\right]~~({\tt Non-Compact})
\end{array}\right.
\vspace*{0.6cm}\nonumber\\
&&{\rm Gauge~Trans}
\left.
\left\lbrace
\begin{array}{cll}
\phi  & \longrightarrow & e^{+ig_AQ_A\alpha}\phi\vspace*{0.2cm}\\
A_\mu & \longrightarrow & A_\mu - \Delta_\mu^+\alpha\vspace*{0.2cm}\\
V_{\pm \mu}  & \longrightarrow & V_{\pm \mu}e^{ig_AQ_A(\alpha_{\pm\mu}-\alpha)}
\end{array}\right.
\right] ~~\Longrightarrow ~~ \left\lbrace
\begin{array}{cll}
D_\mu^\pm\phi & \longrightarrow & e^{ig_AQ_A\alpha}(D_\mu^\pm\phi)\vspace*{0.2cm}\\
V_{\mu\nu}  & \longrightarrow & V_{\mu\nu} ~{\rm (gauge~inv.~!)}
\end{array}\right.
\nonumber
%\end{empheq}
\label{eq:U1toolkit}
\end{eqnarray}
\endgroup
\\ \end{mdframed}
"""
print("Testing processing function...")
text, labels = process_lines(test_string)
print("Processed text:")
print(text)
print("Labels found:")
print(labels)
