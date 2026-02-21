import sys
import regex as re

table_counter = 1

# ── Recursive brace pattern (for regex module) ──────────────────────────
# Matches { ... } allowing nested braces
_BRACE = r"\{(?:[^{}]|\{(?:[^{}]|\{[^{}]*\})*\})*\}"

# ── LaTeX formatting → Markdown ──────────────────────────────────────────

_FORMAT_RULES = [
    # bold + italic combos
    (re.compile(r"\{\s*\\it\s+\\bf\s+(.*?)\s*\}", re.DOTALL), r"***\1***"),
    (re.compile(r"\{\s*\\bf\s+\\it\s+(.*?)\s*\}", re.DOTALL), r"***\1***"),
    # bold
    (re.compile(r"\{\s*\\bf\s+(.*?)\s*\}", re.DOTALL), r"**\1**"),
    (re.compile(r"\\textbf\{(.*?)\}", re.DOTALL), r"**\1**"),
    # italic
    (re.compile(r"\{\s*\\it\s+(.*?)\s*\}", re.DOTALL), r"*\1*"),
    (re.compile(r"\\textit\{(.*?)\}", re.DOTALL), r"*\1*"),
    (re.compile(r"\\emph\{(.*?)\}", re.DOTALL), r"*\1*"),
    # monospace: {\tt word} and \texttt{word}
    (re.compile(r"\{\s*\\tt\s+(.*?)\s*\}", re.DOTALL), r"`\1`"),
    (re.compile(r"\\texttt\{(.*?)\}", re.DOTALL), r"`\1`"),
    # bare \tt word  (no braces, consumes up to next whitespace/& or end)
    (re.compile(r"\\tt\s+(\S+)"), r"`\1`"),
    # href
    (re.compile(r"\\href\{(.+?)\}\{(.+?)\}"), r"[\2](\1)"),
]

# Things to simply strip from table cell text
_STRIP_RULES = [
    (re.compile(r"\\hline"), ""),
    (re.compile(r"\\cline\{[^}]*\}"), ""),
    (re.compile(r"\\normalfont"), ""),
    (re.compile(r"\\small"), ""),
    (re.compile(r"\\footnotesize"), ""),
    (re.compile(r"\\tiny\{(.*?)\}", re.DOTALL), r"\1"),
    (re.compile(r"\\tiny"), ""),
    (re.compile(r"\\color\{[^}]*\}"), ""),
    (re.compile(r"\\noindent"), ""),
    (re.compile(r"\\vspace\*?\{[^}]*\}"), ""),
    (re.compile(r"\\hspace\*?\{[^}]*\}"), ""),
    (re.compile(r"\\label\{[^}]*\}"), ""),
    (re.compile(r"\\centering"), ""),
    (re.compile(r"\\caption\{[^}]*\}"), ""),
    (re.compile(r"\\newline"), " "),
    (re.compile(r"~"), " "),
    (re.compile(r"\\,"), " "),
    (re.compile(r"\\_"), "_"),
]


def _clean_cell(text: str) -> str:
    """Apply formatting and stripping rules to a single cell's text."""

    # Handle any remaining \multicolumn{N}{align}{content} → content
    def _multicol_repl(m):
        return _strip_outer_braces(m.group(3))

    text = _MULTICOL_RE.sub(_multicol_repl, text)

    # Strip rules first
    for pat, repl in _STRIP_RULES:
        text = pat.sub(repl, text)

    # Apply formatting rules iteratively until stable
    for _ in range(5):
        old = text
        for pat, repl in _FORMAT_RULES:
            text = pat.sub(repl, text)
        if text == old:
            break

    # Collapse whitespace
    text = re.sub(r"\s+", " ", text).strip()
    return text


# ── Parse column spec ────────────────────────────────────────────────────


