import sys
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
    r"\\begin\{equation\*?\}(\g<body>)\\end\{equation\*?\}": r"$$\n\1\n$$",
    r"\\begin\{align\*?\}(\g<body>)\\end\{align\*?\}": r"$$\n\\begin{malign}\1\\end{malign}\n$$",
    r"\\begin\{eqnarray\*?\}(\g<body>)\\end\{eqnarray\*?\}": r"$$\n\\begin{malign}\1\n\\end{malign}\n$$",
    r"\\begin\{alignat\*?\}{.}(\g<body>)\\end\{alignat\*?\}": r"$$\n\\begin{malign}\1\\end{malign}\n$$",
    ## code environments
    r"\\begin\{shell-sessioncode\}([\s\S]+?)\\end\{shell-sessioncode\}": r"```bash\1```",
    r"\\begin\{C\+\+code\}([\s\S]+?)\\end\{C\+\+code\}": r"```cpp\1```",
    ## minor formatting
    # r"\\\\": r"\n",
    r"&=&": r" = ",
    r"&": r" ",
    r"\\nonumber": r"",
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
    r"\\hspace\{.+?\}": r"",
    r"\\hspace\*\{.+?\}": r"",
    r"%.*": r"",
    r"\\begin\{itemize\}": r"",
    r"\\end\{itemize\}": r"",
    r"\\begin\{enumerate\}": r"",
    r"\\end\{enumerate\}": r"",
    r"\\item": r"- ",
    r"\\ ": r" ",
    # Code stuff
    r"\\codeline\[[\w]+?\]\{([0-9]+)\}": r"`\1`",
    # Text formatting
    r"\{\s*?\\it\s?\\bf\s?(\g<body>)\s*\}": r"***\1***",
    r"\{\s*?\\bf\s?\\it\s?(\g<body>)\s*\}": r"***\1***",
    r"\{\s*?\\it\s?(\g<body>)\s*\}": r"*\1*",
    r"\{\s*?\\bf\s?(\g<body>)\s*\}": r"**\1**",
    r"\{\s*?\\tt\s?(\g<body>)\s*\}": r"`\1`",
    r"\\textit\{\s*(\g<body>)\s*\}": r"*\1*",
    r"\\textbf\{\s*(\g<body>)\s*\}": r"**\1**",
    r"\\texttt\{\s*(\g<body>)\s*\}": r"`\1`",
    r"\\emph\{\s*(\g<body>)\s*\}": r"*\1*",
    # Erroneous spaces in math mode
    r"\s\$\s(\g<body>)\$": r" $\2$",
    # mdframed
    r"\\begin\{mdframed\}\n\$\$(\g<body>)\$\$": r"$$\n\1\n$$\n\\begin{mdframed}",
    r"\\begin\{mdframed\}\n\\end\{mdframed\}": r"\n\n",
}
repl_pass_2 = {
    # mdframed
    r"\\begin\{mdframed\}\n\n": r"\\begin{mdframed}\n",
    r"\\begin\{mdframed\}\n\\end\{mdframed\}": r"\n\n",
    r"\\begin\{mdframed\}\n\$\$(\g<body>)\$\$": r"$$\1$$\n\\begin{mdframed}",
    r"\\begin\{mdframed\}\n(.*)\n": r"> \1 \n\\begin{mdframed}\n",
    r"\n>\n": r"\n",
    # Hyperlinks
    r"\\href\{(.+?)\}\{(.+?)\}": r"[\2](\1)",
}

