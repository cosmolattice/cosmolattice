#!/usr/bin/env python3
"""Generate the researcher-map data from the YAML source of truth. Offline — no INSPIRE.

    python3 build_map_data.py            # writes researcher-locations.js (window.CL_MAP_DATA)

Method (option B, agreed): for every author on every paper take the FIRST
affiliation only; dedupe each researcher (by INSPIRE author id, else name); place
them at that affiliation on their MOST RECENT paper that resolves to coordinates;
bin by city. Institutions with no coordinates borrow their city centre from a
geocoded sibling. Emitted as a JS global the map page loads via <script src>.

Each city also carries the papers written from it, for the map's hover card. A
paper counts for a city when at least one of its authors listed that city as
their first affiliation ON THAT PAPER — so a paper stays with the place it was
written from even after its authors move. Titles live once in the top-level
`paper_list`; a city's `papers` holds [paper index, authors from this city]
pairs, newest first, which keeps the generated file small.
"""
import json
import os

import clpubs

# Written as a JS global (not a fetched .json) so the map page works over file://
# and http alike — no fetch, no CORS. Loaded via <script src> on the map page.
OUT = os.path.join(clpubs.DOC_ROOT, "source", "docs", "publications", "researcher-locations.js")


def recency(arxiv):
    h, t = str(arxiv).split(".")
    return int(h) * 100000 + int(t)


def main():
    pubs, insts = clpubs.load()
    insts = {int(k): v for k, v in insts.items()}

    # researcher -> list of (recency, institution id) across all their papers
    researchers = {}
    for p in pubs:
        rec = recency(p["arxiv"])
        for a in p.get("authors", []):
            key = "id:%s" % a["id"] if a.get("id") else "nm:%s" % clpubs.canon(a["name"])
            researchers.setdefault(key, {"name": a["name"], "entries": []})
            researchers[key]["entries"].append((rec, a.get("inst")))

    # city -> coords, from institutions that have them (for the borrow fallback)
    city_geo = {}
    for o in insts.values():
        if o.get("lat") is not None and o.get("city"):
            city_geo["%s|%s" % (o["city"], o.get("country") or "")] = (o["lat"], o["lon"])

    def coords_for(o):
        if o.get("lat") is not None:
            return o["lat"], o["lon"], False
        g = city_geo.get("%s|%s" % (o.get("city"), o.get("country") or "")) if o.get("city") else None
        return (g[0], g[1], True) if g else None

    def city_key(inst_id):
        """City bin key for an author's first affiliation, or None if unplaceable."""
        o = insts.get(inst_id) if inst_id else None
        if not o or not o.get("city") or not coords_for(o):
            return None
        return "%s|%s" % (o["city"], o.get("country") or "")

    bins, unresolved, approx = {}, [], 0
    for r in researchers.values():
        cands = sorted(
            (e for e in r["entries"]
             if e[1] and insts.get(e[1]) and insts[e[1]].get("city") and coords_for(insts[e[1]])),
            key=lambda e: e[0], reverse=True)
        if not cands:
            reason = "no city/coords" if any(e[1] for e in r["entries"]) else "no affiliation"
            unresolved.append({"name": r["name"], "reason": reason})
            continue
        o = insts[cands[0][1]]
        lat, lon, is_approx = coords_for(o)
        approx += is_approx
        bk = "%s|%s" % (o["city"], o.get("country") or "")
        b = bins.setdefault(bk, {"city": o["city"], "country": o.get("country"),
                                 "lat": lat, "lon": lon, "count": 0,
                                 "researchers": [], "institutions": set(), "papers": []})
        b["count"] += 1
        b["researchers"].append(r["name"])
        if o.get("name"):
            b["institutions"].add(o["name"])

    # Papers per city, newest first. Walking the papers in recency order means
    # each city's list comes out sorted without a second pass.
    ordered = sorted(pubs, key=lambda p: recency(p["arxiv"]), reverse=True)
    paper_list = [{"arxiv": p["arxiv"], "title": p["title"]} for p in ordered]
    for ix, p in enumerate(ordered):
        here = {}
        for a in p.get("authors", []):
            bk = city_key(a.get("inst"))
            if bk in bins:
                here[bk] = here.get(bk, 0) + 1
        for bk, n in here.items():
            bins[bk]["papers"].append([ix, n])

    cities = sorted(
        ({"city": b["city"], "country": b["country"], "lat": b["lat"], "lon": b["lon"],
          "count": b["count"], "papers_count": len(b["papers"]), "papers": b["papers"],
          "institutions": sorted(b["institutions"]),
          "researchers": sorted(b["researchers"])} for b in bins.values()),
        key=lambda c: (-c["count"], c["city"]))

    out = {
        "method": "INSPIRE-HEP; first affiliation (option B); researcher placed at most-recent paper",
        "papers": len(pubs),
        "located": sum(c["count"] for c in cities),
        "total_researchers": len(researchers),
        "cities_count": len(cities),
        "countries_count": len({c["country"] for c in cities}),
        "unresolved_count": len(unresolved),
        "approx_placed_count": approx,
        "paper_list": paper_list,
        "cities": cities,
        "unresolved": unresolved,
    }
    with open(OUT, "w", encoding="utf-8") as f:
        f.write("/* Generated by scripts/build_map_data.py from the publications YAML. Do not edit. */\n")
        f.write("window.CL_MAP_DATA = ")
        json.dump(out, f, ensure_ascii=False, indent=1)
        f.write(";\n")

    print("wrote %s" % os.path.basename(OUT))
    print("  %d of %d researchers located · %d cities · %d countries · %d unresolved · %d at city-centre"
          % (out["located"], out["total_researchers"], out["cities_count"],
             out["countries_count"], out["unresolved_count"], out["approx_placed_count"]))
    print("  top: " + ", ".join("%s %d" % (c["city"], c["count"]) for c in cities[:6]))


if __name__ == "__main__":
    main()
