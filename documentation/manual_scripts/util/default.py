import regex as re
from util.katex import katex

from util.regex import insert_body

default_pass = {
    # Useless \\
    r"\\\\": r"\n",
    # Code stuff
    r"\\codeline\[[\w]+?\]\{([0-9]+)\}": r"`\1`",
    r"\\mintinline\{C\+\+\}\{(.*?)\}": r"`\1`",
    ## code environments
    r"\\begin\{shell-sessioncode\}([\s\S]+?)\\end\{shell-sessioncode\}": r"```bash\1```",
    r"\\begin\{C\+\+code\}([\s\S]+?)\\end\{C\+\+code\}": r"```cpp\1```",
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
}
default_pass = insert_body(default_pass)

clean_labels = {
    # remove formatting things
    r"\{\s*?\\it\s?\\bf\s?(\g<body>)\s*\}": r"\1",
    r"\{\s*?\\bf\s?\\it\s?(\g<body>)\s*\}": r"\1",
    r"\{\s*?\\it\s?(\g<body>)\s*\}": r"\1",
    r"\{\s*?\\bf\s?(\g<body>)\s*\}": r"\1",
    r"\{\s*?\\tt\s?(\g<body>)\s*\}": r"\1",
    r"\\textit\{\s*(\g<body>)\s*\}": r"\1",
    r"\\textbf\{\s*(\g<body>)\s*\}": r"\1",
    r"\\texttt\{\s*(\g<body>)\s*\}": r"\1",
    r"\\emph\{\s*(\g<body>)\s*\}": r"\1",
}
clean_labels = insert_body(clean_labels)


def insert_code(lines, tmpdir="./"):
    insert_pattern = re.compile(
        r"\\insertcppcode\{(.+?)\}\{([0-9]+)\}\{([0-9]+)\}\{(.+?)\}"
    )
    for match in insert_pattern.finditer(lines):
        name = match.group(1)
        filename = tmpdir + match.group(4)
        startline = int(match.group(2))
        endline = int(match.group(3))
        try:
            with open(filename, "r", encoding="utf-8") as infile:
                file_lines = infile.readlines()
                code_snippet = "".join(file_lines[startline - 1 : endline])
                lines = lines.replace(match.group(0), "```cpp\n" + code_snippet + "```")
        except Exception as e:
            print(f"Error reading {filename}: {e}")

    return lines


def replace_labels(lines):
    label_dict = {}
    # find subsections with labels
    label_pattern = re.compile(r"### (.+?)\s*?\n*?\s*?\\label\{(.+?)\}")
    for match in label_pattern.finditer(lines):
        section_name = match.group(1)
        label_name = match.group(2)
        repl = label_name.replace(":", "_")
        lines = lines.replace(
            match.group(0), f"### {section_name} " + "{" + f" #{repl} " + "}\n"
        )
        cleaned_section_name = section_name
        for old, new in clean_labels.items():
            cleaned_section_name = re.sub(
                old, new, cleaned_section_name, flags=re.MULTILINE
            )
        label_dict[label_name] = cleaned_section_name

    label_pattern = re.compile(r"#### (.+?)\s*?\n*?\s*?\\label\{(.+?)\}")
    for match in label_pattern.finditer(lines):
        section_name = match.group(1)
        label_name = match.group(2)
        repl = label_name.replace(":", "_")
        lines = lines.replace(match.group(0), f"#### {section_name} {{ #{repl} }}\n")
        cleaned_section_name = section_name
        for old, new in clean_labels.items():
            cleaned_section_name = re.sub(
                old, new, cleaned_section_name, flags=re.MULTILINE
            )
        label_dict[label_name] = cleaned_section_name

    return lines, label_dict


def fix_inline_math(lines):
    # Get rid of newlines inside any inline math $...$
    inline_math_pattern = re.compile(r"\$(.+?)\$", re.DOTALL)
    for match in inline_math_pattern.finditer(lines):
        math_content = match.group(1)
        math_content_clean = math_content.replace("\n", " ")
        lines = lines.replace(match.group(0), f"${math_content_clean}$")

    return lines


def default_block(lines, tmpdir):
    lines, label_dict = replace_labels(lines)

    while True:
        old_lines = lines
        for old, new in default_pass.items():
            lines = re.sub(old, new, lines, flags=re.MULTILINE)
        if lines == old_lines:
            break

    lines = insert_code(lines, tmpdir)
    lines = fix_inline_math(lines)
    lines = katex(lines)

    return lines, label_dict
