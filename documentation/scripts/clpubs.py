"""Shared helpers for the CosmoLattice publications pipeline.

Source of truth = two YAML files (see README):
  publications.yaml   one entry per "Using CL" paper
  institutions.yaml   INSPIRE institution id -> {name, city, country, lat, lon}

Everything is derived from INSPIRE-HEP by arXiv id. Only stdlib + PyYAML.
"""
import json
import os
import re
import time
import unicodedata
import urllib.request
import urllib.parse

import yaml

HERE = os.path.dirname(os.path.abspath(__file__))          # documentation/scripts
DOC_ROOT = os.path.dirname(HERE)                            # documentation/
CACHE = os.path.join(DOC_ROOT, "tmp", "inspire_cache")     # gitignored
PUBS_YAML = os.path.join(DOC_ROOT, "source", "data", "publications.yaml")
INST_YAML = os.path.join(DOC_ROOT, "source", "data", "institutions.yaml")

UA = "CosmoLattice-publications/0.1 (docs pipeline; aflorio@physik.uni-bielefeld.de)"
SLEEP_S = 0.35  # ~14 req / 5 s, within INSPIRE's guideline

# INSPIRE cities[0] is literal (campus / municipality); collapse to the base city.
CITY_ALIAS = {
    "Burjassot": "Valencia", "Paterna": "Valencia",
    "Orsay": "Paris", "Gif-sur-Yvette": "Paris", "Palaiseau": "Paris", "Meudon": "Paris",
    "Annecy-le-Vieux": "Annecy", "Garching": "Munich", "Zeuthen": "Berlin",
    "Toyonaka": "Osaka", "Parkville": "Melbourne", "Mohanpur": "Kolkata",
    "Cuautitlan": "Mexico City", "Kashiwa": "Tokyo", "Bunkyo": "Tokyo", "Hachioji": "Tokyo",
}
_MUNI_SUFFIX = re.compile(r"[ -]?(shi|ku|gun|cho|machi)$", re.I)


# ---------------------------------------------------------------- YAML I/O ----
class Flow(dict):
    """A dict rendered inline (flow style) so authors/institutions stay one-liners."""


def _flow_repr(dumper, data):
    return dumper.represent_mapping("tag:yaml.org,2002:map", data, flow_style=True)


yaml.add_representer(Flow, _flow_repr)


def load():
    pubs = []
    insts = {}
    if os.path.exists(PUBS_YAML):
        pubs = yaml.safe_load(open(PUBS_YAML, encoding="utf-8")) or []
    if os.path.exists(INST_YAML):
        insts = yaml.safe_load(open(INST_YAML, encoding="utf-8")) or {}
    return pubs, insts


def save(pubs, insts):
    with open(PUBS_YAML, "w", encoding="utf-8") as f:
        f.write("# CosmoLattice publications — source of truth (Using CL list).\n")
        f.write("# Grow with: python3 add_new_citation.py <arxiv-id>\n")
        f.write("# Fields flagged in needs_review want a human eye; empty the list once curated.\n\n")
        yaml.dump(pubs, f, sort_keys=False, allow_unicode=True, width=100)
    with open(INST_YAML, "w", encoding="utf-8") as f:
        f.write("# INSPIRE institution id -> location. Auto-grown; edit coords/city by hand where needed.\n")
        f.write("# lat/lon null => INSPIRE has no coordinates; the map falls back to the city centre.\n\n")
        for iid in sorted(insts, key=int):
            f.write(yaml.dump({iid: Flow(insts[iid])}, allow_unicode=True, sort_keys=False, width=200))


# ------------------------------------------------------------- INSPIRE API ----
def _get(url, cache_key):
    os.makedirs(CACHE, exist_ok=True)
    cf = os.path.join(CACHE, cache_key + ".json")
    if os.path.exists(cf):
        return json.load(open(cf, encoding="utf-8"))
    time.sleep(SLEEP_S)
    req = urllib.request.Request(url, headers={"User-Agent": UA, "Accept": "application/json"})
    with urllib.request.urlopen(req, timeout=30) as r:
        data = json.load(r)
    json.dump(data, open(cf, "w", encoding="utf-8"))
    return data


def fetch_literature(arxiv):
    fields = "titles.title,authors.full_name,authors.record,authors.affiliations," \
             "publication_info,dois,earliest_date,control_number"
    url = "https://inspirehep.net/api/literature?q=arxiv:" + urllib.parse.quote(arxiv) + "&fields=" + fields
    hits = _get(url, "lit_" + arxiv).get("hits", {}).get("hits", [])
    return hits[0]["metadata"] if hits else None


def fetch_institution(iid):
    url = "https://inspirehep.net/api/institutions/%s?fields=addresses,ICN,legacy_ICN,control_number" % iid
    return _get(url, "inst_%s" % iid).get("metadata")


