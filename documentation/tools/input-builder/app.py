"""
CosmoLattice input-file builder.

A lightweight, locally-hosted GUI for generating CosmoLattice `.in` parameter
files from the curated parameter database (`source/data/parameters.yaml`).

Run with:   streamlit run app.py
"""

from __future__ import annotations

from pathlib import Path

import streamlit as st
import streamlit.components.v1 as components
import yaml

# --------------------------------------------------------------------------- #
# Data loading
# --------------------------------------------------------------------------- #

# Canonical emission order of framework categories (see parameters.schema.md).
CATEGORY_ORDER = [
    "run",
    "IC",
    "output",
    "spectra",
    "snapshots",
    "GW",
    "saving",
    "couplings",
    "model",
]

CATEGORY_LABELS = {
    "run": "Run / lattice / time evolution",
    "IC": "Initial conditions",
    "output": "Output",
    "spectra": "Power spectra",
    "snapshots": "Field snapshots",
    "GW": "Gravitational waves",
    "saving": "Checkpointing",
    "couplings": "Couplings",
    "model": "Model parameters",
}

# Snapshot energy/field quantities, grouped by field sector. Each label is one of
# the tokens the C++ EnergySnapshotsMeasurer checks for in the `energy_snapshot`
# list (see include/CosmoInterface/measurements/energysnapshotmeasurer.h). The
# selected labels are emitted as a space-separated `energy_snapshot = ...` line.
SNAPSHOT_SECTORS = [
    ("Scalar singlets", [
        ("S", "Field value"),
        ("E_S_K", "Kinetic energy"),
        ("E_S_G", "Gradient energy"),
    ]),
    ("Complex scalars", [
        ("CS", "Field modulus"),
        ("E_CS_K", "Kinetic energy"),
        ("E_CS_G", "Gradient energy"),
    ]),
    ("SU(2) doublets", [
        ("E_SU2D_K", "Kinetic energy"),
        ("E_SU2D_G", "Gradient energy"),
    ]),
    ("U(1) gauge sector", [
        ("E_A_K", "Electric energy"),
        ("E_A_G", "Magnetic energy"),
    ]),
    ("SU(2) gauge sector", [
        ("E_B_K", "Electric energy"),
        ("E_B_G", "Magnetic energy"),
    ]),
    ("Total / potential", [
        ("E_V", "Potential energy"),
        ("E", "Total energy"),
    ]),
]

# Keeps the live-preview column pinned while the (long) form scrolls. Scoped via
# :has(.cl-preview-anchor) so only the preview column is affected — nested
# columns inside the form are left alone. On narrow viewports Streamlit stacks
# the columns vertically (preview drops below the form); the media query then
# disables sticky so the stacked preview scrolls normally.
PREVIEW_CSS = """
<style>
/* Pin the generated-file column so it stays in view while the (long) form
   scrolls. The preview lives in st.container(key="cl-preview") -> the DOM class
   st-key-cl-preview, used here to single out the correct outer two-column row.
   The sticky element must be the column (whose parent row is full form height,
   giving it room to travel); align-items:flex-start lets that column shrink to
   its content so it actually slides instead of filling the row. */
[data-testid="stHorizontalBlock"]:has(.st-key-cl-preview) {
    align-items: flex-start;
}
[data-testid="stHorizontalBlock"]:has(.st-key-cl-preview) > [data-testid="stColumn"]:has(.st-key-cl-preview) {
    position: sticky;
    top: 3rem;
}
/* Long generated file scrolls inside the pinned panel instead of growing taller
   than the viewport. */
.st-key-cl-preview pre {
    max-height: 78vh;
    overflow: auto;
}
/* Draggable divider between the form and preview columns (created by
   SPLITTER_JS in the parent DOM). */
.cl-splitter {
    position: absolute;
    top: 0;
    bottom: 0;
    width: 11px;
    cursor: col-resize;
    z-index: 5;
}
.cl-splitter::before {
    content: "";
    position: absolute;
    left: 5px;
    top: 0;
    bottom: 0;
    width: 1px;
    background: rgba(49, 51, 63, 0.18);
    transition: background 0.1s, width 0.1s;
}
.cl-splitter:hover::before, .cl-splitter.cl-dragging::before {
    background: rgba(255, 75, 75, 0.7);
    width: 3px;
    left: 4px;
}
@media (max-width: 992px) {
    /* Stack: form on top, generated file below (Streamlit's own auto-stack
       kicks in only at much narrower widths, so force it here). */
    [data-testid="stHorizontalBlock"]:has(.st-key-cl-preview) {
        flex-direction: column;
    }
    [data-testid="stHorizontalBlock"]:has(.st-key-cl-preview) > [data-testid="stColumn"] {
        width: 100% !important;
    }
    [data-testid="stHorizontalBlock"]:has(.st-key-cl-preview) > [data-testid="stColumn"]:has(.st-key-cl-preview) {
        position: static;
    }
    .st-key-cl-preview pre { max-height: none; }
}
</style>
"""

