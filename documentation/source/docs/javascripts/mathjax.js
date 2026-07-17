// Single merged MathJax config. NOTE: assign window.MathJax exactly ONCE —
// a second assignment before MathJax loads silently discards the first
// (this previously wiped the arithmatex options when the AMS-numbering
// settings were added as a separate object on June 24th 2026).
window.MathJax = {
    loader: {
        // \class / \style / \href (used to color sector terms in equations)
        load: ["[tex]/html"]
    },
    tex: {
        inlineMath: [["\\(", "\\)"]],
        displayMath: [["\\[", "\\]"]],
        processEscapes: true,
        processEnvironments: true,
        tags: "ams",        // automatic numbering
        useLabelIds: true,  // allows \label{} and \ref{}
        packages: { "[+]": ["html"] }
    },
    // No options.ignoreHtmlClass/processHtmlClass here: the nav sidebar and
    // page TOC contain LaTeX outside .arithmatex wrappers, so MathJax must
    // scan the whole page (the theme's default behavior on this site).
    startup: {
        // Decorate the sector terms after the initial typeset. This is the
        // path that actually runs on a normal page load — the document$
        // subscription below fires before the MathJax library has loaded.
        pageReady() {
            return MathJax.startup.defaultPageReady().then(clLinkEquationSectors)
        }
    }
};

// Sector-colored equation terms (\class{cl-eq-*}{...} in the LaTeX source)
// link to their user-manual chapter. Done in JS rather than TeX \href because
// MathJax drops the spaces that appear in some manual filenames. URLs are
// relative to the site root, where the pages using these classes live.
const CL_EQ_SECTOR_LINKS = {
    "cl-eq-scalar":    "Manual/My first model of (singlet) scalar fields.html",
    "cl-eq-u1-kin":    "Manual/My first model of gauge fields.html",
    "cl-eq-u1-gauge":  "Manual/My first model of gauge fields.html",
    "cl-eq-su2-kin":   "Manual/My first model of gauge fields.html",
    "cl-eq-su2-gauge": "Manual/My first model of gauge fields.html",
    "cl-eq-alp-kin":   "Manual/ALP.html",
    "cl-eq-alp-coup":  "Manual/ALP.html",
    "cl-eq-nmc-kin":   "Manual/NMC.html",
    "cl-eq-nmc-coup":  "Manual/NMC.html"
};

function clWire(el, url) {
    if (el.dataset.clLinked) return;
    el.dataset.clLinked = "1";
    el.setAttribute("role", "link");
    el.setAttribute("tabindex", "0");
    el.addEventListener("click", () => { window.location.href = url; });
    el.addEventListener("keydown", e => {
        if (e.key === "Enter") window.location.href = url;
    });
}

function clLinkEquationSectors() {
    for (const [cls, url] of Object.entries(CL_EQ_SECTOR_LINKS)) {
        document.querySelectorAll("." + cls).forEach(el => clWire(el, url));
    }
    // The GW equations are boxed as a whole panel: cl-gw-mark only tags the
    // math, the clickable element is the surrounding MathJax container.
    document.querySelectorAll(".cl-gw-mark").forEach(el => {
        clWire(el.closest("mjx-container") || el, "Manual/GW.html");
    });
}

document$.subscribe(() => {
    // On the first emission (initial page load) the MathJax library has not
    // replaced the config object yet — window.MathJax has no API methods and
    // this callback would throw. Initial typeset + decoration are handled by
    // startup.pageReady above; this path only matters for soft navigations.
    if (!window.MathJax || !window.MathJax.typesetPromise) return
    MathJax.startup.output.clearCache()
    MathJax.typesetClear()
    MathJax.texReset()
    MathJax.typesetPromise().then(clLinkEquationSectors)
})
