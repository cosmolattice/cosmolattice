#!/usr/bin/env python3
"""
Run clang-tidy on CosmoLattice source files and produce structured JSON output.

Usage:
    python3 tools/run_clang_tidy.py [options]

Examples:
    # Run performance checks on all project files (4 parallel jobs)
    python3 tools/run_clang_tidy.py -p performance

    # Run on only non-test source files with all check profiles
    python3 tools/run_clang_tidy.py -c src -p all

    # Run specific checks on test files, 8 jobs
    python3 tools/run_clang_tidy.py -c tests -p performance,modernize -j 8

    # Dry run: just list which files would be analyzed
    python3 tools/run_clang_tidy.py --dry-run
"""

import argparse
import json
import os
import re
import subprocess
import sys
import time
from concurrent.futures import ProcessPoolExecutor, as_completed
from dataclasses import dataclass, field, asdict
from pathlib import Path
from typing import Optional

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

CLANG_TIDY = os.environ.get(
    "CLANG_TIDY", "/opt/homebrew/opt/llvm/bin/clang-tidy"
)

# Check profiles — curated sets of checks relevant to a Kokkos/GPU C++20 codebase.
CHECK_PROFILES: dict[str, str] = {
    "performance": ",".join([
        "performance-*",
        "bugprone-unused-return-value",
        "misc-unused-using-decls",
    ]),
    "modernize": ",".join([
        "modernize-*",
        "-modernize-use-trailing-return-type",       # noisy, style preference
        "-modernize-avoid-c-arrays",                 # sometimes needed for Kokkos
        "-modernize-use-nodiscard",                  # too much churn for little gain
    ]),
    "bugprone": ",".join([
        "bugprone-*",
        "-bugprone-easily-swappable-parameters",     # very noisy in math code
    ]),
    "readability": ",".join([
        "readability-*",
        "-readability-magic-numbers",                # physics code uses constants
        "-readability-identifier-length",            # i, j, k are fine
        "-readability-braces-around-statements",     # pure style preference
        "-readability-redundant-inline-specifier",   # too much churn for little gain
        "-readability-named-parameter",              # deliberate unnamed params in interfaces/tag-dispatch
    ]),
    "cppcoreguidelines": ",".join([
        "cppcoreguidelines-*",
        "-cppcoreguidelines-avoid-magic-numbers",
        "-cppcoreguidelines-avoid-const-or-ref-data-members",   # deliberate design choice
        "-cppcoreguidelines-non-private-member-variables-in-classes",  # intentional in inheritance
        "-cppcoreguidelines-pro-bounds-array-to-pointer-decay",  # C-API interop (HDF5, MPI)
        "-cppcoreguidelines-pro-bounds-pointer-arithmetic",      # intentional in memory management
        "-cppcoreguidelines-pro-type-cstyle-cast",               # required by MPI C-API
        "-cppcoreguidelines-macro-usage",                        # macros are part of model-definition DSL
        "-cppcoreguidelines-owning-memory",                      # GPU/Kokkos device allocation
        "-cppcoreguidelines-avoid-non-const-global-variables",   # false positive on constexpr inline vars
        "-cppcoreguidelines-c-copy-assignment-signature",        # expr-template operator= returns void by design
    ]),
}

# Files to exclude from diagnostics (third-party / vendored code).
SUPPRESSED_FILES: list[str] = [
    "libkeccak.h",
]

# Header filter: only report diagnostics from project headers, not system/deps.
HEADER_FILTER = r".*/src/(include|tests|benchmarks|models)/.*"

# ---------------------------------------------------------------------------
# Data model
# ---------------------------------------------------------------------------

@dataclass
class Diagnostic:
    file: str           # absolute path where the diagnostic was reported
    line: int
    column: int
    severity: str       # warning, error, note
    message: str
    check: str          # clang-tidy check name, e.g. "performance-unnecessary-copy-initialization"
    source_file: str    # the .cpp translation unit that triggered the analysis

    @property
    def rel_file(self) -> str:
        """File path relative to project root."""
        idx = self.file.find("cosmolattice_GPU_impl/")
        if idx != -1:
            return self.file[idx + len("cosmolattice_GPU_impl/"):]
        return self.file

    @property
    def rel_source(self) -> str:
        idx = self.source_file.find("cosmolattice_GPU_impl/")
        if idx != -1:
            return self.source_file[idx + len("cosmolattice_GPU_impl/"):]
        return self.source_file


