/* Citation presentation for pages using mkdocs-bibtex footnotes:
   1. Runs of 3+ consecutive citation superscripts collapse into a single
      "[49–56]" chip that expands on click (each number keeps its link and
      its Material hover tooltip).
   2. The auto-generated bibliography at the page bottom folds behind a
      "References (N)" bar, opening into a two-column list.
   Both are anchor-aware: following a citation or a back-reference (↩)
   opens whatever hides the target. Styling lives in extra.css. */
(function () {
    "use strict";

    function isCiteSup(node) {
        return node && node.nodeType === 1 && node.tagName === "SUP"
            && node.querySelector("a.footnote-ref");
    }

    function wrapRun(run) {
        var nums = run.map(function (s) { return s.textContent.trim(); });
        var group = document.createElement("span");
        group.className = "cl-cite-group";
        var btn = document.createElement("button");
        btn.type = "button";
        btn.className = "cl-cite-toggle";
        var closedLabel = "[" + nums[0] + "–" + nums[nums.length - 1] + "]";
        btn.textContent = closedLabel;
        btn.title = run.length + " references — click to show";
        run[0].parentNode.insertBefore(group, run[0]);
        group.appendChild(btn);
        run.forEach(function (s) { group.appendChild(s); });

        function setOpen(open) {
            group.classList.toggle("cl-open", open);
            btn.textContent = open ? "«" : closedLabel;
            btn.title = open ? "collapse references"
                             : run.length + " references — click to show";
        }
        group.clCiteOpen = function () { setOpen(true); };
        btn.addEventListener("click", function () {
            setOpen(!group.classList.contains("cl-open"));
        });
    }

    function groupRuns() {
        var seen = new Set();
        document.querySelectorAll(".md-typeset sup").forEach(function (sup) {
            if (seen.has(sup) || !isCiteSup(sup)) return;
            var run = [sup];
            var node = sup.nextSibling;
            while (node) {
                if (node.nodeType === 3 && !node.textContent.trim()) {
                    node = node.nextSibling;
                    continue;
                }
                if (isCiteSup(node)) {
                    run.push(node);
                    node = node.nextSibling;
                    continue;
                }
                break;
            }
            run.forEach(function (s) { seen.add(s); });
            if (run.length >= 3) wrapRun(run);
        });
    }

    var openBib = null; // set once the bibliography is folded

    function foldBibliography() {
        var bib = document.querySelector(".md-typeset .footnote");
        if (!bib || bib.dataset.clBibDone) return;
        bib.dataset.clBibDone = "1";
        var count = bib.querySelectorAll("ol > li").length;

        var bar = document.createElement("button");
        bar.type = "button";
        bar.className = "cl-bib-toggle";
        bar.setAttribute("aria-expanded", "false");
        var label = document.createElement("span");
        label.textContent = "References (" + count + ")";
        var chevron = document.createElement("span");
        chevron.className = "cl-prose-chevron";
        chevron.setAttribute("aria-hidden", "true");
        bar.appendChild(label);
        bar.appendChild(chevron);
        bib.parentNode.insertBefore(bar, bib);
        bib.classList.add("cl-bib-folded");

        // arXiv / DOI links as compact chips; the long DOI strings become a
        // uniform "DOI" label with the full identifier kept in the tooltip
        bib.querySelectorAll('a[href*="arxiv.org"]').forEach(function (a) {
            a.classList.add("cl-ref-chip");
        });
        bib.querySelectorAll('a[href*="doi.org"]').forEach(function (a) {
            a.title = a.textContent;
            a.textContent = "DOI";
            a.classList.add("cl-ref-chip");
        });
        // the chips absorb their role as list items, so drop the leftover
        // ", " / "." separators that followed the original link texts
        bib.querySelectorAll(".cl-ref-chip").forEach(function (a) {
            var t = a.nextSibling;
            if (t && t.nodeType === 3 && /^[\s,.;]+$/.test(t.textContent)) {
                t.textContent = " ";
            }
        });

        function setOpen(open) {
            bib.classList.toggle("cl-bib-open", open);
            bar.setAttribute("aria-expanded", open ? "true" : "false");
        }
        bar.addEventListener("click", function () {
            setOpen(!bib.classList.contains("cl-bib-open"));
        });
        openBib = function () { setOpen(true); };
    }

    // Opening whatever hides the element the URL hash points at: a footnote
    // inside the folded bibliography, or a citation inside a collapsed chip.
    function revealHash() {
        if (!location.hash) return;
        var el;
        try {
            el = document.getElementById(decodeURIComponent(location.hash.slice(1)));
        } catch (e) {
            return;
        }
        if (!el) return;
        var group = el.closest(".cl-cite-group");
        if (group && group.clCiteOpen) group.clCiteOpen();
        if (el.closest(".footnote") && openBib) openBib();
        requestAnimationFrame(function () {
            el.scrollIntoView({ block: "center" });
        });
    }

    function init() {
        groupRuns();
        foldBibliography();
        revealHash();
        window.addEventListener("hashchange", revealHash);
    }

    if (document.readyState === "loading") {
        document.addEventListener("DOMContentLoaded", init);
    } else {
        init();
    }
})();
