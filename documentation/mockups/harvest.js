#!/usr/bin/env node
/* Harvest researcher locations for the "Using CL" papers from INSPIRE-HEP.
   Method (option B): institution comes straight from each paper's
   authors[].affiliations[0].record (the FIRST affiliation only). Each
   researcher is deduped (by INSPIRE author id, else canonical name) and
   placed once, at the first affiliation on their MOST RECENT paper that
   resolves to an institution with coordinates. Binned by city.

   Everything is cached to ./cache so re-runs cost zero requests. */
const fs = require("fs");
const path = require("path");

const HERE = __dirname;
const CACHE = path.join(HERE, "cache");
fs.mkdirSync(CACHE, { recursive: true });
const UA = "CosmoLattice-researcher-map/0.1 (docs mockup; aflorio@physik.uni-bielefeld.de)";
const SLEEP_MS = 350; // ~14 req / 5 s, within INSPIRE's guideline

// metro / campus -> city aliases (INSPIRE cities[0] is literal)
const CITY_ALIAS = {
  "Burjassot": "Valencia", "Paterna": "Valencia",
  "Orsay": "Paris", "Gif-sur-Yvette": "Paris", "Palaiseau": "Paris", "Meudon": "Paris",
  "Annecy-le-Vieux": "Annecy", "Garching": "Munich", "Zeuthen": "Berlin",
  "Toyonaka": "Osaka", "Parkville": "Melbourne", "Mohanpur": "Kolkata",
  "Cuautitlan": "Mexico City", "Kashiwa": "Tokyo", "Bunkyo": "Tokyo", "Hachioji": "Tokyo",
};
// Japanese municipal suffixes etc. that should collapse to the base city
function normalizeCity(raw){
  if (!raw) return null;
  let c = raw.replace(/[ -]?(shi|ku|gun|cho|machi)$/i, "").trim();
  return CITY_ALIAS[c] || CITY_ALIAS[raw] || c;
}

const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
function deaccent(s){ return s.normalize("NFD").replace(/[̀-ͯ]/g,""); }
function canon(name){ return deaccent(name).replace(/[.\s]+/g," ").trim().toLowerCase(); }

async function getJSON(url, cacheKey){
  const cf = path.join(CACHE, cacheKey + ".json");
  if (fs.existsSync(cf)) return JSON.parse(fs.readFileSync(cf, "utf8"));
  await sleep(SLEEP_MS);
  const res = await fetch(url, { headers: { "User-Agent": UA, "Accept": "application/json" } });
  if (!res.ok) throw new Error(url + " -> HTTP " + res.status);
  const j = await res.json();
  fs.writeFileSync(cf, JSON.stringify(j));
  return j;
}

const refId = (ref) => ref && ref.$ref ? ref.$ref.split("/").pop() : null;

async function litRecord(arxiv){
  const url = "https://inspirehep.net/api/literature?q=arxiv:" + encodeURIComponent(arxiv) +
    "&fields=authors.full_name,authors.record,authors.affiliations,titles.title,earliest_date";
  const j = await getJSON(url, "lit_" + arxiv);
  const hit = j.hits && j.hits.hits && j.hits.hits[0];
  return hit ? hit.metadata : null;
}
async function instRecord(id){
  const url = "https://inspirehep.net/api/institutions/" + id +
    "?fields=addresses,ICN,legacy_ICN,control_number";
  const j = await getJSON(url, "inst_" + id);
  return j.metadata || null;
}