@dataclass
class FileResult:
    file: str
    diagnostics: list[Diagnostic] = field(default_factory=list)
    elapsed_s: float = 0.0
    error: Optional[str] = None


@dataclass
class RunResult:
    clang_tidy_version: str
    checks: str
    profiles_used: list[str]
    category: str
    total_files: int
    files_with_diagnostics: int
    total_diagnostics: int
    elapsed_s: float
    file_results: list[FileResult] = field(default_factory=list)

# ---------------------------------------------------------------------------
# Parsing
# ---------------------------------------------------------------------------

# Matches lines like:
#   /path/to/file.h:123:45: warning: some message [check-name]
DIAG_RE = re.compile(
    r"^(?P<file>.+?):(?P<line>\d+):(?P<col>\d+): "
    r"(?P<sev>warning|error|note): "
    r"(?P<msg>.+?) \[(?P<check>[^\]]+)\]$"
)


def parse_clang_tidy_output(output: str, source_file: str) -> list[Diagnostic]:
    """Parse clang-tidy stdout into Diagnostic objects."""
    diagnostics = []
    for line in output.splitlines():
        m = DIAG_RE.match(line)
        if m:
            diagnostics.append(Diagnostic(
                file=m.group("file"),
                line=int(m.group("line")),
                column=int(m.group("col")),
                severity=m.group("sev"),
                message=m.group("msg"),
                check=m.group("check"),
                source_file=source_file,
            ))
    return diagnostics

# ---------------------------------------------------------------------------
# Running clang-tidy on a single file
# ---------------------------------------------------------------------------

def run_on_file(
    filepath: str,
    build_dir: str,
    checks: str,
    header_filter: str,
    extra_args: list[str] | None = None,
) -> FileResult:
    """Run clang-tidy on a single translation unit. Returns a FileResult."""
    cmd = [
        CLANG_TIDY,
        f"-checks=-*,{checks}",
        f"-header-filter={header_filter}",
        f"-p={build_dir}",
    ]
    if extra_args:
        cmd.extend(extra_args)
    cmd.append(filepath)

    t0 = time.monotonic()
    try:
        proc = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=600,  # 10 min per file max
        )
        elapsed = time.monotonic() - t0
        # Parse diagnostics from both stdout and stderr (clang-tidy may emit to either)
        combined = proc.stdout + "\n" + proc.stderr
        diagnostics = parse_clang_tidy_output(combined, filepath)
        # Only report error if there are no diagnostics AND return code is non-zero
        # (clang-tidy often crashes after producing valid output on complex template code)
        error = None
        if proc.returncode != 0 and not diagnostics:
            error = proc.stderr.strip()[:200] if proc.stderr else f"exit code {proc.returncode}"
        return FileResult(
            file=filepath,
            diagnostics=diagnostics,
            elapsed_s=round(elapsed, 2),
            error=error,
        )
    except subprocess.TimeoutExpired:
        return FileResult(
            file=filepath,
            elapsed_s=round(time.monotonic() - t0, 2),
            error="TIMEOUT (600s)",
        )
    except Exception as e:
        return FileResult(
            file=filepath,
            elapsed_s=round(time.monotonic() - t0, 2),
            error=str(e),
        )

# ---------------------------------------------------------------------------
# File selection
# ---------------------------------------------------------------------------

def get_project_files(compile_db_path: str, category: str) -> list[str]:
    """
    Read compile_commands.json and return deduplicated project source files.
    category: 'all', 'src', 'tests', 'benchmarks'
    """
    with open(compile_db_path) as f:
        db = json.load(f)

    files = sorted({
        e["file"] for e in db
        if "_deps/" not in e["file"]
    })

    if category == "all":
        return files
    elif category == "src":
        return [f for f in files if "/tests/" not in f and "/benchmarks/" not in f]
    elif category == "tests":
        return [f for f in files if "/tests/" in f]
    elif category == "benchmarks":
        return [f for f in files if "/benchmarks/" in f]
    else:
        raise ValueError(f"Unknown category: {category}")

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def resolve_profiles(profile_str: str) -> list[tuple[str, str]]:
    """Resolve a comma-separated list of profile names (or 'all') into
    a list of (profile_name, checks_string) pairs.

    Each profile is kept separate so clang-tidy can be invoked once per profile
    per file — combining too many checks in a single invocation crashes clang-tidy
    on complex template-heavy codebases.
    """
    if profile_str == "all":
        names = list(CHECK_PROFILES.keys())
    else:
        names = [n.strip() for n in profile_str.split(",")]

    result = []
    for name in names:
        if name in CHECK_PROFILES:
            result.append((name, CHECK_PROFILES[name]))
        else:
            # Treat as a raw clang-tidy check pattern
            result.append((name, name))

    return result


