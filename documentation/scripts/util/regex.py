import regex as re

repl_pass_1 = {
    r"\\CLns": r"CosmoLattice",
    r"\\CL": r"CosmoLattice",
    # Headings
    r"\\subsection\{(\g<body>)\}": r"### \1",
    r"\\subsubsection\{(\g<body>)\}": r"#### \1",
    # Whitespaces and new lines
    r" \n": r"\n",
    r"\n ": r"\n",
    r"\n\n\n": r"\n\n",
    r"\\newpage": r"\n\n",
    r"([^\\])\\\n": r"\1\n",
    r"~+": r" ",
    r"\\,": r" ",
    # Math
    ## shortcuts (why...)
    r"\\bea": r"\\begin{eqnarray}",
    r"\\eea": r"\\end{eqnarray}",
    r"\\be([^a-zA-Z]+?)": r"\\begin{equation}\1",
    r"\\ee": r"\\end{equation}",
    ## environments
    r"\\begin\{equation\*?\}(\g<body>)\\end\{equation\*?\}": r"\n$$\n\1\n$$\n",
    r"\\begin\{align\*?\}(\g<body>)\\end\{align\*?\}": r"\n$$\n\\begin{malign}\1\\end{malign}\n$$\n",
    r"\\begin\{eqnarray\*?\}(\g<body>)\\end\{eqnarray\*?\}": r"\n$$\n\\begin{malign}\1\n\\end{malign}\n$$\n",
    r"\\begin\{alignat\*?\}{.}(\g<body>)\\end\{alignat\*?\}": r"\n$$\n\\begin{malign}\1\\end{malign}\n$$\n",
    ## minor formatting
    # r"\\\\": r"\n",
    r"\\nonumber": r"",
    r"`": r"'",
    r"\\vv": r"\\vec",
    r"\\newline": r"\n",
    # Environments
    ## center
    r"\\begin\{center\}(\g<body>)\\end\{center\}": r"\1",
    # Commands for removal
    r"\\addcontentsline\{.+?\}\{.+?\}\{.+?\}": r"",
    r"\\allowdisplaybreaks": r"",
    r"\\begingroup": r"",
    r"\\endgroup": r"",
    r"\\noindent": r"",
    r"\\medskip": r"",
    r"\\begin\{multicols\}\{[0-9]\}": r"",
    r"\\end\{multicols\}": r"",
    r"\\small": r"",
    r"\\footnotesize": r"",
    r"\\color\{.+?\}": r"",
    r"\\vspace\{.+?\}": r"",
    r"\\vspace\*\{.+?\}": r"",
    # r"\\hspace\{.+?\}": r"",
    # r"\\hspace\*\{.+?\}": r"",
    # Comments
    r"^%.*\n": r"",
    r"([^\\])%[^\n]*": r"\1",
    # Lists
    r"\\begin\{itemize\}": r"",
    r"\\end\{itemize\}": r"",
    r"\\begin\{enumerate\}": r"",
    r"\\end\{enumerate\}": r"",
    r"\\item": r"- ",
    r"([^\\])\\ ": r"\1 ",
    # Erroneous spaces in math mode
    r"\s\$\s(\g<body>)\$": r" $\2$",
    # extra characters
    re.escape(r"a\^"): r"â",
    # very specific replacements
    r"In the lattice": r"On the lattice",
    r"in the lattice": r"on the lattice",
    re.escape("----- U(1) toolkit -----"): r"     **U(1) toolkit**\n_____\n",
    re.escape("----- SU(N) toolkit -----"): r"     **SU(2) toolkit**\n_____\n",
    r"\\appendix": r"",
}

repl_pass_2 = {
    # Hyperlinks
    # r"\\href\{(.+?)\}\{(.+?)\}": r"[\2](\1)",
}

