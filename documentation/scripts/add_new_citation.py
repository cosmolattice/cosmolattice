#!/usr/bin/env python3
"""Add one paper to the publications source of truth from its arXiv id.

    python3 add_new_citation.py 2507.04161

Fetches the INSPIRE record, appends an entry to publications.yaml (newest first)
and any new institutions to institutions.yaml, then prints warnings for anything
that needs a human. Exit code is non-zero when review is needed, so it is safe to
wire into CI / a pre-commit check.
"""
import sys

import clpubs
import gen_publications
import build_map_data


def recency(arxiv):
    head, tail = arxiv.split(".")
    return int(head) * 100000 + int(tail)


def main(argv):
    if len(argv) != 1:
        print("usage: python3 add_new_citation.py <arxiv-id>", file=sys.stderr)
        return 2
    arxiv = argv[0].strip().replace("arXiv:", "").replace("arxiv:", "")

    pubs, insts = clpubs.load()
    if any(str(p.get("arxiv")) == arxiv for p in pubs):
        print("• arXiv:%s is already in publications.yaml — nothing to do." % arxiv)
        return 0

    entry, warnings = clpubs.build_entry(arxiv, insts)
    if entry is None:
        for w in warnings:
            print("  ⚠ " + w, file=sys.stderr)
        return 1

    # insert keeping the list newest-first by arXiv id
    pubs.append(entry)
    pubs.sort(key=lambda p: recency(str(p["arxiv"])), reverse=True)
    clpubs.save(pubs, insts)

    # regenerate the on-page list and the map data so one command updates everything
    gen_publications.main([])
    build_map_data.main()

    print("\n✓ added: %s" % entry["title"])
    print("  %d authors · %s" % (len(entry["authors"]), entry["journal"] or "preprint"))
    if warnings:
        print("\n  %d thing(s) to check by hand:" % len(warnings))
        for w in warnings:
            print("  ⚠ " + w)
        if entry["needs_review"]:
            print("\n  needs_review: %s  (clear it in publications.yaml once fixed)" % entry["needs_review"])
        return 1
    print("  nothing flagged — clean import.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