(async () => {
  const arxivs = fs.readFileSync(path.join(HERE, "using_cl_arxiv.txt"), "utf8")
    .split("\n").map((s) => s.trim()).filter(Boolean);
  // recency key: YYMM.number as a comparable number
  const recency = (ax) => { const [a, b] = ax.split("."); return parseInt(a, 10) * 100000 + parseInt(b, 10); };

  const researchers = new Map(); // key -> { name, entries:[{rec:recency, instId}] }
  let noHit = [];

  for (let i = 0; i < arxivs.length; i++){
    const ax = arxivs[i];
    process.stderr.write(`[${i + 1}/${arxivs.length}] ${ax}\r`);
    let md;
    try { md = await litRecord(ax); } catch (e) { console.error("\n! lit", ax, e.message); continue; }
    if (!md || !md.authors) { noHit.push(ax); continue; }
    const rec = recency(ax);
    md.authors.forEach((a) => {
      const aid = refId(a.record);
      const key = aid ? "id:" + aid : "nm:" + canon(a.full_name || "");
      if (!key || key === "nm:") return;
      const aff0 = (a.affiliations && a.affiliations[0]) || null;
      const instId = aff0 ? refId(aff0.record) : null; // FIRST affiliation only
      if (!researchers.has(key)) researchers.set(key, { name: a.full_name, entries: [] });
      researchers.get(key).entries.push({ rec, instId, affValue: aff0 && aff0.value });
    });
  }
  process.stderr.write("\n");

  // resolve institutions (unique)
  const instIds = new Set();
  for (const r of researchers.values())
    r.entries.forEach((e) => { if (e.instId) instIds.add(e.instId); });
  const inst = {}; // id -> {city,country,lat,lon,name}
  let k = 0;
  for (const id of instIds){
    process.stderr.write(`inst [${++k}/${instIds.size}] ${id}\r`);
    let md; try { md = await instRecord(id); } catch (e) { console.error("\n! inst", id, e.message); continue; }
    if (!md) continue;
    const addr = (md.addresses && md.addresses[0]) || {};
    const cityRaw = (addr.cities && addr.cities[0]) || null;
    const city = normalizeCity(cityRaw);
    inst[id] = {
      city, country: addr.country_code || null,
      lat: addr.latitude ?? null, lon: addr.longitude ?? null,
      name: ((md.ICN && md.ICN[0]) || md.legacy_ICN || "").replace(/^obsolete$/i, "") || null,
    };
  }
  process.stderr.write("\n");

  // build a city -> coords table from institutions that DO have coordinates,
  // so a no-coord institution can borrow the coords of a sibling in the same city.
  const cityGeo = {}; // "city|CC" -> [lat,lon]
  for (const o of Object.values(inst))
    if (o.lat != null && o.city) cityGeo[o.city + "|" + (o.country || "")] = [o.lat, o.lon];
  // exact coords if the record has them, else borrow from a sibling in the same city
  function coordsFor(o){
    if (o.lat != null) return { lat: o.lat, lon: o.lon, approx: false };
    if (o.city){ const g = cityGeo[o.city + "|" + (o.country || "")]; if (g) return { lat: g[0], lon: g[1], approx: true }; }
    return null;
  }

  // assign each researcher to most-recent paper with a resolvable, geocoded first-aff
  const unresolved = [];
  let approxCount = 0;
  const cityBins = new Map(); // "city|CC" -> {city,country,lat,lon,count,researchers:[],institutions:Set}
  for (const [key, r] of researchers){
    const cands = r.entries
      .filter((e) => e.instId && inst[e.instId] && inst[e.instId].city && coordsFor(inst[e.instId]))
      .sort((a, b) => b.rec - a.rec);
    if (!cands.length){ unresolved.push({ name: r.name, reason: r.entries.some((e)=>e.instId) ? "no city/coords" : "no affiliation" }); continue; }
    const pick = inst[cands[0].instId];
    const co = coordsFor(pick);
    if (co.approx) approxCount++;
    const bk = pick.city + "|" + (pick.country || "");
    if (!cityBins.has(bk)) cityBins.set(bk, {
      city: pick.city, country: pick.country, lat: co.lat, lon: co.lon,
      count: 0, researchers: [], institutions: new Set(),
    });
    const b = cityBins.get(bk);
    b.count++; b.researchers.push(r.name); if (pick.name) b.institutions.add(pick.name);
  }

  const cities = [...cityBins.values()]
    .map((b) => ({ city: b.city, country: b.country, lat: b.lat, lon: b.lon,
      count: b.count, institutions: [...b.institutions], researchers: b.researchers.sort() }))
    .sort((a, b) => b.count - a.count || a.city.localeCompare(b.city));

  const out = {
    method: "INSPIRE-HEP; first affiliation per author (option B); deduped researcher placed at most-recent paper",
    generated_from: "Using CL list",
    papers: arxivs.length,
    located: cities.reduce((s, c) => s + c.count, 0),
    total_researchers: researchers.size,
    cities_count: cities.length,
    countries_count: new Set(cities.map((c) => c.country)).size,
    unresolved_count: unresolved.length,
    approx_placed_count: approxCount,
    cities, unresolved,
    papers_no_inspire_hit: noHit,
  };
  fs.writeFileSync(path.join(HERE, "researcher-locations.json"), JSON.stringify(out, null, 1));
  console.log(`\nDONE: ${out.total_researchers} researchers, ${out.located} located across ` +
    `${out.cities_count} cities / ${out.countries_count} countries; ` +
    `${out.unresolved_count} unresolved; ${noHit.length} papers had no INSPIRE hit.`);
  console.log("Top 12:");
  cities.slice(0, 12).forEach((c, i) => console.log(`  ${String(i + 1).padStart(2)}. ${c.city} (${c.country}) — ${c.count}`));
})();
