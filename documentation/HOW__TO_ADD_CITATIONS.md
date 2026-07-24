# How to add a citation to the publications list

All of this is run from `documentation/`.

## The one command

```bash
make add-citation A=2507.04161      # arXiv id, with or without the "arXiv:" prefix
```

It fetches the INSPIRE record for that arXiv id and then, in one go:

1. appends the paper to `source/data/publications.yaml` (list kept newest-first),
2. adds any institution it has never seen to `source/data/institutions.yaml`,
3. regenerates `source/docs/publications/Publications.md` (the `@gen:publications:using-cl` block only),
4. regenerates `source/docs/publications/researcher-locations.js` (the researcher map),
5. prints warnings for anything a human should look at.

Then commit the four files together.

Equivalent without `make`: `python3 scripts/add_new_citation.py <arxiv-id>`.

## About `make: *** [add-citation] Error 1`

**This is not a failure.** `add_new_citation.py` exits non-zero whenever it printed
warnings, so it can be wired into CI or a pre-commit hook. The paper has already been
added and all files have already been written by the time the warnings appear. Read
the `⚠` lines, fix what needs fixing (see below), and move on.

Exit 0 with `nothing flagged — clean import.` means there is nothing to do.

## Which file to edit by hand

Only two files are ever edited by hand. Everything else is generated output.

| Want to fix… | Edit | Then run |
|---|---|---|
| Title, author name, `inst:` of one author, journal, DOI, date, remove a paper | `source/data/publications.yaml` | `make gen-pubs && make gen-map` |
| Institution coordinates, city, country, display name | `source/data/institutions.yaml` | `make gen-map` |

**Never edit by hand:**

- `source/docs/publications/Publications.md` — only the region between
  `<!-- @gen:publications:using-cl -->` and `<!-- @endgen -->` is generated; the
  surrounding prose is hand-written and is left untouched by the generator. Edits
  inside the block are lost on the next run.
- `source/docs/publications/researcher-locations.js` — fully generated.

Useful checks:

```bash
make check-pubs    # no-write check that Publications.md matches publications.yaml
make gen-pubs      # rewrite the generated block in Publications.md
make gen-map       # rewrite researcher-locations.js
make test-pubs     # generator unit tests
```

Note that re-running `make add-citation` on a paper that is already in the YAML is a
no-op — it exits immediately without regenerating anything. After a manual edit, run
`gen-pubs` / `gen-map` directly.

## The warnings you will actually see

**`institution #NNNNNN 'X' has no coordinates on INSPIRE`**

INSPIRE has the institution but no lat/lon, so the entry is written with
`lat: null, lon: null`. The map then falls back to borrowing the coordinates of
another institution in the same city (counted as "at city-centre" in the run summary),
and if no such institution exists the researcher is left unresolved.

Fix it by hand in `source/data/institutions.yaml`, keeping the `note:` convention used
by the other hand-fixed rows:

```yaml
906808: {name: 'Carlos III U., Madrid', city: Madrid, country: ES, lat: 40.3325, lon: -3.7654, note: coords hand-added (UC3M Leganes campus)}
```

Then `make gen-map` and confirm the summary line shows `0 unresolved`.

One quirk to be aware of: researchers are binned by **city**, and the pin for a city
takes the coordinates of whichever researcher entered the bin first. Adding real
campus coordinates to a suburban institution can therefore shift a whole city's marker
by a few km. Harmless, but it explains unexpected diff churn in the `.js`.

**`no journal yet (preprint) — 'journal' left empty`**

Nothing to fix. `journal: null` is correct for a preprint and many entries have it.
There is no automatic refresh, so when the paper is published, fill in `journal:` and
`doi:` by hand in `publications.yaml` and run `make gen-pubs`.

**`author N has no affiliation on this paper`**

INSPIRE has no affiliation on that author's record for this paper. Either leave it
(the researcher is then placed using their other papers, or listed as unresolved), or
add the right INSPIRE institution id as `inst:` on that author in `publications.yaml`.

**`title contained markup — auto-stripped, please verify`**

Check the `title:` line reads correctly and fix it in `publications.yaml` if not.

**`needs_review: [...]`**

Some fields were guessed. Fix them in `publications.yaml`, then empty the
`needs_review` list for that entry.

## Same author, different institutions — this is normal

Authors are identified by their **INSPIRE author id**, not by name, so spelling
variants across papers (`Francisco Torrenti` vs `Francisco Torrentí`) are correctly
treated as one person and never duplicated. The displayed name is the one from their
most recent paper.

The `inst:` field is the affiliation **on that paper**, so the same person legitimately
appears with different institution ids across entries — that is a record of where they
were, not an inconsistency. The map places each researcher at the affiliation on their
**most recent** paper, which is why adding a new paper can move someone from one city
to another.

## Bulk import

`scripts/import_using_cl.py` does the same job for many papers at once and reports how
many entries carry `needs_review` flags and how many institutions lack coordinates. The
per-paper `add-citation` route above is the normal way to add a single new reference.