# Injected in a (zero-height) components.html iframe so it can reach the parent
# document — Streamlit's markdown sanitiser strips <script>. It adds a drag
# handle between the two columns, persists the split ratio in localStorage, and
# reapplies it via a MutationObserver so it survives Streamlit reruns. Self-
# healing: if Streamlit replaces the row, the handle is rebuilt on the next tick.
SPLITTER_JS = """
<script>
(function () {
  const KEY = 'cl-split-ratio', DEF = 0.66, MIN = 0.25, MAX = 0.85;
  const doc = window.parent.document, win = window.parent;

  const getRatio = () => {
    const v = parseFloat(win.localStorage.getItem(KEY));
    return (v >= MIN && v <= MAX) ? v : DEF;
  };
  const getRow = () => {
    const a = doc.querySelector('.st-key-cl-preview');
    return a ? a.closest('[data-testid="stHorizontalBlock"]') : null;
  };
  const cols = (row) =>
    [...row.children].filter(c => c.getAttribute('data-testid') === 'stColumn');

  function apply(row) {
    const cs = cols(row);
    if (cs.length < 2) return;
    const handle = row.querySelector('.cl-splitter');
    // Decide from the viewport width (matches the CSS @media breakpoint) rather
    // than reading flex-direction, which Streamlit itself toggles to column at
    // narrow content widths and would otherwise be misread as "stacked".
    const narrow = win.innerWidth <= 992;
    if (narrow) {
      row.style.flexDirection = '';   // let the CSS media query stack them
      cs.forEach(c => { c.style.flex = ''; c.style.width = ''; c.style.minWidth = ''; });
      if (handle) handle.style.display = 'none';
      return;
    }
    // Wide: force side-by-side (override Streamlit's own auto-stacking) and size.
    row.style.flexDirection = 'row';
    row.style.alignItems = 'flex-start';
    const r = getRatio();
    cs[0].style.flex = '0 0 ' + (r * 100) + '%';
    cs[0].style.width = (r * 100) + '%';
    cs[0].style.minWidth = '0';
    cs[1].style.flex = '1 1 0%';
    cs[1].style.width = 'auto';
    cs[1].style.minWidth = '0';
    if (handle) {
      handle.style.display = 'block';
      handle.style.left = 'calc(' + (r * 100) + '% - 5px)';
    }
  }

  function makeHandle(row) {
    const old = row.querySelector('.cl-splitter');
    if (old) old.remove();
    if (getComputedStyle(row).position === 'static') row.style.position = 'relative';
    const h = doc.createElement('div');
    h.className = 'cl-splitter';
    row.appendChild(h);

    let dragging = false;
    h.addEventListener('mousedown', (e) => {
      dragging = true; e.preventDefault();
      doc.body.style.userSelect = 'none';
      h.classList.add('cl-dragging');
    });
    const onMove = (e) => {
      if (!dragging) return;
      const rect = row.getBoundingClientRect();
      let ra = (e.clientX - rect.left) / rect.width;
      ra = Math.max(MIN, Math.min(MAX, ra));
      win.localStorage.setItem(KEY, ra);
      apply(row);
    };
    const onUp = () => {
      dragging = false;
      doc.body.style.userSelect = '';
      h.classList.remove('cl-dragging');
    };
    if (win.__clMove) doc.removeEventListener('mousemove', win.__clMove);
    if (win.__clUp) doc.removeEventListener('mouseup', win.__clUp);
    win.__clMove = onMove; win.__clUp = onUp;
    doc.addEventListener('mousemove', onMove);
    doc.addEventListener('mouseup', onUp);
  }

  function tick() {
    const row = getRow();
    if (!row) return;
    if (!row.querySelector('.cl-splitter')) makeHandle(row);
    apply(row);
  }

  if (win.__clObs) win.__clObs.disconnect();
  const obs = new MutationObserver(() => tick());
  win.__clObs = obs;
  obs.observe(doc.querySelector('[data-testid="stMain"]') || doc.body,
              { childList: true, subtree: true });
  if (win.__clResize) win.removeEventListener('resize', win.__clResize);
  win.__clResize = tick;
  win.addEventListener('resize', tick);

  tick();
  setTimeout(tick, 300);
  setTimeout(tick, 900);
})();
</script>
"""


