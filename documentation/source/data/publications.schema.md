# Publications data — schema & workflow

The **Using CL** list on the Publications page is generated from two YAML files
in this directory (the single source of truth), the same way the parameter
appendix is generated from `parameters.yaml`.

| File | Role |
|------|------|
| `publications.yaml` | one entry per paper — the source of truth for the list |
| `institutions.yaml` | INSPIRE institution id → location; the researcher-map's geo lookup |

The researcher map is a page (`publications/Researcher-map.html`, reached from the
"researchers" stat on the Publications page). `build_map_data.py` writes its data
to `source/docs/publications/researcher-locations.js` (a `window.CL_MAP_DATA`
global — generated, do not edit). The page loads it and the static
`world-lowres.js` geometry via `<script src>` (no `fetch`, so the page also works
over `file://`); `javascripts/researcher-map.js` renders from those globals.

Everything is derived from **INSPIRE-HEP** by arXiv id. Only `PyYAML` is needed
(already a docs dependency); HTTP uses the standard library. INSPIRE responses
are cached under `tmp/inspire_cache/` (gitignored), so re-runs are offline.

## `publications.yaml`

```yaml
- arxiv: "2102.01031"          # primary key
  inspire: 1844216             # INSPIRE recid
  title: "CosmoLattice: ..."   # $..$ math is normalised to \(..\) on import
  authors:
    - {name: Daniel G. Figueroa, id: 1046060, inst: 903319}   # id = INSPIRE author id, inst = institution id
  journal: "Comput.Phys.Commun. 283 (2023) 108586"   # null while a preprint
  doi: "10.1016/j.cpc.2022.108586"
  date: 2021-02-01
  needs_review: [title]        # fields a human should check; empty once curated
```

## `institutions.yaml`

```yaml
903319: {name: "IFIC, Valencia", city: Valencia, country: ES, lat: 39.5094, lon: -0.4105}
902670: {name: Bielefeld U., city: Bielefeld, country: DE, lat: 52.0378, lon: 8.4934, note: coords hand-fixed}
```

`lat/lon: null` means INSPIRE has no coordinates; add them by hand and (optionally)
a `note:`. **Manual edits persist** — the scripts only ever *add* new institutions,
never overwrite existing ones.

## Workflow

Add a paper (fetches INSPIRE, grows the YAML, regenerates the page **and** the map):

```bash
make add-citation A=2507.04161        # or: python3 scripts/add_new_citation.py 2507.04161
```

It warns about anything needing a human (markup in the title, an author with no
affiliation, an institution with no coordinates, a missing journal) and clears
nothing silently. After curating a flagged entry in the YAML:

```bash
make gen-pubs      # re-render the Using CL list in Publications.md
make gen-map       # re-render researcher-locations.js (window.CL_MAP_DATA)
make check-pubs    # verify Publications.md matches the YAML (build.sh runs this as a warning)
make test-pubs     # generator unit tests
```

## Rendering

`scripts/gen_publications.py` injects the `<ol>` **between markers only** in
`source/docs/publications/Publications.md`:

```
<!-- @gen:publications:using-cl -->
 ... generated <ol> ...
<!-- @endgen -->
```

Everything outside the markers (the intro, the mount div, the hand-maintained
**Citing CL** list) is left byte-for-byte untouched; `javascripts/publications.js`
parses the generated `<ol>` exactly as before.

## One-time bootstrap

`scripts/import_using_cl.py` rebuilt these YAMLs from the previously
hand-maintained list in `Publications.md`. It is kept for reference; you should
not need it again.
