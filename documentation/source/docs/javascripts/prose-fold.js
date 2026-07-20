/* Prose-card grids: activates .cl-prose-grid wrappers (see extra.css) so their
   .cl-prose-card children become a 3-column row of cards showing their full,
   untouched text. With JS disabled the cards just stack as plain full-width
   prose. */
(function () {
    "use strict";

    var FOLD_EM = 6.4;  // keep in sync with single-paragraph folds (p.cl-fold)

    function makeToggle(extraClass) {
        var btn = document.createElement("button");
        btn.type = "button";
        btn.className = "cl-prose-toggle" + (extraClass ? " " + extraClass : "");
        btn.setAttribute("aria-expanded", "false");
        var label = document.createElement("span");
        label.textContent = "Read more";
        var chevron = document.createElement("span");
        chevron.className = "cl-prose-chevron";
        chevron.setAttribute("aria-hidden", "true");
        btn.appendChild(label);
        btn.appendChild(chevron);
        btn.clProseLabel = label;
        return btn;
    }

    // Standalone folded paragraphs ({: .cl-fold } in the Markdown)
    function setUpFold(p) {
        if (p.scrollHeight < FOLD_EM * parseFloat(getComputedStyle(p).fontSize) * 1.3) return;
        p.classList.add("cl-managed");
        var btn = makeToggle("cl-fold-toggle");
        p.insertAdjacentElement("afterend", btn);

        function setOpen(open) {
            p.classList.toggle("cl-open", open);
            btn.setAttribute("aria-expanded", open ? "true" : "false");
            btn.clProseLabel.textContent = open ? "Show less" : "Read more";
        }

        btn.addEventListener("click", function () {
            setOpen(!p.classList.contains("cl-open"));
        });
        p.addEventListener("click", function (e) {
            if (p.classList.contains("cl-open") || e.target.closest("a")) return;
            setOpen(true);
        });
    }

    // Manual-landing contents rail: each .cl-toc-row collapses to its
    // summary line (p.cl-toc-line); a chevron — or clicking the row —
    // reveals the full chapter description underneath. An "Expand all"
    // master toggle above the rail opens every description at once.
    function setUpTocRow(row, onToggle) {
        var line = row.querySelector(".cl-toc-line");
        if (!line) return;
        var hasBody = Array.prototype.some.call(row.children, function (el) {
            return el !== line;
        });
        if (!hasBody) return;

        var btn = document.createElement("button");
        btn.type = "button";
        btn.className = "cl-toc-toggle";
        btn.setAttribute("aria-expanded", "false");
        btn.setAttribute("aria-label", "Show chapter description");
        var chevron = document.createElement("span");
        chevron.className = "cl-prose-chevron";
        chevron.setAttribute("aria-hidden", "true");
        btn.appendChild(chevron);
        line.appendChild(btn);

        function setOpen(open) {
            row.classList.toggle("cl-open", open);
            btn.setAttribute("aria-expanded", open ? "true" : "false");
            btn.setAttribute("aria-label", open ? "Hide chapter description"
                                                : "Show chapter description");
            if (onToggle) onToggle();
        }
        row.clTocSetOpen = setOpen;

        btn.addEventListener("click", function (e) {
            e.stopPropagation();
            setOpen(!row.classList.contains("cl-open"));
        });

        // The whole collapsed row is clickable — but never hijack a link
        // click, and never collapse from the row body (it would fire while
        // selecting text).
        row.addEventListener("click", function (e) {
            if (row.classList.contains("cl-open") || e.target.closest("a")) return;
            setOpen(true);
        });
    }

    function init() {
        document.querySelectorAll(".md-typeset .cl-prose-grid").forEach(function (grid) {
            if (grid.dataset.clProseDone) return;
            grid.dataset.clProseDone = "1";
            grid.classList.add("cl-managed");
        });
        document.querySelectorAll(".md-typeset .cl-toc").forEach(function (rail) {
            if (rail.dataset.clTocDone) return;
            rail.dataset.clTocDone = "1";
            rail.classList.add("cl-managed");

            var rows = [];
            var master = null;

            // Keep the master toggle's label honest whichever way rows are
            // opened — individually or all at once.
            function updateMaster() {
                if (!master) return;
                var allOpen = rows.every(function (r) {
                    return r.classList.contains("cl-open");
                });
                master.clProseLabel.textContent = allOpen ? "Collapse all" : "Expand all";
                master.setAttribute("aria-expanded", allOpen ? "true" : "false");
            }

            rail.querySelectorAll(".cl-toc-row").forEach(function (row) {
                setUpTocRow(row, updateMaster);
                if (row.clTocSetOpen) rows.push(row);
            });

            if (rows.length) {
                master = makeToggle("cl-toc-master");
                master.clProseLabel.textContent = "Expand all";
                master.addEventListener("click", function () {
                    var anyClosed = rows.some(function (r) {
                        return !r.classList.contains("cl-open");
                    });
                    rows.forEach(function (r) { r.clTocSetOpen(anyClosed); });
                });
                rail.insertAdjacentElement("afterbegin", master);
            }
        });
        document.querySelectorAll(".md-typeset p.cl-fold").forEach(function (p) {
            if (p.dataset.clFoldDone) return;
            p.dataset.clFoldDone = "1";
            setUpFold(p);
        });
    }

    if (document.readyState === "loading") {
        document.addEventListener("DOMContentLoaded", init);
    } else {
        init();
    }

})();
