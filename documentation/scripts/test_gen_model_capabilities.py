#!/usr/bin/env python3
"""Tests for gen_model_capabilities.py.

Runnable directly (``python documentation/scripts/test_gen_model_capabilities.py``)
or via pytest. They verify the ModelPars -> capability extraction, the
commented-out-typedef handling, and that the committed parameters.yaml copies
are in sync with the headers.
"""

import sys
import tempfile
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(SCRIPT_DIR))

import gen_model_capabilities as g  # noqa: E402  (after sys.path tweak)


def _parse_str(raw):
    """Parse a synthetic header string via a temp file -> (name, caps)."""
    with tempfile.TemporaryDirectory() as d:
        p = Path(d) / "fake.h"
        p.write_text(raw, encoding="utf-8")
        return g.parse_header(p)


def test_all_models_extracted_lphi4_first():
    caps = g.extract_capabilities()
    assert len(caps) == 18, f"expected 18 models, got {len(caps)}"
    assert next(iter(caps)) == "lphi4", "lphi4 should be emitted first"


def test_capability_matrix_spot_checks():
    caps = g.extract_capabilities()
    # axion model: axion-U(1) coupling active, one scalar, one U(1) field
    assert caps["U1Axion"]["couplings"] == {"axionU1": True}
    assert caps["U1Axion"]["NScalars"] == 1 and caps["U1Axion"]["NU1Flds"] == 1
    assert caps["m2phi2_axionU1"]["couplings"] == {"axionU1": True}
    # full gauge model: three couplings active
    assert caps["lphi4SU2U1"]["couplings"] == {
        "csU1": True, "su2dbU1": True, "su2dbSU2": True}
    # non-minimal models
    assert caps["lphi4NonMinimal"]["couplings"] == {"nonMinimal": True}
    assert caps["NMC_tanh4_w_mass_P"]["couplings"] == {"nonMinimal": True}
    # SU(2) gauge model
    assert caps["SU2"]["couplings"] == {"su2dbSU2": True}
    # plain models: no couplings
    assert caps["lphi4"]["couplings"] == {}
    assert caps["scalar"]["couplings"] == {}
    # 2D model
    assert caps["ON_2D"]["NDim"] == 2
    # SU2Doub has a doublet but all coupling typedefs are commented out
    assert caps["SU2Doub"]["NSU2Doublet"] == 1
    assert caps["SU2Doub"]["couplings"] == {}


def test_active_typedef_detected():
    name, caps = _parse_str(
        "#define MODELNAME fake\n"
        "struct ModelPars : public TempLat::DefaultModelPars {\n"
        "  static constexpr size_t NScalars = 1;\n"
        "  static constexpr size_t NU1Flds = 1;\n"
        "  typedef TempLat::CouplingsManager<NScalars, NU1Flds, true> ScalarU1AxionCouplings;\n"
        "};\n"
    )
    assert name == "fake"
    assert caps["couplings"] == {"axionU1": True}


def test_using_syntax_detected():
    _, caps = _parse_str(
        "#define MODELNAME fake\n"
        "struct ModelPars : public TempLat::DefaultModelPars {\n"
        "  static constexpr size_t NCScalars = 1;\n"
        "  static constexpr size_t NU1Flds = 1;\n"
        "  using CsU1Couplings = TempLat::CouplingsManager<NCScalars, NU1Flds, true>;\n"
        "};\n"
    )
    assert caps["couplings"] == {"csU1": True}


def test_commented_typedef_is_inactive():
    """A coupling typedef inside a comment must not count as active."""
    _, caps = _parse_str(
        "#define MODELNAME fake\n"
        "struct ModelPars : public TempLat::DefaultModelPars {\n"
        "  static constexpr size_t NSU2Doublet = 1;\n"
        "  // typedef TempLat::CouplingsManager<NSU2Doublet, NSU2Flds, true> SU2DoubletSU2Couplings;\n"
        "  /* typedef TempLat::CouplingsManager<NCScalars, NU1Flds, true> CsU1Couplings; */\n"
        "};\n"
    )
    assert caps["couplings"] == {}


def test_inactive_default_args_not_counted():
    """A CouplingsManager with no boolean args (the default form) is inactive."""
    assert g._coupling_is_active("NScalars, NU1Flds") is False
    assert g._coupling_is_active("NScalars, 1, false, true") is True
    assert g._coupling_is_active("NScalars, NU1Flds, true") is True


def test_defaults_applied_for_absent_fields():
    """Fields not declared in ModelPars inherit DefaultModelPars (NDim=3, counts 0)."""
    _, caps = _parse_str(
        "#define MODELNAME fake\n"
        "struct ModelPars : public TempLat::DefaultModelPars {\n"
        "  static constexpr size_t NScalars = 1;\n"
        "};\n"
    )
    assert caps["NDim"] == 3
    assert caps["NCScalars"] == 0 and caps["NU1Flds"] == 0


def test_yaml_copies_in_sync():
    """The committed parameters.yaml copies match the headers (no drift)."""
    assert g.main(["--check"]) == 0, "parameters.yaml is out of sync with headers"


def _run():
    tests = [
        test_all_models_extracted_lphi4_first,
        test_capability_matrix_spot_checks,
        test_active_typedef_detected,
        test_using_syntax_detected,
        test_commented_typedef_is_inactive,
        test_inactive_default_args_not_counted,
        test_defaults_applied_for_absent_fields,
        test_yaml_copies_in_sync,
    ]
    failures = 0
    for t in tests:
        try:
            t()
        except AssertionError as exc:
            failures += 1
            print(f"FAIL {t.__name__}: {exc}")
        else:
            print(f"ok   {t.__name__}")
    if failures:
        sys.exit(f"{failures} test(s) failed")
    print("all tests passed")


if __name__ == "__main__":
    _run()