def find_database() -> Path:
    """Locate parameters.yaml by walking up from this file."""
    here = Path(__file__).resolve()
    for parent in [here.parent, *here.parents]:
        candidate = parent / "source" / "data" / "parameters.yaml"
        if candidate.is_file():
            return candidate
    raise FileNotFoundError(
        "Could not locate source/data/parameters.yaml relative to this script."
    )


@st.cache_data
def load_params() -> list[dict]:
    with find_database().open() as fh:
        return yaml.safe_load(fh)["parameters"]


def models(params: list[dict]) -> list[str]:
    found = {p["scope"].split(":", 1)[1] for p in params if p["scope"].startswith("model:")}
    # lphi4 (the default model) first, the rest alphabetical
    rest = sorted(found - {"lphi4"})
    return (["lphi4"] if "lphi4" in found else []) + rest


def params_for(params: list[dict], model: str) -> list[dict]:
    # Keep the first entry per name: some keys appear more than once in the
    # database (the C++ reads them more than once), but each maps to a single
    # input-file line, so render each only once.
    seen: set[str] = set()
    out: list[dict] = []
    for p in params:
        if p["scope"] != "framework" and p["scope"] != f"model:{model}":
            continue
        if p["name"] in seen:
            continue
        seen.add(p["name"])
        out.append(p)
    return out


def group_by_category(entries: list[dict]) -> dict[str, list[dict]]:
    groups: dict[str, list[dict]] = {}
    for p in entries:
        groups.setdefault(p["category"], []).append(p)
    return {c: groups[c] for c in CATEGORY_ORDER if c in groups}


# --------------------------------------------------------------------------- #
# Widgets
# --------------------------------------------------------------------------- #


def is_vector(p: dict) -> bool:
    return p["arity"] != "1"


def help_text(p: dict) -> str:
    bits = [p["description"] or ""]
    meta = []
    if p["units"] and p["units"] != "n/a":
        meta.append(f"units: {p['units']}")
    if is_vector(p):
        meta.append(f"arity: {p['arity']}")
    if p["default"] is not None:
        meta.append(f"default: `{p['default']}`")
    if meta:
        bits.append("  \n*" + " · ".join(meta) + "*")
    if p["constraints"]:
        bits.append("  \n⚠️ " + p["constraints"])
    return "\n".join(b for b in bits if b)


def default_str(p: dict) -> str:
    """Literal default to prefill a text widget, else ''."""
    d = p["default"]
    return "" if d is None else str(d)


