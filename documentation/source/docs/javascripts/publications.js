/* Publications page: turn the hand-maintained <ol> lists of papers into an
   interactive view — a "papers per year" lattice histogram and a "top users"
   ranked chart in a sticky sidebar, plus live search and click-to-filter by
   year or author. Everything is DERIVED from the list itself (year from the
   arXiv id, authors and journal from the entry text), so maintenance is
   unchanged: add an <li> to the right <ol> and the widgets update themselves.

   When the page carries more than one list, the panels are wrapped in tabs
   (e.g. "Using CL" / "Citing CL"). Each list opts in with an empty marker
   placed just before it:
     <div class="cl-pubs-mount" data-kind="publications"
          data-label="Using CL" data-updated="…"></div>
   data-kind is "publications" (used the code) or "citations" (cite the code
   paper); it tweaks a few labels. data-label is the tab caption.

   Progressive enhancement: with JS off, the raw ordered lists still render.
   No navigation.instant in this theme, so DOMContentLoaded is enough; the
   readyState guard also covers loading after the DOM is parsed. Styling lives
   in extra.css. */
(function () {
  "use strict";

  var ALIAS = { "Yanoi Cui": "Yanou Cui" };
  var DISPLAY = { "francisco torrenti": "Francisco Torrentí",
                  "nicolas loayza": "Nicolás Loayza" };

  function deaccent(s) { return s.normalize("NFD").replace(/[̀-ͯ]/g, ""); }
  function canon(name) { return deaccent(name).replace(/[.\s]+/g, " ").trim().toLowerCase(); }
  function el(tag, cls) { var e = document.createElement(tag); if (cls) e.className = cls; return e; }
  function esc(s) { var d = document.createElement("div"); d.textContent = s; return d.innerHTML; }

  function parse(li) {
    var b = li.querySelector("b");
    if (!b) return null;
    var title = b.textContent.replace(/\s+/g, " ").trim().replace(/\.$/, "");
    var iEl = li.querySelector("i");
    var journal = iEl ? iEl.textContent.replace(/\s+/g, " ").trim().replace(/\.$/, "").trim() : "";
    var link = null, arxiv = null, year = null;
    li.querySelectorAll("a").forEach(function (a) {
      var href = a.getAttribute("href") || "";
      var m = href.match(/arxiv\.org\/abs\/(\d{2})(\d{2})\.\d{4,5}/);
      if (m) { arxiv = a.textContent.trim(); year = 2000 + parseInt(m[1], 10); link = href; }
      else if (!link) { link = href; }
    });
    var full = li.textContent.replace(/\s+/g, " ").trim();
    var after = full.slice(full.indexOf(title) + title.length);
    if (journal) after = after.split(journal)[0];
    after = after.split(/arXiv/i)[0].split(/INSPIRE/i)[0];
    var authors = after.replace(/^[\s.,;:–-]+/, "").replace(/[\s.,;:]+$/, "").trim();
    var list = authors.split(/,| and |&/)
      .map(function (a) { return a.replace(/\s+/g, " ").trim(); })
      .filter(function (a) { return a.length > 2 && /[A-Za-z]/.test(a); })
      .map(function (a) { return ALIAS[a] || a; });
    // Prefer structured authors when present (the generated Using CL list): each
    // <span class="clau" data-aid> carries an INSPIRE author id, so identity —
    // not the name string — drives de-duplication (two different people named
    // "Jing Liu" stay distinct, matching the researcher map). Hand-authored
    // lists have no such spans and fall back to the parsed names.
    var ids;
    var auSpans = li.querySelectorAll("span.clau");
    if (auSpans.length) {
      list = []; ids = [];
      auSpans.forEach(function (s) {
        var nm = s.textContent.replace(/\s+/g, " ").trim();
        if (!nm) return;
        list.push(ALIAS[nm] || nm);
        ids.push(s.getAttribute("data-aid") || null);
      });
      authors = list.join(", ");
    } else {
      ids = list.map(function () { return null; });
    }
    if (!year) { var y = full.match(/\((19|20)(\d{2})\)/); if (y) year = parseInt(y[1] + y[2], 10); }
    return { li: li, idx: li.getAttribute("value") || "", title: title, titleHtml: b.innerHTML,
             authors: authors, authorList: list, authorIds: ids, journal: journal, arxiv: arxiv, link: link,
             year: year, cell: null };
  }

  /* Build one interactive panel from a mount marker + the <ol> that follows it.
     Returns { root, label, kind, count } or null if there is nothing to build. */
  function buildPanel(mount) {
    if (!mount || mount.dataset.ready) return null;
    var ol = mount.nextElementSibling;
    while (ol && ol.tagName !== "OL") ol = ol.nextElementSibling;
    if (!ol) return null;
    mount.dataset.ready = "1";

    var kind = mount.getAttribute("data-kind") || "publications";
    var isCite = kind === "citations";
    var noun = isCite ? "citing works" : "papers";
    var updated = mount.getAttribute("data-updated") || "";
    var label = mount.getAttribute("data-label") || (isCite ? "Citing CL" : "Using CL");
    var mapHref = mount.getAttribute("data-map-href") || "";  // links the "researchers" stat to the map page

    var papers = Array.prototype.map.call(ol.querySelectorAll(":scope > li"), parse).filter(Boolean);
    if (!papers.length) return null;

    var reduce = window.matchMedia("(prefers-reduced-motion: reduce)").matches;

    /* ---- author aggregation (merge accents/dots, keep the nicest form) ---- */
    var abank = {};
    papers.forEach(function (p) {
      p.authorList.forEach(function (a, i) {
        var id = p.authorIds[i];
        var k = id ? "id:" + id : canon(a);            // identity first, name only as fallback
        if (!abank[k]) abank[k] = { count: 0, forms: {}, canon: canon(a) };
        abank[k].count++;
        abank[k].forms[a] = (abank[k].forms[a] || 0) + 1;
      });
    });
    Object.keys(abank).forEach(function (k) {
      var e = abank[k];
      if (DISPLAY[e.canon]) { e.name = DISPLAY[e.canon]; return; }
      var f = e.forms, best = null, bn = -1;
      for (var n in f) { if (f[n] > bn) { bn = f[n]; best = n; } }
      e.name = best;
    });
    var topAuthors = Object.keys(abank)
      .map(function (k) { return { key: k, name: abank[k].name, count: abank[k].count }; })
      .sort(function (a, b) { return b.count - a.count || a.name.localeCompare(b.name); })
      .slice(0, 13);
    var maxAuthor = topAuthors.length ? topAuthors[0].count : 1;

    /* ---- year buckets ---- */
    var yrsArr = papers.filter(function (p) { return p.year; }).map(function (p) { return p.year; });
    var minY = Math.min.apply(null, yrsArr), maxY = Math.max.apply(null, yrsArr);
    var yearList = []; for (var yy = minY; yy <= maxY; yy++) yearList.push(yy);
    var ramp = ["--cl-pub-y0", "--cl-pub-y1", "--cl-pub-y2", "--cl-pub-y3",
                "--cl-pub-y4", "--cl-pub-y5", "--cl-pub-y6"];
    function yColor(yr) {
      var i = maxY === minY ? ramp.length - 1
            : Math.round((yr - minY) / (maxY - minY) * (ramp.length - 1));
      return "var(" + ramp[i] + ")";
    }
    var byYear = {}; yearList.forEach(function (yr) { byYear[yr] = []; });
    papers.forEach(function (p) { if (p.year) byYear[p.year].push(p); });
    var maxCount = Math.max.apply(null, yearList.map(function (yr) { return byYear[yr].length; }));
    var peakYear = yearList.filter(function (yr) { return byYear[yr].length === maxCount; })[0];
    var brick = Math.max(6, Math.min(12, Math.round((250 - (maxCount - 1)) / maxCount)));

    /* ---- scaffold ---- */
    var root = el("div", "cl-pubs");
    root.setAttribute("data-kind", kind);
    root.style.setProperty("--cl-pub-brick", brick + "px");

    var stats = el("div", "cl-pubs-stats");
    stats.appendChild(mkStat(papers.length, noun));
    stats.appendChild(mkSep());
    stats.appendChild((mapHref && !isCite)
      ? mkStatLink(Object.keys(abank).length, "researchers", mapHref)
      : mkStat(Object.keys(abank).length, "researchers"));
    stats.appendChild(mkSep());
    stats.appendChild(mkStat(minY + "–" + maxY, "and counting"));
    if (isFinite(maxCount) && peakYear) {
      stats.appendChild(mkSep());
      stats.appendChild(mkStat(maxCount, "in " + peakYear + ", the busiest year"));
    }
    root.appendChild(stats);

    var layout = el("div", "cl-pubs-layout");
    var main = el("div", "cl-pubs-main");
    var side = el("aside", "cl-pubs-side");
    layout.appendChild(main);
    layout.appendChild(side);
    root.appendChild(layout);

    /* controls */
    var controls = el("div", "cl-pubs-controls");
    var search = el("div", "cl-pubs-search");
    search.innerHTML =
      '<svg viewBox="0 0 24 24" fill="none" stroke-width="2" stroke-linecap="round" aria-hidden="true"><circle cx="11" cy="11" r="7"/><path d="m20 20-3.5-3.5"/></svg>';
    var input = el("input");
    input.type = "search";
    input.placeholder = "Search titles and authors…";
    input.setAttribute("aria-label", "Search " + noun);
    search.appendChild(input);
    var count = el("span", "cl-pubs-count");
    var clearBtn = el("button", "cl-pubs-clear");
    clearBtn.type = "button";
    clearBtn.textContent = "Clear ✕";
    controls.appendChild(search);
    controls.appendChild(count);
    controls.appendChild(clearBtn);
    main.appendChild(controls);

    /* the existing <ol> becomes the styled list, enhanced in place */
    ol.classList.add("cl-pub-list");
    main.appendChild(ol);
    papers.forEach(function (p) {
      p.li.className = "cl-pub";
      var meta = '<span class="yr">' + (p.year || "") + "</span>";
      if (p.journal) meta += '<span class="jr">' + esc(p.journal) + "</span>";
      if (p.link) {
        meta += '<a href="' + esc(p.link) + '" target="_blank" rel="noopener noreferrer">' +
          (p.arxiv ? "arXiv " + esc(p.arxiv) : "INSPIRE") + " ↗</a>";
      }
      var authorsHtml = p.authorList.map(function (a, i) {
        var id = p.authorIds[i];
        var key = id ? "id:" + id : canon(a);
        return '<span class="me" data-a="' + esc(key) + '">' + esc(a) + "</span>";
      }).join(", ");
      p.li.innerHTML =
        '<div class="idx">' + esc(p.idx) + "</div>" +
        '<div class="body"><p class="title">' + p.titleHtml + "</p>" +
        '<p class="authors">' + authorsHtml + "</p></div>" +
        '<div class="meta">' + meta + "</div>";
    });
    var empty = el("p", "cl-pubs-empty");
    empty.innerHTML = "No " + noun + " match. " +
      '<button type="button" class="cl-pubs-clear is-inline">Clear filters</button>';
    main.appendChild(empty);

    /* sidebar: histogram card */
    var histCard = el("section", "cl-card");
    histCard.innerHTML =
      '<h2 class="cl-card-title">' + (isCite ? "Citing works per year" : "Papers per year") + "</h2>" +
      '<p class="cl-card-note">Each brick is one ' + (isCite ? "work" : "paper") +
      ". Hover for the title · click a year to filter.</p>";
    var hist = el("div", "cl-pubs-hist");
    hist.setAttribute("role", "img");
    hist.setAttribute("aria-label", noun + " per year");
    var xlabels = el("div", "cl-pubs-xlabels");
    histCard.appendChild(hist);
    histCard.appendChild(xlabels);
    side.appendChild(histCard);

    /* sidebar: top users/authors card */
    var userCard = el("section", "cl-card");
    userCard.innerHTML =
      '<h2 class="cl-card-title">' + (isCite ? "Top authors" : "Top users") + "</h2>" +
      '<p class="cl-card-note">Authors by ' + (isCite ? "citing work" : "paper") +
      " count · click to filter.</p>";
    var contrib = el("div", "cl-pubs-contrib");
    userCard.appendChild(contrib);
    side.appendChild(userCard);

    if (updated) {
      var upd = el("p", "cl-pubs-updated");
      upd.innerHTML = "<strong>Last update:</strong> " + esc(updated);
      side.appendChild(upd);
    }

    mount.parentNode.insertBefore(root, mount);
    mount.parentNode.removeChild(mount);

    /* ---- state & filtering ---- */
    var state = { q: "", author: null, year: null };
    function anyFilter() { return state.q || state.author || state.year; }
    function matches(p) {
      if (state.year && p.year !== state.year) return false;
      if (state.author) {
        var akeys = p.authorList.map(function (a, i) {
          return p.authorIds[i] ? "id:" + p.authorIds[i] : canon(a);
        });
        if (akeys.indexOf(state.author) < 0) return false;
      }
      if (state.q) {
        var s = state.q.toLowerCase();
        if ((p.title + " " + p.authors).toLowerCase().indexOf(s) < 0) return false;
      }
      return true;
    }

    /* ---- tooltip ---- */
    var tt = el("div", "cl-pubs-tt");
    document.body.appendChild(tt);
    function showTip(p, e) {
      tt.innerHTML = '<div class="tt-t">' + p.titleHtml + "</div>" +
        '<div class="tt-a">' + esc(p.authors) + "</div>" +
        '<div class="tt-y">' + (p.year || "") + (p.journal ? " · " + esc(p.journal) : "") + "</div>";
      tt.classList.add("is-open");
      moveTip(e);
    }
    function moveTip(e) {
      var x = e.clientX + 14, y = e.clientY + 14;
      if (x + 300 > window.innerWidth) x = e.clientX - 300;
      if (y + 130 > window.innerHeight) y = e.clientY - 130;
      tt.style.left = x + "px"; tt.style.top = y + "px";
    }
    function hideTip() { tt.classList.remove("is-open"); }

    /* ---- histogram ---- */
    yearList.forEach(function (yr) {
      var col = el("div", "cl-pubs-col");
      var stack = el("div", "cl-pubs-stack");
      byYear[yr].slice().reverse().forEach(function (p) {
        var c = el("div", "cl-pubs-cell");
        c.style.setProperty("--cl-pub-cell", yColor(yr));
        p.cell = c;
        c.addEventListener("mouseenter", function (e) { showTip(p, e); });
        c.addEventListener("mousemove", moveTip);
        c.addEventListener("mouseleave", hideTip);
        c.addEventListener("click", function () { jumpTo(p); });
        stack.appendChild(c);
      });
      col.appendChild(stack);
      hist.appendChild(col);

      var tick = el("button", "cl-pubs-xtick");
      tick.type = "button";
      tick.setAttribute("aria-pressed", "false");
      tick.innerHTML = '<span class="yr">' + String(yr).slice(2) + "</span>" +
        '<span class="ct">' + byYear[yr].length + "</span>";
      tick.title = yr + " — " + byYear[yr].length + " " + noun;
      tick.addEventListener("click", function () {
        state.year = state.year === yr ? null : yr; state.author = null; apply();
      });
      tick._y = yr;
      xlabels.appendChild(tick);
    });

    /* ---- contributors ---- */
    topAuthors.forEach(function (a) {
      var b = el("button", "cl-pubs-crow");
      b.type = "button";
      b.setAttribute("aria-pressed", "false");
      b.innerHTML = '<span class="nm">' + esc(a.name) + "</span>" +
        '<span class="val">' + a.count + "</span>" +
        '<span class="bar"><i style="width:' + (a.count / maxAuthor * 100) + '%"></i></span>';
      b.addEventListener("click", function () {
        state.author = state.author === a.key ? null : a.key; state.year = null; apply();
      });
      b._author = a.key;
      contrib.appendChild(b);
    });

    /* ---- author name clicks inside the list ---- */
    ol.addEventListener("click", function (e) {
      var a = e.target.closest(".me");
      if (!a) return;
      state.author = state.author === a.dataset.a ? null : a.dataset.a;
      state.year = null; apply();
    });

    /* ---- apply ---- */
    function apply() {
      var n = 0;
      papers.forEach(function (p) {
        var m = matches(p);
        p.li.classList.toggle("is-hidden", !m);
        if (p.cell) p.cell.classList.toggle("is-dim", anyFilter() && !m);
        if (m) n++;
      });
      count.textContent = anyFilter() ? "showing " + n + " / " + papers.length
                                      : papers.length + " " + noun;
      var on = !!anyFilter();
      root.querySelectorAll(".cl-pubs-clear").forEach(function (b) { b.classList.toggle("is-shown", on); });
      empty.classList.toggle("is-shown", n === 0);
      Array.prototype.forEach.call(contrib.children, function (b) {
        b.setAttribute("aria-pressed", state.author === b._author);
      });
      Array.prototype.forEach.call(xlabels.children, function (b) {
        b.setAttribute("aria-pressed", state.year === b._y);
      });
    }
    function reset() { state = { q: "", author: null, year: null }; input.value = ""; apply(); }

    function jumpTo(p) {
      if (p.li.classList.contains("is-hidden")) reset();
      p.li.scrollIntoView({ behavior: reduce ? "auto" : "smooth", block: "center" });
      root.querySelectorAll(".cl-pub.is-target").forEach(function (x) { x.classList.remove("is-target"); });
      p.li.classList.add("is-target");
      setTimeout(function () { p.li.classList.remove("is-target"); }, 2200);
    }

    input.addEventListener("input", function (e) { state.q = e.target.value; apply(); });
    root.querySelectorAll(".cl-pubs-clear").forEach(function (b) { b.addEventListener("click", reset); });
    apply();

    return { root: root, label: label, kind: kind, count: papers.length };

    function mkStat(nv, l) { var s = el("span", "s"); s.innerHTML = "<b>" + esc(String(nv)) + "</b> " + esc(l); return s; }
    function mkStatLink(nv, l, href) {
      var a = el("a", "s cl-pubs-maplink"); a.href = href;
      a.setAttribute("aria-label", "See where these researchers are based — open the map");
      a.innerHTML =
        '<svg class="pin" viewBox="0 0 24 24" fill="currentColor" aria-hidden="true">' +
        '<path d="M12 2C8.1 2 5 5.1 5 9c0 5.2 7 13 7 13s7-7.8 7-13c0-3.9-3.1-7-7-7zm0 9.5A2.5 2.5 0 1 1 12 6.5a2.5 2.5 0 0 1 0 5z"/></svg>' +
        "<b>" + esc(String(nv)) + '</b> <span class="lbl">' + esc(l) + "</span>";
      return a;
    }
    function mkSep() { var s = el("span", "sep"); s.textContent = "·"; return s; }
  }

  /* Wrap several panels in a tablist; only the active panel is shown. */
  function buildTabs(panels) {
    var bar = el("div", "cl-pubs-tabbar");
    bar.setAttribute("role", "tablist");
    bar.setAttribute("aria-label", "Publication lists");

    panels.forEach(function (p) {
      var b = el("button", "cl-pubs-tab");
      b.type = "button";
      b.setAttribute("role", "tab");
      b.innerHTML = esc(p.label) + ' <span class="tc">' + p.count + "</span>";
      b.addEventListener("click", function () {
        select(p);
        if (window.history && history.replaceState) history.replaceState(null, "", "#" + p.kind);
      });
      p.tab = b;
      p.root.setAttribute("role", "tabpanel");
      p.root.setAttribute("aria-labelledby", (p.root.id = "cl-pubs-tab-" + p.kind + "-panel"));
      b.id = "cl-pubs-tab-" + p.kind;
      bar.appendChild(b);
    });

    panels[0].root.parentNode.insertBefore(bar, panels[0].root);

    function select(active) {
      panels.forEach(function (p) {
        var on = p === active;
        p.root.hidden = !on;
        p.tab.classList.toggle("is-active", on);
        p.tab.setAttribute("aria-selected", on ? "true" : "false");
        p.tab.tabIndex = on ? 0 : -1;
      });
    }

    bar.addEventListener("keydown", function (e) {
      if (e.key !== "ArrowRight" && e.key !== "ArrowLeft") return;
      var i = -1;
      panels.forEach(function (p, j) { if (p.tab === document.activeElement) i = j; });
      if (i < 0) return;
      e.preventDefault();
      var n = (i + (e.key === "ArrowRight" ? 1 : panels.length - 1)) % panels.length;
      select(panels[n]); panels[n].tab.focus();
    });

    var hash = (location.hash || "").replace("#", "");
    var start = null;
    panels.forEach(function (p) { if (p.kind === hash) start = p; });
    select(start || panels[0]);
  }

  function init() {
    var mounts = Array.prototype.slice.call(document.querySelectorAll(".cl-pubs-mount"));
    if (!mounts.length) return;
    var panels = mounts.map(buildPanel).filter(Boolean);
    if (panels.length > 1) buildTabs(panels);
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", init);
  } else {
    init();
  }
})();
