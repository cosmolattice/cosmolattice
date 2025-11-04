import regex as re


def katex(lines):
    ################################################
    # We need to replace $...$ with $`...`$ to avoid conflicts with mkdocs katex.
    # Same thing for $$...$$ with ```math ... ```
    ################################################

    # print("    Processing katex...")

    return lines

    block_math_pattern = re.compile(r"\$\$(.+?)\$\$", re.DOTALL)
    lines = block_math_pattern.sub(r"\n```math\n\1```\n", lines)
    inline_math_pattern = re.compile(r"\$(.+?)\$")
    lines = inline_math_pattern.sub(r"$`\1`$", lines)

    return lines
