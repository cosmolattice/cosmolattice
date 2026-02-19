#!/usr/bin/env python3
"""Apply Phase 2 transformations using externalize_test.py"""

import subprocess
import sys
from pathlib import Path

# Phase 2 classes: (name, header, test_header, cpp, use_struct)
PHASE_2_CLASSES = [
    # TempLat/util/ (14 remaining classes - ContainsSpace already done in Phase 1)
    ("GetCPPTypeName", "src/include/TempLat/util/getcpptypename.h", "src/include/TempLat/util/getcpptypename_test.h", "src/tests/TempLat/util/getcpptypename.cpp", False),
    ("PrettyToString", "src/include/TempLat/util/prettytostring.h", "src/include/TempLat/util/prettytostring_test.h", "src/tests/TempLat/util/prettytostring.cpp", False),
    ("NumericalIntegrator", "src/include/TempLat/util/numericalintegrator.h", "src/include/TempLat/util/numericalintegrator_test.h", "src/tests/TempLat/util/numericalintegrator.cpp", False),
    ("FileToString", "src/include/TempLat/util/filetostring.h", "src/include/TempLat/util/filetostring_test.h", "src/tests/TempLat/util/filetostring.cpp", False),
    ("FloatToString", "src/include/TempLat/util/floattostring.h", "src/include/TempLat/util/floattostring_test.h", "src/tests/TempLat/util/floattostring.cpp", False),
    ("StringTrimmer", "src/include/TempLat/util/stringtrimmer.h", "src/include/TempLat/util/stringtrimmer_test.h", "src/tests/TempLat/util/stringtrimmer.cpp", False),
    ("ParenthesisStripper", "src/include/TempLat/util/parenthesisstripper.h", "src/include/TempLat/util/parenthesisstripper_test.h", "src/tests/TempLat/util/parenthesisstripper.cpp", False),
    ("Endianness", "src/include/TempLat/util/endianness.h", "src/include/TempLat/util/endianness_test.h", "src/tests/TempLat/util/endianness.cpp", False),
    ("CStyleTime", "src/include/TempLat/util/cstyletime.h", "src/include/TempLat/util/cstyletime_test.h", "src/tests/TempLat/util/cstyletime.cpp", False),
    ("LoadBalance", "src/include/TempLat/util/loadbalance.h", "src/include/TempLat/util/loadbalance_test.h", "src/tests/TempLat/util/loadbalance.cpp", False),
    ("Factorize", "src/include/TempLat/util/factorize.h", "src/include/TempLat/util/factorize_test.h", "src/tests/TempLat/util/factorize.cpp", False),
    ("IsInContainer", "src/include/TempLat/util/isincontainer.h", "src/include/TempLat/util/isincontainer_test.h", "src/tests/TempLat/util/isincontainer.cpp", False),
    ("Exception", "src/include/TempLat/util/exception.h", "src/include/TempLat/util/exception_test.h", "src/tests/TempLat/util/exception.cpp", False),
    ("StaticWarning", "src/include/TempLat/util/staticwarning.h", "src/include/TempLat/util/staticwarning_test.h", "src/tests/TempLat/util/staticwarning.cpp", False),

    # TempLat/util/debug/ (3 classes)
    ("PoorMansProfile", "src/include/TempLat/util/debug/poormansprofile.h", "src/include/TempLat/util/debug/poormansprofile_test.h", "src/tests/TempLat/util/debug/poormansprofile.cpp", False),
    ("MPIDebuggerHanger", "src/include/TempLat/util/debug/mpidebuggerhanger.h", "src/include/TempLat/util/debug/mpidebuggerhanger_test.h", "src/tests/TempLat/util/debug/mpidebuggerhanger.cpp", False),
    ("Stacktrace", "src/include/TempLat/util/debug/stacktrace.h", "src/include/TempLat/util/debug/stacktrace_test.h", "src/tests/TempLat/util/debug/stacktrace.cpp", False),

    # TempLat/util/hash/ (2 classes)
    ("KeccakHashBareClass", "src/include/TempLat/util/hash/keccakhashbareclass.h", "src/include/TempLat/util/hash/keccakhashbareclass_test.h", "src/tests/TempLat/util/hash/keccakhashbareclass.cpp", False),
    ("KeccakHash", "src/include/TempLat/util/hash/keccakhash.h", "src/include/TempLat/util/hash/keccakhash_test.h", "src/tests/TempLat/util/hash/keccakhash.cpp", False),

    # TempLat/util/json/ (1 class)
    ("FileToJSON", "src/include/TempLat/util/json/filetojson.h", "src/include/TempLat/util/json/filetojson_test.h", "src/tests/TempLat/util/json/filetojson.cpp", False),

    # TempLat/util/conditionaloutput/ (4 classes, templated - use struct)
    ("ConditionalFileStream", "src/include/TempLat/util/conditionaloutput/conditionalfilestream.h", "src/include/TempLat/util/conditionaloutput/conditionalfilestream_test.h", "src/tests/TempLat/util/conditionaloutput/conditionalfilestream.cpp", True),
    ("ConditionalStream", "src/include/TempLat/util/conditionaloutput/conditionalstream.h", "src/include/TempLat/util/conditionaloutput/conditionalstream_test.h", "src/tests/TempLat/util/conditionaloutput/conditionalstream.cpp", True),
    ("ConditionalSayShort", "src/include/TempLat/util/conditionaloutput/conditionalsayshort.h", "src/include/TempLat/util/conditionaloutput/conditionalsayshort_test.h", "src/tests/TempLat/util/conditionaloutput/conditionalsayshort.cpp", True),
    ("OutputStream", "src/include/TempLat/util/conditionaloutput/outputstream.h", "src/include/TempLat/util/conditionaloutput/outputstream_test.h", "src/tests/TempLat/util/conditionaloutput/outputstream.cpp", True),

    # TempLat/util/random/ (1 class)
    ("RandomGaussian", "src/include/TempLat/util/random/randomgaussian.h", "src/include/TempLat/util/random/randomgaussian_test.h", "src/tests/TempLat/util/random/randomgaussian.cpp", False),

    # TempLat/parameters/ (5 classes, some templated)
    ("PairMaker", "src/include/TempLat/parameters/pairmaker.h", "src/include/TempLat/parameters/pairmaker_test.h", "src/tests/TempLat/parameters/pairmaker.cpp", False),
    ("FileReader", "src/include/TempLat/parameters/filereader.h", "src/include/TempLat/parameters/filereader_test.h", "src/tests/TempLat/parameters/filereader.cpp", False),
    ("ParameterParser", "src/include/TempLat/parameters/parameterparser.h", "src/include/TempLat/parameters/parameterparser_test.h", "src/tests/TempLat/parameters/parameterparser.cpp", False),
    ("ParameterGetter", "src/include/TempLat/parameters/parametergetter.h", "src/include/TempLat/parameters/parametergetter_test.h", "src/tests/TempLat/parameters/parametergetter.cpp", True),
    ("MultipleParameterGetter", "src/include/TempLat/parameters/multipleParametergetter.h", "src/include/TempLat/parameters/multipleParametergetter_test.h", "src/tests/TempLat/parameters/multipleParametergetter.cpp", True),

    # TempLat/session/ (1 class)
    ("SessionGuard", "src/include/TempLat/session/sessionguard.h", "src/include/TempLat/session/sessionguard_test.h", "src/tests/TempLat/session/sessionguard.cpp", False),
]