# --------------------------------------------------------------- parsing ------
def ref_id(ref):
    return ref["$ref"].rstrip("/").split("/")[-1] if ref and ref.get("$ref") else None


def display_name(inspire_name):
    """'Figueroa, Daniel G.' -> 'Daniel G. Figueroa'. Leaves collaboration names as-is."""
    if "," in inspire_name:
        last, first = inspire_name.split(",", 1)
        return "%s %s" % (first.strip(), last.strip())
    return inspire_name.strip()


def clean_title(raw):
    """Strip MathML/HTML tags and normalise TeX delimiters. Returns (clean, had_markup).

    INSPIRE stores markup in some titles. This site's MathJax renders \\(..\\) and
    \\[..\\] (see javascripts/mathjax.js) but NOT single-$; so $..$ / $$..$$ are
    converted to the delimiters that actually render.
    """
    had = "<" in raw or "&" in raw or "$" in raw
    txt = re.sub(r"<[^>]+>", "", raw)
    txt = re.sub(r"\$\$(.+?)\$\$", r"\\[\1\\]", txt)   # display math
    txt = re.sub(r"\$(.+?)\$", r"\\(\1\\)", txt)        # inline math
    txt = re.sub(r"\s+", " ", txt).strip()
    return txt, had


def normalize_city(raw):
    if not raw:
        return None
    base = _MUNI_SUFFIX.sub("", raw).strip()
    return CITY_ALIAS.get(base, CITY_ALIAS.get(raw, base))


def canon(name):
    s = unicodedata.normalize("NFD", name)
    s = "".join(c for c in s if unicodedata.category(c) != "Mn")
    return re.sub(r"[.\s]+", " ", s).strip().lower()


def format_journal(pub_info):
    if not pub_info:
        return None
    pi = pub_info[0]
    jt = pi.get("journal_title")
    if not jt:
        return None
    vol = pi.get("journal_volume", "")
    year = pi.get("year", "")
    artid = pi.get("artid") or pi.get("page_start") or ""
    tail = (" (%s)" % year if year else "") + (" %s" % artid if artid else "")
    return ("%s %s%s" % (jt, vol, tail)).strip()


# ----------------------------------------------------------- the core op ------
def ensure_institution(iid, insts):
    """Fetch + normalise an institution into `insts` if not already present. Returns the record."""
    key = int(iid)
    if key in insts:
        return insts[key]
    md = fetch_institution(iid) or {}
    addr = (md.get("addresses") or [{}])[0]
    name = ((md.get("ICN") or [""])[0] or md.get("legacy_ICN") or "")
    name = "" if name.lower() == "obsolete" else name
    insts[key] = {
        "name": name or None,
        "city": normalize_city((addr.get("cities") or [None])[0]),
        "country": addr.get("country_code"),
        "lat": addr.get("latitude"),
        "lon": addr.get("longitude"),
    }
    return insts[key]


def build_entry(arxiv, insts):
    """Fetch one paper, append needed institutions, return (entry_dict, warnings[])."""
    warnings = []
    md = fetch_literature(arxiv)
    if not md:
        return None, ["no INSPIRE record for arXiv:%s — add by hand" % arxiv]

    title, had_markup = clean_title((md.get("titles") or [{}])[0].get("title", ""))
    review = []
    if had_markup:
        review.append("title")
        warnings.append("title contained markup — auto-stripped, please verify: %r" % title)

    authors = []
    for i, a in enumerate(md.get("authors", []), 1):
        aid = ref_id(a.get("record"))
        aff0 = (a.get("affiliations") or [None])[0]
        inst_id = ref_id(aff0.get("record")) if aff0 else None
        if inst_id:
            rec = ensure_institution(inst_id, insts)
            if rec["lat"] is None:
                warnings.append("author %d (%s): institution #%s '%s' has no coordinates on INSPIRE"
                                % (i, a["full_name"], inst_id, rec["name"] or "?"))
        else:
            review.append("author %d affiliation" % i)
            warnings.append("author %d (%s) has no affiliation on this paper" % (i, a["full_name"]))
        authors.append(Flow({"name": display_name(a["full_name"]),
                             "id": int(aid) if aid else None,
                             "inst": int(inst_id) if inst_id else None}))

    journal = format_journal(md.get("publication_info"))
    if not journal:
        warnings.append("no journal yet (preprint) — 'journal' left empty")

    entry = {
        "arxiv": str(arxiv),
        "inspire": md.get("control_number"),
        "title": title,
        "authors": authors,
        "journal": journal,
        "doi": (md.get("dois") or [{}])[0].get("value"),
        "date": md.get("earliest_date"),
        "needs_review": review,
    }
    return entry, warnings
