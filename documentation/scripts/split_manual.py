from base import *

input = sys.argv[1]
outputfolder = sys.argv[2]
tmpdir = sys.argv[3]
outname = sys.argv[4]

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
        navfile.write(f"  - {outname}:\n")
        for section_name, lines, label in section_sets:
            navfile.write(
                '      - "'
                + section_name
                + '": "'
                + outname
                + "/"
                + section_name
                + '.md"\n'
            )

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