def _parse_num_cols(colspec: str) -> int:
    """
    Given a LaTeX column specification like '|c|c|X|X|' or '| m{3.7cm} | m{12.8cm}|',
    return the number of data columns.
    """
    # Remove nested brace groups (column width specs like {3.7cm})
    cleaned = re.sub(r"\{[^}]*\}", "", colspec)
    # Remove >{ ... } prefixes (like >{\tt\small}) — already stripped above
    cleaned = re.sub(r">", "", cleaned)
    # Count column type letters: c, l, r, m, p, X
    cols = re.findall(r"[clrmpX]", cleaned)
    return len(cols)


# ── Parse the environment header including nested-brace column specs ─────


def _find_colspec(block_content: str) -> str | None:
    """
    Extract the column specification string from a \\begin{tabular/tabularx/longtable} header.
    Handles nested braces such as m{3.7cm}.
    """
    # Pattern: \begin{env} possibly {extra arg} then {colspec}
    # We use the recursive-brace pattern to correctly match.
    pat = (
        r"\\begin\{(?:tabular|tabularx|longtable)\}\s*"
        r"(?:" + _BRACE + r"\s*)?"  # optional first arg (e.g. \textwidth)
        r"(" + _BRACE + r")"  # capture the column spec (including outer braces)
    )
    m = re.search(pat, block_content)
    if m:
        # Strip the outer braces from the captured group
        spec = m.group(1)
        if spec.startswith("{") and spec.endswith("}"):
            spec = spec[1:-1]
        return spec
    return None


# ── Parse table rows ─────────────────────────────────────────────────────


def _split_rows(body: str) -> list[str]:
    """Split the table body on \\\\ (LaTeX row terminator)."""
    return re.split(r"\\\\", body)


def _split_cells(row: str) -> list[str]:
    """Split a row on & (the column separator), respecting braces and $."""
    cells = []
    depth_brace = 0
    depth_dollar = 0
    current: list[str] = []
    i = 0
    s = row
    while i < len(s):
        ch = s[i]
        if ch == "\\" and i + 1 < len(s):
            current.append(s[i : i + 2])
            i += 2
            continue
        if ch == "{":
            depth_brace += 1
        elif ch == "}":
            depth_brace -= 1
        elif ch == "$":
            depth_dollar = 1 - depth_dollar
        elif ch == "&" and depth_brace == 0 and depth_dollar == 0:
            cells.append("".join(current))
            current = []
            i += 1
            continue
        current.append(ch)
        i += 1
    cells.append("".join(current))
    return cells


# ── Handle \multicolumn ──────────────────────────────────────────────────

# Pattern that handles nested braces inside the alignment and content args
_MULTICOL_RE = re.compile(
    r"\\multicolumn\{(\d+)\}"  # {span}
    r"(" + _BRACE + r")"  # {alignment} (may contain nested braces)
    r"(" + _BRACE + r")",  # {content}   (may contain nested braces)
    re.DOTALL,
)


def _strip_outer_braces(s: str) -> str:
    """Remove one layer of outer braces if present."""
    s = s.strip()
    if s.startswith("{") and s.endswith("}"):
        return s[1:-1]
    return s


def _expand_multicolumn(cells: list[str], num_cols: int) -> list[str]:
    """
    If a cell contains \\multicolumn{N}{...}{content}, expand it so the
    returned list has exactly num_cols entries.
    """
    expanded: list[str] = []
    for cell in cells:
        m = _MULTICOL_RE.match(cell.strip())
        if m:
            span = int(m.group(1))
            content = _strip_outer_braces(m.group(3))
            expanded.append(content)
            for _ in range(span - 1):
                expanded.append("")
        else:
            expanded.append(cell)
    return expanded


# ── Build Markdown table ─────────────────────────────────────────────────