shortcut_replacements = {
    r"\\dx": r"\\delta x",
    r"\\dt": r"\\delta t",
    r"\\deta": r"\\delta\\eta",
    r"\\bn": r"{\\bf n}",
    r"\\dd": r"\\text{d}",
    #
    r"\\nn": r"",
    r"\\lag": r"\\mathcal{L}",
    r"\\mn": r"\\mu\\nu",
    r"\\ab": r"\\alpha\\beta",
    #
    r"\\hI": r"",
    r"\\hVII": r"",
    r"\\hV": r"",
    r"\\Lin": r"U_{\\mu,n}",
    r"\\Pla": r"U_{\\mu\\nu,n}",
    r"\\hu": r"{\\mathcal H}",
    r"\\lqn\{(.+?)\}": r"\\lefteqn{\1}",
    r"\\lb([^r])": r"\\left\\lbrace\1",
    r"\\rb([^r])": r"\\right\\rbrace\1",
    # r"\\rh ": r"{\\rm rh}",
    #
    r"\\piSc": r"\\tilde\\pi_\\phi",
    r"\\piSinglpar": r"\\left(\\tilde\\pi_\\varphi\\right)",
    r"\\piSingl": r"\\tilde\\pi_\\varphi",
    r"\\piApar": r"\\left(\\tilde\\pi_A\\right)",
    r"\\piA": r"\\tilde\\pi_A",
    r"\\piDoublpar": r"\\left(\\widetilde\\pi_\\Phi\\right)",
    r"\\piDoubl": r"\\widetilde\\pi_\\Phi",
    r"\\piBpar": r"\\left(\\tilde\\pi_B\\right)",
    r"\\piB": r"\\tilde\\pi_B",
    #
    r"\\EKsutwo": r"E_K^B",
    r"\\EGsutwo": r"E_G^B",
    r"\\kersutwo": r"\\mathcal{K}_{B_i}",
    r"\\kersutwoComp": r"\\mathcal{K}_{B_i^a}",
    #
}

bpatt = r"""
(?<body>
  (?: [^\[\]{}]
    | (?<square_brackets>
        \[
        \g<body>
        \]
    )
    | (?<curly_braces>
        \{
        \g<body>
        \}
    )
  )+?
)"""
bpatt = r"""
(?<body>
  (?: [^{}]
    | (?<curly_braces>
        \{
        \g<body>
        \}
    )
  )+?
)
""".replace(
    " ", ""
).replace(
    "\n", ""
)
bpatt = bpatt.replace(r"\g<body>", bpatt)
bpatt = bpatt.replace(r"\g<body>", bpatt)


def insert_body(patt_dict: dict[str, str]) -> dict[str, str]:
    npass = patt_dict.copy()
    for fr, to in patt_dict.items():
        nfr = fr.replace(r"\g<body>", bpatt)
        nto = to.replace(r"\g<body>", r"\1")
        npass[nfr] = nto
        if nfr != fr:
            del npass[fr]
    return npass


repl_pass_1 = insert_body(repl_pass_1)
repl_pass_2 = insert_body(repl_pass_2)


def global_pass(lines):
    while True:
        old_lines = lines
        for old, new in repl_pass_1.items():
            lines = re.sub(old, new, lines, flags=re.MULTILINE)
        if lines == old_lines:
            break
    while True:
        old_lines = lines
        for old, new in repl_pass_2.items():
            lines = re.sub(old, new, lines, flags=re.MULTILINE)
        if lines == old_lines:
            break
    while True:
        old_lines = lines
        for old, new in shortcut_replacements.items():
            lines = re.sub(old, new, lines, flags=re.MULTILINE)
        if lines == old_lines:
            break
    lines = lines.replace("malign", "align")
    return lines


def final_pass(lines):
    # Find parts of the text that start and end with mdframed
    mdframed_pattern = r"\\begin\{mdframed\}(\g<body>)\\end\{mdframed\}".replace(
        r"\g<body>", bpatt
    )
    mdframed_pattern = re.compile(mdframed_pattern, re.DOTALL)

    for match in mdframed_pattern.finditer(lines):
        # extract content
        framed_content = match.group(1).strip()
        # indent everything by four spaces
        indented_content = "\n".join(
            ["    " + line for line in framed_content.split("\n")]
        )
        # prepend !!! note\n
        md_content = "!!! note\n" + indented_content + "\n"
        # replace in lines
        lines = lines.replace(match.group(0), md_content)
    return lines