def render_widget(p: dict, key: str):
    """Render the appropriate input widget; returns the value as a string (or None)."""
    name = p["name"]
    label = f"{name}" + (" *" if p["required"] else "")
    help_ = help_text(p)

    # Vectors (arity != 1): one free-text field, space-separated values.
    if is_vector(p):
        placeholder = "space-separated values"
        if p["arity"].isdigit():
            placeholder = f"{p['arity']} space-separated values"
        elif p["default"] is not None:
            placeholder = str(p["default"])
        val = st.text_input(label, value=default_str(p), key=key,
                            help=help_, placeholder=placeholder)
        return val.strip() or None

    t = p["type"]
    if t == "bool":
        default_true = str(p["default"]).lower() == "true"
        val = st.checkbox(label, value=default_true, key=key, help=help_)
        return "true" if val else "false"

    if t == "enum":
        opts = p["enum_values"] or []
        idx = opts.index(p["default"]) if p["default"] in opts else 0
        val = st.selectbox(label, opts, index=idx, key=key, help=help_)
        return val

    # int / float / string -> text input (keeps symbolic defaults like "10*dt" usable)
    placeholder = "" if p["default"] is None else str(p["default"])
    val = st.text_input(label, value="", key=key, help=help_, placeholder=placeholder)
    return val.strip() or None


# --------------------------------------------------------------------------- #
# Snapshots — custom, click-to-select UI
# --------------------------------------------------------------------------- #


def render_snapshots_section(model: str, values: dict[str, str]):
    """Clickable snapshot UI: tick the quantities to save (-> energy_snapshot)
    and optionally restrict the saved region to a sub-volume (-> snap_* coords)."""
    st.header(CATEGORY_LABELS["snapshots"])
    st.caption(
        "Tick the configuration-space quantities to dump as HDF5 snapshots. "
        "Only sectors that actually exist in the selected model produce files — "
        "selecting a quantity for an absent sector is harmless and simply ignored."
    )

    # --- quantity selection -> energy_snapshot ---
    selected: list[str] = []
    cols = st.columns(3)
    for i, (sector, items) in enumerate(SNAPSHOT_SECTORS):
        with cols[i % 3]:
            st.markdown(f"**{sector}**")
            for label, human in items:
                if st.checkbox(human, key=f"snap:{model}:{label}"):
                    selected.append(label)

    values["energy_snapshot"] = " ".join(selected) if selected else None

    if selected:
        st.success("`energy_snapshot = " + " ".join(selected) + "`")

        # --- optional sub-volume restriction -> snap_lowercoord/upper/step ---
        with st.expander("Restrict to a sub-volume (optional)"):
            st.caption(
                "By default the whole lattice is saved. Set per-dimension lower / "
                "upper coordinates (in lattice units) and a stride to save a slice "
                "or coarsened sub-volume. Leave a row at its defaults to keep that "
                "dimension full. `N` is the lattice size you set above."
            )
            restrict = st.checkbox("Restrict snapshot region", key=f"snap:{model}:restrict")
            if restrict:
                ndim = st.radio("Lattice dimensions", [1, 2, 3], index=2,
                                horizontal=True, key=f"snap:{model}:ndim")
                lowers, uppers, steps = [], [], []
                hdr = st.columns([1, 2, 2, 2])
                hdr[1].caption("lower"); hdr[2].caption("upper (blank = N)"); hdr[3].caption("step")
                for d in range(ndim):
                    c = st.columns([1, 2, 2, 2])
                    c[0].markdown(f"dim {d + 1}")
                    lo = c[1].number_input("lo", min_value=0, value=0, step=1,
                                           key=f"snap:{model}:lo{d}", label_visibility="collapsed")
                    up = c[2].text_input("up", value="", placeholder="N",
                                         key=f"snap:{model}:up{d}", label_visibility="collapsed")
                    sp = c[3].number_input("sp", min_value=1, value=1, step=1,
                                           key=f"snap:{model}:sp{d}", label_visibility="collapsed")
                    n_val = (values.get("N") or "N")
                    lowers.append(str(lo))
                    uppers.append(up.strip() or n_val)
                    steps.append(str(sp))
                values["snap_lowercoord"] = " ".join(lowers)
                values["snap_uppercoord"] = " ".join(uppers)
                values["snap_stepcoord"] = " ".join(steps)
    else:
        st.info("No snapshot quantities selected — no snapshot files will be written.")