def _rows_to_markdown(rows: list[list[str]], num_cols: int) -> str:
    """Produce a Markdown table string from a list of cell-string rows."""
    if not rows:
        return ""

    # Pad / trim every row to num_cols
    for i, row in enumerate(rows):
        if len(row) < num_cols:
            row.extend([""] * (num_cols - len(row)))
        elif len(row) > num_cols:
            rows[i] = row[:num_cols]

    # Column widths (minimum 3 for separator dashes)
    widths = [3] * num_cols
    for row in rows:
        for j, cell in enumerate(row):
            widths[j] = max(widths[j], len(cell))

    lines: list[str] = []
    for idx, row in enumerate(rows):
        line = (
            "| "
            + " | ".join(cell.ljust(widths[j]) for j, cell in enumerate(row))
            + " |"
        )
        lines.append(line)
        if idx == 0:
            sep = "| " + " | ".join("-" * widths[j] for j in range(num_cols)) + " |"
            lines.append(sep)

    return "\n".join(lines)


# ── Main entry point ─────────────────────────────────────────────────────


def _extract_label(block_content: str) -> tuple[str | None, dict[str, str]]:
    """Extract \\label{...} from a block, return (label_name, label_dict)."""
    global table_counter
    label_dict: dict[str, str] = {}
    m = re.search(r"\\label\{(.+?)\}", block_content)
    if m:
        label_name = m.group(1)
        label_dict[label_name] = f"{table_counter}"
        return label_name, label_dict
    return None, label_dict


def table_block(block_content: str, tmpdir: str = "./") -> tuple[str, dict]:
    """
    Convert a LaTeX table block (tabular / tabularx / longtable)
    into a Markdown table.
    """
    global table_counter
    print(f"Processing table block {table_counter}...", file=sys.stderr)

    # Extract label before processing
    label_name, label_dict = _extract_label(block_content)

    # ── Determine number of columns ──────────────────────────────────
    colspec = _find_colspec(block_content)
    if colspec is not None:
        num_cols = _parse_num_cols(colspec)
    else:
        # fallback: count maximum & in any row + 1
        num_cols = (
            max(
                (line.count("&") for line in block_content.split("\\\\")),
                default=1,
            )
            + 1
        )

    # ── Strip the environment wrapper ────────────────────────────────
    # Remove \begin{env}{...}{...} including nested-brace arguments
    body = re.sub(
        r"\\begin\{(?:tabular|tabularx|longtable)\}\s*(?:" + _BRACE + r"\s*)*",
        "",
        block_content,
    )
    body = re.sub(r"\\end\{(?:tabular|tabularx|longtable)\}", "", body)
    body = re.sub(r"\\begin\{table\}(?:\[[^\]]*\])?", "", body)
    body = re.sub(r"\\end\{table\}", "", body)

    # Remove \caption{...} and \label{...}
    body = re.sub(r"\\caption\{[^}]*\}", "", body)
    body = re.sub(r"\\label\{[^}]*\}", "", body)
    body = re.sub(r"\\centering", "", body)

    # ── Split into rows and cells ────────────────────────────────────
    raw_rows = _split_rows(body)
    table_rows: list[list[str]] = []

    for raw_row in raw_rows:
        # Strip \hline and surrounding whitespace
        row_text = re.sub(r"\\hline", "", raw_row).strip()
        if not row_text:
            continue
        # Split into cells
        cells = _split_cells(row_text)
        # Handle \multicolumn
        cells = _expand_multicolumn(cells, num_cols)
        # Clean each cell
        cleaned = [_clean_cell(c) for c in cells]
        # Skip rows where every cell is empty
        if all(c == "" for c in cleaned):
            continue
        table_rows.append(cleaned)

    if not table_rows:
        table_counter += 1
        return "", label_dict

    md_table = _rows_to_markdown(table_rows, num_cols)

    # ── Wrap with label anchor if needed ─────────────────────────────
    if label_name:
        result = "\n[](){ #" + label_name.replace(":", "_") + " }\n\n" + md_table + "\n"
    else:
        result = "\n" + md_table + "\n"

    table_counter += 1
    return result, label_dict
