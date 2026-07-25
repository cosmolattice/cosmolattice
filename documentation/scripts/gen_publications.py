#!/usr/bin/env python3
"""Generate the "Using CL" publication list from source/data/publications.yaml.

The list on the Publications page is rendered from the single source of truth
``documentation/source/data/publications.yaml``. This script renders the papers
as the ``<ol>`` markup that ``javascripts/publications.js`` already parses, and
injects it into ``Publications.md`` **between explicit markers only**:

    <!-- @gen:publications:using-cl -->
    ... generated <ol> goes here ...
    <!-- @endgen -->

Everything outside the markers (the intro, the mount div) is left byte-for-byte
untouched. The injection is idempotent:
running the script twice yields the exact same file.

Usage:
    python3 documentation/scripts/gen_publications.py [--check]

With ``--check`` the script writes nothing and exits non-zero if the committed
file is not already up to date (used by build.sh / CI as a warning).
"""

import argparse
import re
import sys
from pathlib import Path

try:
    import yaml
except ImportError:  # pragma: no cover - dependency hint
    sys.exit("error: PyYAML is required (pip install pyyaml)")

# --- Paths (resolved relative to this script, so it runs from anywhere) ------
SCRIPT_DIR = Path(__file__).resolve().parent
DOC_ROOT = SCRIPT_DIR.parent  # documentation/
YAML_PATH = DOC_ROOT / "source" / "data" / "publications.yaml"
MD_PATH = DOC_ROOT / "source" / "docs" / "publications" / "Publications.md"

# --- Marker grammar (same style as gen_param_appendix.py) --------------------
OPEN = "<!-- @gen:publications:using-cl -->"
CLOSE = "<!-- @endgen -->"
MARK = re.compile(r"(%s)(.*?)(%s)" % (re.escape(OPEN), re.escape(CLOSE)), re.S)


def esc(text):
    """Escape a value for inclusion in HTML text (not attributes)."""
    if text is None:
        return ""
    return str(text).replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")


def render_list(pubs):
    """Render the papers as the <ol> the publications widget parses.

    `value` is the stable "Nth paper to use CL" counter: the list is newest
    first, so the top entry gets the highest number and it counts down. A newly
    added paper simply takes the next number without renumbering the rest.
    """
    n = len(pubs)
    out = ["<ol>"]
    for i, p in enumerate(pubs):
        value = n - i
        # each author carries its INSPIRE id (data-aid) so publications.js can
        # dedupe researchers by identity, not by name — two people who share a
        # name (e.g. the two "Jing Liu") stay distinct, matching the map.
        authors = ", ".join(
            '<span class="clau"%s>%s</span>' % (
                (' data-aid="%d"' % a["id"]) if a.get("id") else "", esc(a["name"]))
            for a in p.get("authors", []) if a.get("name"))
        journal = ("<i>%s</i>. " % esc(p["journal"])) if p.get("journal") else ""
        arxiv = esc(p["arxiv"])
        out.append('<li value="%d">' % value)
        out.append("<b>%s</b>.<br>" % esc(p["title"]))
        out.append("%s<br>" % authors)
        out.append('%sarXiv: <a href="https://arxiv.org/abs/%s" target="_blank" '
                   'rel="noopener noreferrer">%s</a><br><br>' % (journal, arxiv, arxiv))
        out.append("</li>")
    out.append("</ol>")
    return "\n".join(out)


def build_block(pubs):
    return "%s\n%s\n%s" % (OPEN, render_list(pubs), CLOSE)


def main(argv):
    ap = argparse.ArgumentParser(description="Render the Using CL list from publications.yaml.")
    ap.add_argument("--check", action="store_true",
                    help="write nothing; exit non-zero if Publications.md is out of date")
    args = ap.parse_args(argv)

    pubs = yaml.safe_load(YAML_PATH.read_text(encoding="utf-8")) or []
    md = MD_PATH.read_text(encoding="utf-8")
    if not MARK.search(md):
        sys.exit("error: markers %s ... %s not found in %s" % (OPEN, CLOSE, MD_PATH))

    block = build_block(pubs)
    new_md = MARK.sub(lambda _m: block, md, count=1)

    rel = MD_PATH.relative_to(DOC_ROOT)
    if args.check:
        if new_md != md:
            print("out of date: %s does not match publications.yaml "
                  "(run 'make gen-pubs' and commit)" % rel, file=sys.stderr)
            return 1
        print("up to date: %s matches publications.yaml (%d papers)" % (rel, len(pubs)))
        return 0

    if new_md != md:
        MD_PATH.write_text(new_md, encoding="utf-8")
        print("wrote %s (%d papers)" % (rel, len(pubs)))
    else:
        print("unchanged: %s already current (%d papers)" % (rel, len(pubs)))
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