shortcut_replacements = {
    r"\\dx": r"\\delta x",
    r"\\dt": r"\\delta t",
    r"\\deta": r"\\delta\\eta",
    r"\\bn": r"**n**",
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
    r"\\rh": r"{\\rm rh}",
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
# bpatt = bpatt.replace(r"\g<body>", bpatt)

npass_1 = repl_pass_1.copy()
for fr, to in repl_pass_1.items():
    nfr = fr.replace(r"\g<body>", bpatt)
    nto = to.replace(r"\g<body>", r"\1")
    npass_1[nfr] = nto
    if nfr != fr:
        del npass_1[fr]
repl_pass_1 = npass_1
npass_2 = repl_pass_2.copy()
for fr, to in repl_pass_2.items():
    nfr = fr.replace(r"\g<body>", bpatt)
    nto = to.replace(r"\g<body>", r"\1")
    npass_2[nfr] = nto
    if nfr != fr:
        del npass_2[fr]
repl_pass_2 = npass_2


def process_lines(lines):
    lines = "".join(lines)
    # print("    Applying common replacements...")
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

    ################################################
    # replace \inserttxtcode{filename}{startline}{endline} with the content of filename from startline to endline
    ################################################
    # print("    Inserting code snippets...")
    insert_pattern = re.compile(
        r"\\inserttxtcode\{(.+?)\}\{([0-9]+)\}\{([0-9]+)\}\{(.+?)\}"
    )
    for match in insert_pattern.finditer(lines):
        name = match.group(1)
        filename = match.group(4)
        startline = int(match.group(2))
        endline = int(match.group(3))
        try:
            with open(filename, "r", encoding="utf-8") as infile:
                file_lines = infile.readlines()
                code_snippet = "".join(file_lines[startline - 1 : endline])
                lines = lines.replace(match.group(0), "```\n" + code_snippet + "```")
        except Exception as e:
            print(f"Error reading {filename}: {e}")

    # Now split the lines into blocks of $$ ... $$ and everything else and put this into an ordered list
    blocks = []
    is_math_block = []
    current_block = []
    in_math_block = False
    for line in lines.split("\n"):
        if r"$$" in line:
            in_math_block = not in_math_block
            if not in_math_block:
                blocks.append("\n" + "\n".join(current_block) + "\n" + line + "\n")
                is_math_block.append(True)
                current_block = []
            else:
                blocks.append("\n" + "\n".join(current_block) + "\n")
                is_math_block.append(False)
                current_block = [line]
        else:
            current_block.append(line)
    if current_block:
        blocks.append("\n" + "\n".join(current_block) + "\n")
        is_math_block.append(in_math_block)
    for i in range(len(blocks)):
        if not is_math_block[i]:
            # get rid of \\
            blocks[i] = blocks[i].replace(r"\\", "\n")
    lines = "".join(blocks)

    ################################################
    # replace \insertcppcode{filename}{startline}{endline} with the content of filename from startline to endline
    ################################################
    insert_pattern = re.compile(
        r"\\insertcppcode\{(.+?)\}\{([0-9]+)\}\{([0-9]+)\}\{(.+?)\}"
    )
    for match in insert_pattern.finditer(lines):
        name = match.group(1)
        filename = match.group(4)
        startline = int(match.group(2))
        endline = int(match.group(3))
        try:
            with open(filename, "r", encoding="utf-8") as infile:
                file_lines = infile.readlines()
                code_snippet = "".join(file_lines[startline - 1 : endline])
                lines = lines.replace(match.group(0), "```cpp\n" + code_snippet + "```")
        except Exception as e:
            print(f"Error reading {filename}: {e}")

    ################################################
    # Find instances of \label{labelname} inside ```math...``` and wrap the entire block with <a name="labelname"></a>
    ################################################
    # print("    Processing labels...")
    label_pattern = re.compile(r"```math\n(.*?)```", re.DOTALL)
    for match in label_pattern.finditer(lines):
        label_content = match.group(1)
        label_name = re.search(r"\\label\{(.+?)\}", label_content)
        if label_name:
            label_content = re.sub(r"\\label\{(.+?)\}", "", label_content)
            label_name = label_name.group(1)
            lines = lines.replace(
                match.group(0),
                f'<a name="{label_name}"></a>\n```math\n{label_content}```',
            )
    # now get rid of these damn labels
    label_cleanup_pattern = re.compile(r"\\label\{(.+?)\}")
    lines = label_cleanup_pattern.sub(r"", lines)

    return lines
    ################################################
    # We need to replace $...$ with $`...`$ to avoid conflicts with mkdocs katex.
    # Same thing for $$...$$ with ```math ... ```
    ################################################
    # print("    Processing math...")
    block_math_pattern = re.compile(r"\$\$(.+?)\$\$", re.DOTALL)
    lines = block_math_pattern.sub(r"\n```math\n\1```\n", lines)
    inline_math_pattern = re.compile(r"\$(.+?)\$")
    lines = inline_math_pattern.sub(r"$`\1`$", lines)

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

    return lines


# For testing purposes
if len(sys.argv) != 3:
    test_string = r"""
~~~~ Here we provide a brief description of the steps to download, compile and run a job with \CLns. As an example we load a model called {\tt lphi4}, with input parameters specified in the file {\tt lphi4.in} (details of this model and of its parameters can be found in Section~\ref{sec:MyFirstModelScalars}, here we just use these files as a demonstration example). An appropriate version of FFTW needs to be installed in order to handle the code's Fourier transforms, either for serial jobs run on a single processor, or for parallelized jobs run on multiple processors. For serial jobs, that is all it is needed. For parallelized jobs, in addition, the appropriate MPI compiler needs also to be installed. Optionally, if one wants to print two- or three-dimensional snapshots of a simulation, or simply to stop/restart a run at a certain time, the HDF5 library also needs to be installed. Also optionally, the simulations can be parallelized in more than one spatial dimension, to speed up jobs of long duration or to run massive simulations. This requires the library PFFT (parallel Fast Fourier transforms) \cite{Pi13} to be installed. For further details on the installation and use of the code and the libraries, please see Appendix~\ref{app:Installation}.\\

\medskip
"""
    print("Testing processing function...")
    print(process_lines(test_string))
    print("Usage: python split.py inputfile outputfolder")
    sys.exit(1)

input = sys.argv[1]
outputfolder = sys.argv[2]

section_sets = []

with open(input, "r", encoding="utf-8") as file:
    section_name = "preamble"
    lines = []
    label = None

    all_lines = file.readlines()

    for i, line in enumerate(all_lines):
        # if the line contains "\\section", start a new file
        if "\\section" in line or r"\begin{thebibliography" in line:
            if section_name != "preamble":
                print(f"---- Processing section: {section_name}")
                # first, write the previous section to a file
                lines = process_lines(lines)
                section_sets.append((section_name, lines, label))

            # reset lines for the new section
            if r"\begin{thebibliography" in line:
                section_name = "Bibliography"
                lines = []
            else:
                section_name = line.split("{")[1].split("}")[0].strip()
                for old, new in repl_pass_1.items():
                    section_name = re.sub(old, new, section_name, flags=re.MULTILINE)
                for old, new in repl_pass_2.items():
                    section_name = re.sub(old, new, section_name)
                for old, new in shortcut_replacements.items():
                    section_name = re.sub(old, new, section_name)
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
    bibliography = "\n".join(lines)
    bibliography = re.sub(r"\\bf", r"", bibliography)
    bibliography = re.sub(r"\\em", r"", bibliography)
    bibliography = re.sub(r"\\newblock", r"", bibliography)
    bibitem_names = re.findall(r"\\bibitem\{(.+?)\}", bibliography)
    # Replace \bibitem{label} with [@label]
    bibrules = {}
    bibrules_outer = {}
    for name in bibitem_names:
        # Our bibtex plugin seems to have issues with certain characters in citation keys.
        stripped_name = (
            name.strip()
            .replace(":", "_")
            .replace(".", "_")
            .replace("-", "_")
            .replace("/", "_")
        )
        bibrules[name] = stripped_name
        bibrules_outer[name + ","] = "@" + stripped_name + ";"
        bibrules_outer[name] = "@" + stripped_name
    bibrules_outer[r"\\cite\{\s*(.+?)\s*\}"] = r"[\1]"
    bibrules_outer[r"\[ @([^\]]+)\]"] = r"[@\1]"
    for old, new in bibrules.items():
        bibliography = re.sub(old, new, bibliography)
    with open(f"{outputfolder}/Bibliography.md", "w", encoding="utf-8") as bibfile:
        bibfile.write(bibliography)

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

        # print(f"Writing section: {section_name}")
        # first, write the previous section to a file
        with open(
            f"{outputfolder}/{section_name}.md", "w", encoding="utf-8"
        ) as outfile:
            outfile.write(lines)

    # Write the navigation file
    with open(f"{outputfolder}/../nav.yml", "w", encoding="utf-8") as navfile:
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
