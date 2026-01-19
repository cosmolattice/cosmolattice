import regex as re


def build_bibliography(inputfile, outputfolder, lines):
    bibliography = lines
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
    with open(f"{outputfolder}/Bibliography.tex", "w", encoding="utf-8") as bibfile:
        bibfile.write(bibliography)

    return bibrules_outer
