import regex as re

math_block_pass_1 = {
    # remove any hspace around a =
    r"& *?\\hspace\{.*?\} *?= *?\\hspace\{.*?\} *?&": r"&=",
    r"& *?\\hspace\{.*?\} *?\\equiv *?\\hspace\{.*?\} *?&": r"&=",
    r"& *?\\hspace\{.*?\} *?\\approx *?\\hspace\{.*?\} *?&": r"&=",
    r"& *?= *?&": r"&=",
    r"& *?\\equiv *?&": r"&=",
    r"& *?\\approx *?&": r"&=",
}


def katex(lines):
    ################################################
    # We need to replace $...$ with $`...`$ to avoid conflicts with mkdocs katex.
    # Same thing for $$...$$ with ```math ... ```
    ################################################

    # print("    Processing katex...")

    # return lines

    block_math_pattern = re.compile(r"\$\$(.+?)\$\$", re.DOTALL)
    lines = block_math_pattern.sub(r"\n```math\n\1```\n", lines)
    inline_math_pattern = re.compile(r"\$(.+?)\$")
    lines = inline_math_pattern.sub(r"$`\1`$", lines)

    for old, new in math_block_pass_1.items():
        lines = re.sub(old, new, lines, flags=re.MULTILINE)

    return lines
