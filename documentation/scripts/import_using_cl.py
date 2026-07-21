#!/usr/bin/env python3
"""One-time bootstrap: build publications.yaml + institutions.yaml from the
existing hand-maintained "Using CL" list in source/docs/publications/Publications.md.

    python3 migrate.py

Parses the arXiv ids out of the first <ol> (the Using CL list), then runs the
same import path as add_new_citation.py over all of them. Re-runnable: it rebuilds
from scratch, and all INSPIRE responses are cached under ./cache.
"""
import os
import re
import sys

import clpubs

MD = os.path.join(clpubs.DOC_ROOT, "source", "docs", "publications", "Publications.md")


def using_cl_arxiv_ids():
    text = open(MD, encoding="utf-8").read()
    # the Using CL list is the first <ol>…</ol> after the data-kind="publications" mount
    start = text.index('data-kind="publications"')
    ol = text.index("<ol>", start)
    end = text.index("</ol>", ol)
    block = text[ol:end]
    ids, seen = [], set()
    for m in re.finditer(r"arxiv\.org/abs/(\d{4}\.\d{4,5})", block):
        if m.group(1) not in seen:
            seen.add(m.group(1))
            ids.append(m.group(1))
    return ids


def main():
    ids = using_cl_arxiv_ids()
    print("Using CL list: %d papers" % len(ids))
    pubs, insts = [], {}
    flagged = 0
    for i, ax in enumerate(ids, 1):
        sys.stderr.write("[%2d/%d] %s\r" % (i, len(ids), ax))
        entry, warnings = clpubs.build_entry(ax, insts)
        if entry is None:
            print("\n! no record for %s" % ax)
            continue
        pubs.append(entry)
        if entry["needs_review"] or warnings:
            flagged += 1
    sys.stderr.write("\n")

    def recency(ax):
        h, t = ax.split(".")
        return int(h) * 100000 + int(t)

    pubs.sort(key=lambda p: recency(str(p["arxiv"])), reverse=True)
    clpubs.save(pubs, insts)

    no_coords = sum(1 for v in insts.values() if v["lat"] is None)
    print("wrote publications.yaml (%d papers) and institutions.yaml (%d institutions)"
          % (len(pubs), len(insts)))
    print("  %d papers have needs_review flags; %d institutions lack coordinates."
          % (flagged, no_coords))
    print("Next: python3 build_map_data.py")


if __name__ == "__main__":
    main()
