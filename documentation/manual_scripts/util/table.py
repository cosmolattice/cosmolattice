import sys
import subprocess
import os
import shutil
import regex as re

table_counter = 1


def table_block(block_content, tmpdir):
    ################################################
    # Process table blocks
    ################################################

    global table_counter
    print(f"Processing table block {table_counter}...", file=sys.stderr)

    # We output the table block to ./tmp/tmp.tex
    with open(tmpdir + "/tex/tmp.tex", "w", encoding="utf-8") as outfile:
        outfile.write(r"\setcounter{table}{" + f"{table_counter-1}" + "}")
        outfile.write(block_content)

    print("we are in the folder ", os.getcwd(), file=sys.stderr)

    # Then, use pdflatex
    subprocess.run(
        [
            "pdflatex",
            "-interaction=nonstopmode",
            "table.tex",
        ],
        stdout=subprocess.DEVNULL,
        # stderr=subprocess.DEVNULL,
        cwd=tmpdir + "/tex/",
    )

    # using imagemagick to convert pdf to png
    subprocess.run(
        [
            "convert",
            "-density",
            "300",
            "-quality",
            "90",
            "-trim",
            "table.pdf",
            "table.png",
        ],
        stdout=subprocess.DEVNULL,
        # stderr=subprocess.DEVNULL,
        cwd=tmpdir + "/tex/",
    )

    # and remove the pdf
    os.remove(tmpdir + "/tex/table.pdf")

    # make a directory /assets/tables if it doesn't exist
    if not os.path.exists(tmpdir + "/assets/tables"):
        os.makedirs(tmpdir + "/assets/tables")
    # Move the generated PDF to /assets/tables/tableN.png
    output_filename = f"/assets/tables/table{table_counter}.png"
    shutil.move(tmpdir + "/tex/table.png", tmpdir + output_filename)

    # extract label if any
    label_dict = {}
    label_name = re.search(r"\\label\{(.+?)\}", block_content)
    if label_name:

        label_name = label_name.group(1)
        label_dict[label_name] = f"{table_counter}"

        block_content = (
            "\n[](){ #"
            + f"{label_name.replace(':', '_')}"
            + " }\n"
            + f"![Table {table_counter}]({output_filename})\n"
        )
    else:
        block_content = f"\n![Table {table_counter}]({output_filename})\n"

    table_counter += 1

    return block_content, label_dict
