/* Prose-card grids for the Home page: activates .cl-prose-grid wrappers
   (see extra.css) so their .cl-prose-card children become a 3-column row of
   compact previews; clicking a card expands it across the row with the full,
   untouched text. Only one card per grid is open at a time. With JS disabled
   the cards just stack as plain full-width prose. */
(function () {
    "use strict";

    var CLAMP_EM = 8.2; // keep in sync with the preview max-height in extra.css
    var FOLD_EM = 6.4;  // ditto for single-paragraph folds (p.cl-fold)

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

    function setUpCard(card, grid) {
        // A card whose text already fits the collapsed preview needs no fade,
        // no toggle and no pointer (heights are measured before MathJax
        // typesets, which only ever shrinks the text — safe direction).
        var body = card.querySelector("p:not(.cl-minihead)");
        if (body && body.scrollHeight <= CLAMP_EM * parseFloat(getComputedStyle(body).fontSize) + 8) {
            card.classList.add("cl-short");
            return;
        }

        var btn = document.createElement("button");
        btn.type = "button";
        btn.className = "cl-prose-toggle";
        btn.setAttribute("aria-expanded", "false");
        var label = document.createElement("span");
        label.textContent = "Read more";
        var chevron = document.createElement("span");
        chevron.className = "cl-prose-chevron";
        chevron.setAttribute("aria-hidden", "true");
        btn.appendChild(label);
        btn.appendChild(chevron);
        card.appendChild(btn);

        function setOpen(open) {
            card.classList.toggle("cl-open", open);
            btn.setAttribute("aria-expanded", open ? "true" : "false");
            label.textContent = open ? "Show less" : "Read more";
        }
        card.clProseSetOpen = setOpen;

        function toggle(open) {
            if (open) {
                // accordion: close the grid's other cards first — except
                // "start here" cards, which only close on their own toggle
                grid.querySelectorAll(".cl-prose-card.cl-open").forEach(function (other) {
                    if (other !== card && !other.classList.contains("cl-start")) {
                        other.clProseSetOpen(false);
                    }
                });
            }
            setOpen(open);
        }

        btn.addEventListener("click", function (e) {
            e.stopPropagation();
            toggle(!card.classList.contains("cl-open"));
        });

        // The whole collapsed card is clickable — but never hijack a link
        // click, and never collapse from the card body (it would fire while
        // selecting text).
        card.addEventListener("click", function (e) {
            if (card.classList.contains("cl-open") || e.target.closest("a")) return;
            toggle(true);
        });

        // "Start here" cards greet the reader open (no accordion round-trip
        // through toggle(): both may start open at once)
        if (card.classList.contains("cl-start")) setOpen(true);
    }

    // Manual-landing contents rail: each .cl-toc-row collapses to its
    // summary line (p.cl-toc-line); a chevron — or clicking the row —
    // reveals the full chapter description underneath.
    function setUpTocRow(row) {
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
        }

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

    // Whole-section folds: headings tagged { .cl-sec-fold } keep their place
    // (and their TOC entry) but collapse everything up to the next heading.
    // Collapsing uses height:0 instead of display:none so MathJax typesets
    // the hidden equations at the correct width.
    function setUpSections() {
        document.querySelectorAll(".md-typeset h2.cl-sec-fold, .md-typeset h3.cl-sec-fold").forEach(function (h) {
            if (h.dataset.clSecDone) return;
            h.dataset.clSecDone = "1";
            var body = document.createElement("div");
            body.className = "cl-sec-body";
            var node = h.nextSibling;
            while (node) {
                if (node.nodeType === 1) {
                    if (/^H[123]$/.test(node.tagName)) break;
                    // never swallow the bibliography (citations.js folds it)
                    if (node.classList.contains("footnote")) break;
                }
                var next = node.nextSibling;
                body.appendChild(node);
                node = next;
            }
            h.insertAdjacentElement("afterend", body);
            h.setAttribute("aria-expanded", "false");
            h.setAttribute("title", "Click to expand this section");
            var btn = makeToggle("cl-sec-toggle");
            body.insertAdjacentElement("afterend", btn);

            function setOpen(open) {
                h.classList.toggle("cl-sec-open", open);
                h.setAttribute("aria-expanded", open ? "true" : "false");
                h.setAttribute("title", open ? "Click to collapse this section"
                                             : "Click to expand this section");
                btn.setAttribute("aria-expanded", open ? "true" : "false");
                btn.clProseLabel.textContent = open ? "Show less" : "Read more";
            }
            h.clSecOpen = function () { setOpen(true); };
            h.addEventListener("click", function (e) {
                if (e.target.closest("a")) return; // permalink anchors etc.
                setOpen(!h.classList.contains("cl-sec-open"));
            });
            btn.addEventListener("click", function () {
                setOpen(!h.classList.contains("cl-sec-open"));
            });
            // the faded preview itself expands too — but never hijack a link
            body.addEventListener("click", function (e) {
                if (h.classList.contains("cl-sec-open") || e.target.closest("a")) return;
                setOpen(true);
            });
        });
    }

    // Navigating to an anchor inside (or at) a collapsed section opens it —
    // covers the sidebar TOC, the intro's section links, and equation refs.
    function revealSection() {
        if (!location.hash) return;
        var el;
        try {
            el = document.getElementById(decodeURIComponent(location.hash.slice(1)));
        } catch (e) {
            return;
        }
        if (!el) return;
        var h = el.classList && el.classList.contains("cl-sec-fold") ? el : null;
        var body = el.closest ? el.closest(".cl-sec-body") : null;
        if (body) h = body.previousElementSibling;
        if (h && h.clSecOpen && h.getAttribute("aria-expanded") !== "true") {
            h.clSecOpen();
            requestAnimationFrame(function () {
                el.scrollIntoView({ block: h === el ? "start" : "center" });
            });
        }
    }

    function init() {
        setUpSections();
        revealSection();
        window.addEventListener("hashchange", revealSection);
        document.querySelectorAll(".md-typeset .cl-prose-grid").forEach(function (grid) {
            if (grid.dataset.clProseDone) return;
            grid.dataset.clProseDone = "1";
            grid.classList.add("cl-managed");
            grid.querySelectorAll(".cl-prose-card").forEach(function (card) {
                setUpCard(card, grid);
            });
        });
        document.querySelectorAll(".md-typeset .cl-toc").forEach(function (rail) {
            if (rail.dataset.clTocDone) return;
            rail.dataset.clTocDone = "1";
            rail.classList.add("cl-managed");
            rail.querySelectorAll(".cl-toc-row").forEach(setUpTocRow);
        });
        document.querySelectorAll(".md-typeset p.cl-fold").forEach(function (p) {
            if (p.dataset.clFoldDone) return;
            p.dataset.clFoldDone = "1";
            setUpFold(p);
        });
    }

    // MathJax typesetting shrinks raw \(\mathcal{...}\) source considerably,
    // so cards measured as "long" at DOM-ready may actually fit once math is
    // rendered. Re-check after typesetting and demote them to .cl-short.
    function reconcile() {
        document.querySelectorAll(".cl-prose-grid.cl-managed .cl-prose-card").forEach(function (card) {
            if (card.classList.contains("cl-short") || card.classList.contains("cl-open")) return;
            var body = card.querySelector("p:not(.cl-minihead)");
            if (!body) return;
            if (body.scrollHeight <= CLAMP_EM * parseFloat(getComputedStyle(body).fontSize) + 8) {
                card.classList.add("cl-short");
                var btn = card.querySelector(".cl-prose-toggle");
                if (btn) btn.remove();
            }
        });
    }

    if (document.readyState === "loading") {
        document.addEventListener("DOMContentLoaded", init);
    } else {
        init();
    }

    window.addEventListener("load", function () {
        if (window.MathJax && MathJax.startup && MathJax.startup.promise) {
            MathJax.startup.promise.then(reconcile);
        } else {
            reconcile();
        }
    });
})();
