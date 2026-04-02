"""MkDocs hook: resolve @emgithub(file:label) into <script> embed tags."""

from __future__ import annotations

import logging
import os
import re
from typing import Optional
from urllib.parse import quote

import sys

# Import label_scanner relative to this hook's directory, since MkDocs
# may load hooks from different working directories (source/ vs website/).
_hook_dir = os.path.dirname(os.path.abspath(__file__))
if _hook_dir not in sys.path:
    sys.path.insert(0, _hook_dir)
from label_scanner import scan_directory

log = logging.getLogger("mkdocs.hooks.resolve_emgithub")

EMGITHUB_PATTERN = re.compile(r"@emgithub\(([^)]+)\)")

SOURCE_ROOTS = [
    ("cosmolattice", "https://github.com/cosmolattice/cosmolattice/blob/CLV2.0Alpha"),
    ("templat", "https://github.com/cosmolattice/templat/blob/main"),
]

SCRIPT_TEMPLATE = (
    '<script src="https://emgithub.com/embed-v2.js?target='
    "{target}"
    "&style=github&type=code&showBorder=on&showLineNumbers=on"
    '&showFileMeta=on&showCopy=on&fetchFromJsDelivr=on"></script>'
)

_label_index: dict[str, dict[str, tuple[int, int]]] | None = None
_file_github_base: dict[str, str] | None = None


def _get_label_index(config) -> dict[str, dict[str, tuple[int, int]]]:
    """Build the label index on first call, then cache it."""
    global _label_index, _file_github_base
    if _label_index is not None:
        return _label_index

    docs_dir = config["docs_dir"]
    code_source = os.path.normpath(
        os.path.join(docs_dir, "..", "..", "tmp", "code_source")
    )

    _label_index = {}
    _file_github_base = {}

    for dir_name, github_base in SOURCE_ROOTS:
        source_root = os.path.join(code_source, dir_name)
        if not os.path.isdir(source_root):
            log.warning("Label source root not found: %s", source_root)
            continue

        log.info("Scanning labels in %s", source_root)
        labels = scan_directory(
            source_root, extensions=(".h", ".cpp", ".txt", ".in")
        )
        total = sum(len(v) for v in labels.values())
        log.info("Found %d labels across %d files in %s", total, len(labels), dir_name)

        for file_path, file_labels in labels.items():
            if file_path in _label_index:
                log.warning(
                    "File '%s' found in multiple source roots; "
                    "using labels from '%s'", file_path, dir_name,
                )
            _label_index[file_path] = file_labels
            _file_github_base[file_path] = github_base

    return _label_index


def _resolve_match(match: re.Match, page_path: str, label_index) -> str:
    """Resolve a single @emgithub(...) match to a <script> tag."""
    raw = match.group(1).strip()

    if ":" in raw:
        file_path, label_name = raw.rsplit(":", 1)
    else:
        file_path = raw
        label_name = None

    file_path = file_path.strip()

    github_base = (_file_github_base or {}).get(
        file_path, SOURCE_ROOTS[0][1]
    )
    target_url = f"{github_base}/{file_path}"

    if label_name is not None:
        label_name = label_name.strip()
        file_labels = label_index.get(file_path)
        if file_labels is None:
            log.warning(
                "%s: file '%s' has no labels (from @emgithub(%s))",
                page_path, file_path, raw,
            )
            return match.group(0)

        span = file_labels.get(label_name)
        if span is None:
            log.warning(
                "%s: label '%s' not found in '%s' (from @emgithub(%s)). "
                "Available labels: %s",
                page_path, label_name, file_path, raw,
                ", ".join(file_labels.keys()),
            )
            return match.group(0)

        start, end = span
        target_url += f"#L{start}-L{end}"

    encoded_target = quote(target_url, safe="")
    return SCRIPT_TEMPLATE.format(target=encoded_target)


def on_page_markdown(markdown: str, page, config, files) -> str:
    """Replace @emgithub(...) patterns with emgithub <script> tags."""
    if "@emgithub(" not in markdown:
        return markdown

    label_index = _get_label_index(config)
    page_path = page.file.src_path

    return EMGITHUB_PATTERN.sub(
        lambda m: _resolve_match(m, page_path, label_index),
        markdown,
    )
