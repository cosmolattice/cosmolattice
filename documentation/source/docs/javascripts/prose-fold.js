/* Prose-card grids for the Home page: activates .cl-prose-grid wrappers
   (see extra.css) so their .cl-prose-card children become a 3-column row of
   compact previews; clicking a card expands it across the row with the full,
   untouched text. Only one card per grid is open at a time. With JS disabled
   the cards just stack as plain full-width prose. */
(function () {
    "use strict";

    function setUpCard(card, grid) {
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
            var apply = function () {
                if (open) {
                    // accordion: close the grid's other cards first
                    grid.querySelectorAll(".cl-prose-card.cl-open").forEach(function (other) {
                        if (other !== card) other.clProseSetOpen(false);
                    });
                }
                setOpen(open);
            };
            // smooth relayout where supported
            if (document.startViewTransition) {
                document.startViewTransition(apply);
            } else {
                apply();
            }
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
    }

    if (document.readyState === "loading") {
        document.addEventListener("DOMContentLoaded", init);
    } else {
        init();
    }
})();