# --------------------------------------------------------------------------- #
# Output generation
# --------------------------------------------------------------------------- #


def build_input_file(model: str, values: dict[str, str], grouped: dict[str, list[dict]]) -> str:
    lines = [f"# CosmoLattice input file  (model: {model})", ""]
    for cat, entries in grouped.items():
        emitted = [(p["name"], values[p["name"]]) for p in entries
                   if values.get(p["name"]) is not None]
        if not emitted:
            continue
        lines.append(f"#{CATEGORY_LABELS.get(cat, cat)}")
        for name, val in emitted:
            lines.append(f"{name} = {val}")
        lines.append("")
    return "\n".join(lines).rstrip() + "\n"


# --------------------------------------------------------------------------- #
# App
# --------------------------------------------------------------------------- #


def main():
    st.set_page_config(page_title="CosmoLattice input builder", layout="wide")
    st.markdown(PREVIEW_CSS, unsafe_allow_html=True)
    components.html(SPLITTER_JS, height=0)
    st.title("CosmoLattice input-file builder")

    all_params = load_params()
    model = st.sidebar.selectbox("Model", models(all_params))
    st.sidebar.caption(
        "Required fields are marked with **\\***. "
        "Required + commonly-tuned parameters are shown directly; "
        "the rest live under **Advanced** sections."
    )

    entries = params_for(all_params, model)
    grouped = group_by_category(entries)

    values: dict[str, str] = {}
    missing_required: list[str] = []

    # Form on the left, live-generated file on the right. Streamlit runs the
    # whole script top-to-bottom on every interaction, so the form widgets in
    # the left column return their current values before we build the preview
    # in the right column. On narrow screens the columns stack (preview drops
    # below the form) — see PREVIEW_CSS.
    form_col, preview_col = st.columns([2, 1], gap="large")

    with form_col:
        for cat, cat_entries in grouped.items():
            if cat == "snapshots":
                render_snapshots_section(model, values)
                continue

            primary = [p for p in cat_entries if p["required"] or p["important"]]
            advanced = [p for p in cat_entries if not (p["required"] or p["important"])]

            if not primary and not advanced:
                continue

            st.header(CATEGORY_LABELS.get(cat, cat))

            for p in primary:
                val = render_widget(p, key=f"{model}:{p['name']}")
                values[p["name"]] = val
                if p["required"] and val is None:
                    missing_required.append(p["name"])

            if advanced:
                with st.expander(f"Advanced — {len(advanced)} optional parameter(s)"):
                    for p in advanced:
                        col_inc, col_w = st.columns([1, 6])
                        include = col_inc.checkbox("set", key=f"inc:{model}:{p['name']}")
                        with col_w:
                            if include:
                                values[p["name"]] = render_widget(p, key=f"{model}:{p['name']}")
                            else:
                                st.markdown(f"**{p['name']}** — *unset (uses default)*")
                                st.caption(help_text(p))

    with preview_col:
        # key="cl-preview" -> DOM class st-key-cl-preview, which PREVIEW_CSS
        # makes sticky so the generated file stays in view while scrolling.
        with st.container(key="cl-preview"):
            st.subheader("Generated input file")

            text = build_input_file(model, values, grouped)
            st.download_button(
                "⬇ Download .in file",
                data=text,
                file_name=f"{model}.in",
                mime="text/plain",
                disabled=bool(missing_required),
                use_container_width=True,
            )
            if missing_required:
                st.warning("Missing required: " + ", ".join(f"`{n}`" for n in missing_required))
            st.code(text, language="ini")


if __name__ == "__main__":
    main()
