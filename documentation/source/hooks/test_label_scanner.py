"""Tests for label_scanner module."""

import os
import tempfile
import warnings

from label_scanner import scan_directory, scan_file


def _write_tmp(content: str, suffix: str = ".h") -> str:
    """Write content to a temp file and return its path."""
    fd, path = tempfile.mkstemp(suffix=suffix)
    with os.fdopen(fd, "w") as f:
        f.write(content)
    return path


class TestScanFile:
    def test_basic_label(self):
        path = _write_tmp(
            "// @label:model_params\n"
            "double lambda = 0.1;\n"
            "double phi0 = 1.0;\n"
            "// @endlabel\n"
        )
        try:
            labels = scan_file(path)
            assert labels == {"model_params": (2, 3)}
        finally:
            os.unlink(path)

    def test_multiple_labels(self):
        path = _write_tmp(
            "int x;\n"
            "// @label:first\n"
            "line A;\n"
            "// @endlabel\n"
            "int y;\n"
            "// @label:second\n"
            "line B;\n"
            "line C;\n"
            "// @endlabel\n"
        )
        try:
            labels = scan_file(path)
            assert labels == {"first": (3, 3), "second": (7, 8)}
        finally:
            os.unlink(path)

    def test_label_with_leading_whitespace(self):
        path = _write_tmp(
            "  // @label:indented\n"
            "  code;\n"
            "  // @endlabel\n"
        )
        try:
            labels = scan_file(path)
            assert labels == {"indented": (2, 2)}
        finally:
            os.unlink(path)

    def test_no_labels(self):
        path = _write_tmp("int x = 1;\nint y = 2;\n")
        try:
            labels = scan_file(path)
            assert labels == {}
        finally:
            os.unlink(path)

    def test_unclosed_label_warns(self):
        path = _write_tmp("// @label:oops\ncode;\n")
        try:
            with warnings.catch_warnings(record=True) as w:
                warnings.simplefilter("always")
                labels = scan_file(path)
                assert len(w) == 1
                assert "unclosed" in str(w[0].message).lower()
                assert labels == {}
        finally:
            os.unlink(path)

    def test_endlabel_without_opener_warns(self):
        path = _write_tmp("code;\n// @endlabel\n")
        try:
            with warnings.catch_warnings(record=True) as w:
                warnings.simplefilter("always")
                scan_file(path)
                assert len(w) == 1
                assert "without" in str(w[0].message).lower()
        finally:
            os.unlink(path)

    def test_duplicate_label_warns(self):
        path = _write_tmp(
            "// @label:dup\n"
            "a;\n"
            "// @endlabel\n"
            "// @label:dup\n"
            "b;\n"
            "// @endlabel\n"
        )
        try:
            with warnings.catch_warnings(record=True) as w:
                warnings.simplefilter("always")
                labels = scan_file(path)
                assert len(w) == 1
                assert "duplicate" in str(w[0].message).lower()
                # Second definition overwrites the first
                assert labels == {"dup": (5, 5)}
        finally:
            os.unlink(path)

    def test_nested_label_warns(self):
        path = _write_tmp(
            "// @label:outer\n"
            "a;\n"
            "// @label:inner\n"
            "b;\n"
            "// @endlabel\n"
        )
        try:
            with warnings.catch_warnings(record=True) as w:
                warnings.simplefilter("always")
                labels = scan_file(path)
                # Should warn about nesting
                assert any("still open" in str(x.message).lower() for x in w)
                # inner should be recorded (outer was implicitly closed)
                assert "inner" in labels
        finally:
            os.unlink(path)


class TestScanDirectory:
    def test_scans_matching_files(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            # Create a .h file with a label
            h_file = os.path.join(tmpdir, "model.h")
            with open(h_file, "w") as f:
                f.write("// @label:params\ndouble x;\n// @endlabel\n")

            # Create a .cpp file with a label
            cpp_file = os.path.join(tmpdir, "model.cpp")
            with open(cpp_file, "w") as f:
                f.write("// @label:init\nsetup();\n// @endlabel\n")

            # Create a .py file (should be ignored)
            py_file = os.path.join(tmpdir, "script.py")
            with open(py_file, "w") as f:
                f.write("# @label:nope\nx = 1\n# @endlabel\n")

            result = scan_directory(tmpdir)
            assert "model.h" in result
            assert "model.cpp" in result
            assert "script.py" not in result
            assert result["model.h"] == {"params": (2, 2)}
            assert result["model.cpp"] == {"init": (2, 2)}

    def test_subdirectory_scanning(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            subdir = os.path.join(tmpdir, "sub")
            os.makedirs(subdir)
            h_file = os.path.join(subdir, "deep.h")
            with open(h_file, "w") as f:
                f.write("// @label:deep\nint y;\n// @endlabel\n")

            result = scan_directory(tmpdir)
            assert os.path.join("sub", "deep.h") in result

    def test_empty_directory(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            result = scan_directory(tmpdir)
            assert result == {}