def main():
    parser = argparse.ArgumentParser(
        description="Run clang-tidy on CosmoLattice and produce structured JSON output.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    parser.add_argument(
        "-b", "--build-dir",
        default="debug_build",
        help="Path to the build directory containing compile_commands.json (default: debug_build)",
    )
    parser.add_argument(
        "-p", "--profiles",
        default="performance",
        help=(
            "Comma-separated check profiles to enable. "
            f"Available: {', '.join(CHECK_PROFILES.keys())}, all. "
            "You can also pass raw clang-tidy check patterns. (default: performance)"
        ),
    )
    parser.add_argument(
        "-c", "--category",
        default="all",
        choices=["all", "src", "tests", "benchmarks"],
        help="Which source files to analyze (default: all)",
    )
    parser.add_argument(
        "-j", "--jobs",
        type=int,
        default=4,
        help="Number of parallel clang-tidy processes (default: 4)",
    )
    parser.add_argument(
        "-o", "--output",
        default=None,
        help="Output JSON file path (default: clang_tidy_results_<profiles>_<category>.json)",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="List files that would be analyzed, then exit.",
    )
    parser.add_argument(
        "--header-filter",
        default=HEADER_FILTER,
        help=f"Regex for headers to include in diagnostics (default: {HEADER_FILTER})",
    )
    parser.add_argument(
        "--extra-args",
        nargs="*",
        default=None,
        help="Extra arguments to pass to clang-tidy",
    )

    args = parser.parse_args()

    # Resolve paths
    project_root = Path(__file__).resolve().parent.parent
    build_dir = (project_root / args.build_dir).resolve()
    compile_db = build_dir / "compile_commands.json"

    if not compile_db.exists():
        print(f"Error: {compile_db} not found. Build the project first.", file=sys.stderr)
        sys.exit(1)

    # Resolve profiles (each kept separate to avoid clang-tidy crashes)
    profiles = resolve_profiles(args.profiles)
    profile_names = [p[0] for p in profiles]

    # Get files
    files = get_project_files(str(compile_db), args.category)

    if args.dry_run:
        print(f"Profiles: {profile_names}")
        for name, checks in profiles:
            print(f"  {name}: {checks}")
        print(f"Category: {args.category}")
        print(f"Files ({len(files)}):")
        for f in files:
            idx = f.find("cosmolattice_GPU_impl/")
            rel = f[idx + len("cosmolattice_GPU_impl/"):] if idx != -1 else f
            print(f"  {rel}")
        total_runs = len(files) * len(profiles)
        print(f"\nTotal clang-tidy invocations: {total_runs} ({len(files)} files x {len(profiles)} profiles)")
        return

    # Verify clang-tidy exists
    try:
        ver = subprocess.run(
            [CLANG_TIDY, "--version"], capture_output=True, text=True
        )
        version_str = ver.stdout.strip().splitlines()[0] if ver.stdout else "unknown"
    except FileNotFoundError:
        print(f"Error: clang-tidy not found at {CLANG_TIDY}", file=sys.stderr)
        print("Set CLANG_TIDY env var or install llvm.", file=sys.stderr)
        sys.exit(1)

    # Output path
    if args.output:
        out_path = Path(args.output)
    else:
        profile_tag = "_".join(profile_names)
        out_path = project_root / f"clang_tidy_results_{profile_tag}_{args.category}.json"

    total_runs = len(files) * len(profiles)
    print(f"clang-tidy: {version_str}")
    print(f"Profiles:   {profile_names} (run as separate passes to avoid crashes)")
    print(f"Category:   {args.category}")
    print(f"Files:      {len(files)}")
    print(f"Passes:     {total_runs} ({len(files)} files x {len(profiles)} profiles)")
    print(f"Jobs:       {args.jobs}")
    print(f"Output:     {out_path}")
    print()

    # Build work items: (file, profile_name, checks)
    work_items = [
        (f, pname, pchecks)
        for f in files
        for pname, pchecks in profiles
    ]

    t0 = time.monotonic()
    # Collect results keyed by file path
    file_diagnostics: dict[str, list[Diagnostic]] = {f: [] for f in files}
    file_errors: dict[str, list[str]] = {f: [] for f in files}
    file_elapsed: dict[str, float] = {f: 0.0 for f in files}
    completed = 0

    with ProcessPoolExecutor(max_workers=args.jobs) as pool:
        futures = {
            pool.submit(
                run_on_file, f, str(build_dir), checks, args.header_filter, args.extra_args
            ): (f, pname)
            for f, pname, checks in work_items
        }
        for future in as_completed(futures):
            completed += 1
            filepath, pname = futures[future]
            result = future.result()
            file_diagnostics[filepath].extend(result.diagnostics)
            file_elapsed[filepath] += result.elapsed_s
            if result.error:
                file_errors[filepath].append(f"[{pname}] {result.error}")

            n_diag = len(result.diagnostics)
            rel = filepath
            idx = rel.find("cosmolattice_GPU_impl/")
            if idx != -1:
                rel = rel[idx + len("cosmolattice_GPU_impl/"):]
            status = f"{n_diag} diagnostics" if n_diag else "clean"
            if result.error:
                status = f"ERROR: {result.error[:50]}"
            print(f"  [{completed}/{total_runs}] {pname}: {rel} — {status} ({result.elapsed_s}s)")

    total_elapsed = round(time.monotonic() - t0, 2)

    # Deduplicate diagnostics (same file+line+col+check from different passes)
    # and filter out diagnostics from suppressed files.
    def dedup_key(d: Diagnostic) -> tuple:
        return (d.file, d.line, d.column, d.check)

    def is_suppressed_file(d: Diagnostic) -> bool:
        return any(s in d.file for s in SUPPRESSED_FILES)

    file_results: list[FileResult] = []
    suppressed_count = 0
    for f in files:
        seen = set()
        unique_diags = []
        for d in file_diagnostics[f]:
            if is_suppressed_file(d):
                suppressed_count += 1
                continue
            k = dedup_key(d)
            if k not in seen:
                seen.add(k)
                unique_diags.append(d)
        errors = file_errors[f]
        file_results.append(FileResult(
            file=f,
            diagnostics=unique_diags,
            elapsed_s=round(file_elapsed[f], 2),
            error="; ".join(errors) if errors else None,
        ))

    # Collect all diagnostics for summary
    all_diags = [d for r in file_results for d in r.diagnostics]
    files_with_diag = sum(1 for r in file_results if r.diagnostics)

    all_checks = ", ".join(f"{name}: {checks}" for name, checks in profiles)
    run_result = RunResult(
        clang_tidy_version=version_str,
        checks=all_checks,
        profiles_used=profile_names,
        category=args.category,
        total_files=len(files),
        files_with_diagnostics=files_with_diag,
        total_diagnostics=len(all_diags),
        elapsed_s=total_elapsed,
        file_results=file_results,
    )

    # Serialize to JSON
    def to_dict(obj):
        if hasattr(obj, "__dataclass_fields__"):
            return {k: to_dict(v) for k, v in asdict(obj).items()}
        if isinstance(obj, list):
            return [to_dict(x) for x in obj]
        return obj

    with open(out_path, "w") as f:
        json.dump(to_dict(run_result), f, indent=2)

    # Print summary
    print()
    print("=" * 60)
    print(f"SUMMARY")
    print(f"  Total files analyzed:     {len(files)}")
    print(f"  Files with diagnostics:   {files_with_diag}")
    print(f"  Total diagnostics:        {len(all_diags)}")
    if suppressed_count:
        print(f"  Suppressed (file filter): {suppressed_count}")
    print(f"  Total time:               {total_elapsed}s")
    print()

    # Breakdown by check name
    if all_diags:
        from collections import Counter
        check_counts = Counter(d.check for d in all_diags)
        print("  Top checks:")
        for check, count in check_counts.most_common(20):
            print(f"    {count:5d}  {check}")
        print()

        # Breakdown by profile
        profile_counts = Counter()
        for d in all_diags:
            for pname, pchecks in profiles:
                # Match diagnostic check name against the profile's patterns
                if any(d.check.startswith(pat.rstrip("*"))
                       for pat in pchecks.split(",") if not pat.startswith("-")):
                    profile_counts[pname] += 1
                    break
        print("  By profile:")
        for pname, count in profile_counts.most_common():
            print(f"    {count:5d}  {pname}")
        print()

        # Breakdown by file (top 10)
        file_counts = Counter(d.rel_file for d in all_diags)
        print("  Top files:")
        for filepath, count in file_counts.most_common(10):
            print(f"    {count:5d}  {filepath}")

    print()
    print(f"Full results written to: {out_path}")


if __name__ == "__main__":
    main()
