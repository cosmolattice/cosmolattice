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
