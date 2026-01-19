# describes the different blocks that can be found in the documentation
# each block has a name and a pair of delimiters (start, end)
# the delimiters are used to find the blocks in the LaTeX source files
blocks = [
    ["math", (r"$$", r"$$")],
    ["figure", (r"\begin{figure}", r"\end{figure}")],
    ["table", (r"\begin{table}", r"\end{table}")],
    ["table", (r"\begin{tabularx}", r"\end{tabularx}")],
    ["table", (r"\begin{longtable}", r"\end{longtable}")],
    ["table", (r"\begin{tabular}", r"\end{tabular}")],
]


def get_blocks(lines):
    lines = lines.split("\n")
    found_blocks = []
    default_block = []
    i = 0
    while i < len(lines):
        line = lines[i]
        # check if line matches the start of any block
        any_block_found = False
        for block_name, (start_delim, end_delim) in blocks:
            if start_delim in line:
                # found the start of a block
                any_block_found = True
                block_lines = []
                # flush the default block if it has content
                if default_block:
                    found_blocks.append([None, "\n".join(default_block)])
                    default_block = []
                # collect lines until the end delimiter is found
                block_lines.append(lines[i])
                i += 1
                while i < len(lines):
                    block_lines.append(lines[i])
                    if end_delim in lines[i]:
                        break
                    i += 1
                found_blocks.append([block_name, "\n".join(block_lines)])
                break
        if not any_block_found:
            default_block.append(line)
        i += 1
    # flush any remaining default block
    if default_block:
        found_blocks.append([None, "\n".join(default_block)])
    return found_blocks