def apply_transformation(class_name, header, test_header, cpp, use_struct, dry_run=False):
    """Apply transformation for a single class."""
    cmd = [
        "python3", "tools/externalize_test.py",
        "--class", class_name,
        "--header", header,
        "--test-header", test_header,
        "--cpp", cpp,
    ]
    if use_struct:
        cmd.append("--struct")
    if dry_run:
        cmd.append("--dry-run")

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0, result.stdout + result.stderr

def main():
    dry_run = "--dry-run" in sys.argv

    print(f"Applying Phase 2 transformations ({len(PHASE_2_CLASSES)} classes)...")
    if dry_run:
        print("[DRY RUN MODE - no files will be modified]\n")

    failed = []
    succeeded = []

    for class_name, header, test_header, cpp, use_struct in PHASE_2_CLASSES:
        # Verify files exist
        if not Path(header).exists():
            print(f"✗ {class_name}: header not found")
            failed.append(class_name)
            continue
        if not Path(test_header).exists():
            print(f"✗ {class_name}: test_header not found")
            failed.append(class_name)
            continue
        if not Path(cpp).exists():
            print(f"✗ {class_name}: cpp not found")
            failed.append(class_name)
            continue

        success, output = apply_transformation(class_name, header, test_header, cpp, use_struct, dry_run)
        if success:
            print(f"✓ {class_name}")
            succeeded.append(class_name)
        else:
            print(f"✗ {class_name}")
            print(f"  {output}")
            failed.append(class_name)

    print(f"\n{'='*60}")
    print(f"Succeeded: {len(succeeded)}/{len(PHASE_2_CLASSES)}")
    if failed:
        print(f"Failed: {', '.join(failed)}")

if __name__ == '__main__':
    main()
