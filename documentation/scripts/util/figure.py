import sys
import subprocess
import os
import shutil
import regex as re

figure_counter = 1


def figure_block(block_content, tmpdir):
    ################################################
    # Process figure blocks
    ################################################

    global figure_counter
    print(f"Processing figure block {figure_counter}...", file=sys.stderr)

    # We output the figure block to ./tmp/tmp.tex
    with open(tmpdir + "/tex/tmp.tex", "w", encoding="utf-8") as outfile:
        outfile.write(r"\setcounter{figure}{" + f"{figure_counter-1}" + "}")
        outfile.write(block_content)

    # Then, use pdflatex
    subprocess.run(
        [
            "pdflatex",
            "-interaction=nonstopmode",
            "figure.tex",
        ],
        stdout=subprocess.DEVNULL,
        # stderr=subprocess.DEVNULL,
        cwd=tmpdir + "/tex/",
    )

    # using imagemagick to convert pdf to png
    subprocess.run(
        [
            "magick",
            "-density",
            "300",
            "./figure.pdf",
            "-quality",
            "90",
            "-trim",
            "./figure.png",
        ],
        stdout=subprocess.DEVNULL,
        # stderr=subprocess.DEVNULL,
        cwd=tmpdir + "/tex/",
    )

    # and remove the pdf
    os.remove(tmpdir + "/tex/figure.pdf")

    # make a directory /assets/figures if it doesn't exist
    if not os.path.exists(tmpdir + "/assets/figures"):
        os.makedirs(tmpdir + "/assets/figures")
    # Move the generated PDF to /assets/figures/figureN.pdf
    output_filename = f"assets/figures/figure{figure_counter}.png"
    shutil.move(tmpdir + "/tex/figure.png", tmpdir + "/" + output_filename)

    # extract label if any
    label_dict = {}
    label_name = re.search(r"\\label\{(.+?)\}", block_content)
    if label_name:
        label_name = label_name.group(1)
        label_dict[label_name] = f"{figure_counter}"

        block_content = (
            "\n[](){ #"
            + f"{label_name.replace(':', '_')}"
            + " }\n"
            + f"![Figure {figure_counter}]({output_filename})\n"
        )
    else:
        block_content = f"\n![Figure {figure_counter}]({output_filename})\n"

    figure_counter += 1
    # Return the markdown to include the figure
    return block_content, label_dict
