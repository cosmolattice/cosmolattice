"""Scan source files for // @label:name ... // @endlabel markers."""

import os
import re
import warnings

LABEL_START = re.compile(r"^\s*(?://|#)\s*@label:(\S+)")
LABEL_END = re.compile(r"^\s*(?://|#)\s*@endlabel")


def scan_file(path: str) -> dict[str, tuple[int, int]]:
    """Scan a single file for label markers.

    Returns a dict mapping label names to (start_line, end_line) tuples.
    Line numbers are 1-based. The marker lines themselves are excluded:
    start_line is the line after @label, end_line is the line before @endlabel.
    """
    labels: dict[str, tuple[int, int]] = {}
    open_label: str | None = None
    open_start: int = 0

    with open(path, encoding="utf-8") as f:
        for lineno, line in enumerate(f, start=1):
            start_match = LABEL_START.match(line)
            end_match = LABEL_END.match(line)

            if start_match:
                name = start_match.group(1)
                if open_label is not None:
                    warnings.warn(
                        f"{path}:{lineno}: opening @label:{name} while "
                        f"@label:{open_label} (opened at line {open_start}) "
                        f"is still open — closing it implicitly"
                    )
                if name in labels:
                    warnings.warn(
                        f"{path}:{lineno}: duplicate label '{name}' "
                        f"(first defined at line {labels[name][0]})"
                    )
                open_label = name
                open_start = lineno

            elif end_match:
                if open_label is None:
                    warnings.warn(
                        f"{path}:{lineno}: @endlabel without a preceding @label"
                    )
                else:
                    labels[open_label] = (open_start + 1, lineno - 1)
                    open_label = None

    if open_label is not None:
        warnings.warn(
            f"{path}: unclosed @label:{open_label} (opened at line {open_start})"
        )

    return labels


def scan_directory(
    root: str, extensions: tuple[str, ...] = (".h", ".cpp")
) -> dict[str, dict[str, tuple[int, int]]]:
    """Scan a directory tree for label markers.

    Returns a dict mapping relative file paths to their label dicts.
    Only files matching the given extensions are scanned.
    Files with no labels are omitted from the result.
    """
    result: dict[str, dict[str, tuple[int, int]]] = {}

    for dirpath, _dirs, filenames in os.walk(root):
        for filename in filenames:
            if not filename.endswith(extensions):
                continue
            filepath = os.path.join(dirpath, filename)
            labels = scan_file(filepath)
            if labels:
                relpath = os.path.relpath(filepath, root)
                result[relpath] = labels

    return result
