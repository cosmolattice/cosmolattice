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

    function init() {
        document.querySelectorAll(".md-typeset .cl-prose-grid").forEach(function (grid) {
            if (grid.dataset.clProseDone) return;
            grid.dataset.clProseDone = "1";
            grid.classList.add("cl-managed");
            grid.querySelectorAll(".cl-prose-card").forEach(function (card) {
                setUpCard(card, grid);
            });
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
