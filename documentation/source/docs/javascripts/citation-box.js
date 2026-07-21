/* Citation box on the "If you use CosmoLattice" page (CLcitation.md):
   a small tabbed card (Citation / LaTeX / BibTeX). The LaTeX and BibTeX
   panes carry a copy button that writes the pane's <code> text to the
   clipboard and flips to a "Copied!" state for a moment. Styling lives in
   extra.css (.cl-cite*). Progressive enhancement: with JS off the panes
   render as plain, selectable text blocks. */
(function () {
    "use strict";

    function copyText(text) {
        if (navigator.clipboard && navigator.clipboard.writeText) {
            return navigator.clipboard.writeText(text);
        }
        // Fallback for older / non-secure contexts.
        return new Promise(function (resolve, reject) {
            var ta = document.createElement("textarea");
            ta.value = text;
            ta.setAttribute("readonly", "");
            ta.style.position = "absolute";
            ta.style.left = "-9999px";
            document.body.appendChild(ta);
            ta.select();
            try {
                document.execCommand("copy") ? resolve() : reject();
            } catch (e) {
                reject(e);
            }
            document.body.removeChild(ta);
        });
    }

    function wireCard(card) {
        if (card.dataset.clCiteDone) return;
        card.dataset.clCiteDone = "1";

        var tabs = Array.prototype.slice.call(
            card.querySelectorAll(".cl-cite__tab"));
        var panes = Array.prototype.slice.call(
            card.querySelectorAll(".cl-cite__pane"));

        function select(name) {
            tabs.forEach(function (t) {
                var on = t.dataset.pane === name;
                t.classList.toggle("is-active", on);
                t.setAttribute("aria-selected", on ? "true" : "false");
            });
            panes.forEach(function (p) {
                var on = p.dataset.pane === name;
                p.classList.toggle("is-active", on);
                if (on) { p.removeAttribute("hidden"); }
                else { p.setAttribute("hidden", ""); }
            });
        }

        tabs.forEach(function (t) {
            t.addEventListener("click", function () { select(t.dataset.pane); });
        });

        card.querySelectorAll(".cl-cite__copy").forEach(function (btn) {
            var pane = btn.closest(".cl-cite__pane");
            var code = pane && pane.querySelector("code");
            var label = btn.querySelector(".cl-cite__copy-label");
            var original = label ? label.textContent : "Copy";
            var resetTimer = null;

            btn.addEventListener("click", function () {
                if (!code) return;
                copyText(code.textContent).then(function () {
                    btn.classList.add("is-copied");
                    if (label) label.textContent = "Copied!";
                    if (resetTimer) clearTimeout(resetTimer);
                    resetTimer = setTimeout(function () {
                        btn.classList.remove("is-copied");
                        if (label) label.textContent = original;
                    }, 1600);
                }).catch(function () {
                    if (label) label.textContent = "Press ⌘/Ctrl+C";
                });
            });
        });
    }

    function init() {
        document.querySelectorAll("[data-cl-cite]").forEach(wireCard);
    }

    if (document.readyState === "loading") {
        document.addEventListener("DOMContentLoaded", init);
    } else {
        init();
    }
})();
