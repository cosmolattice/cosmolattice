import regex as re
from util.katex import katex

tag_counter = 1


def math_labels(lines):
    ################################################
    # Find instances of \label{labelname} inside ```math...``` and wrap the entire block with <a name="labelname"></a>
    ################################################

    # print("    Processing labels...")

    label_dict = {}
    global tag_counter

    label_pattern = re.compile(r"\$\$\n(.*?)\$\$", re.DOTALL)
    for match in label_pattern.finditer(lines):
        prefix = ""
        label_content = match.group(1)
        label_name = re.search(r"\\label\{(.+?)\}", label_content)
        while label_name:
            label_content = label_content.replace(
                label_name.group(0), r"\tag{" + str(tag_counter) + "}"
            )
            label_name = label_name.group(1)
            repl = label_name.replace(":", "_")
            prefix += "[](){ #" + f"{repl}" + " }\n"
            label_dict[label_name] = f"{tag_counter}"
            tag_counter += 1
            label_name = re.search(r"\\label\{(.+?)\}", label_content)

        lines = lines.replace(
            match.group(0),
            prefix + "$$\n" + label_content + f"\n$$",
        )

    # now get rid of these damn labels
    label_cleanup_pattern = re.compile(r"\\label\{(.+?)\}")
    lines = label_cleanup_pattern.sub(r"", lines)

    return lines, label_dict


def math_block(lines):
    # get rid of superfluous \n
    lines = lines.replace("\\begin{align}\n\n", "\\begin{align}\n")
    lines = lines.replace("\n\n\\end{align}", "\n\\end{align}")
    lines = lines.replace("&&", "&")

    # remove comments
    # first, lines that are only comments
    # lines = re.sub(r"^\s*%.*$\n", r"", lines, flags=re.MULTILINE)
    # then, comments at the end of lines
    # lines = re.sub(r"(?<!\\)%.*$", r"", lines, flags=re.MULTILINE)

    lines, label_dict = math_labels(lines)
    lines = katex(lines)

    return lines, label_dict
