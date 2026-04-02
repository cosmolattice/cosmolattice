"""Tests for resolve_emgithub hook."""

import logging
import os
import tempfile

import resolve_emgithub as hook


def _make_source_tree(tmpdir):
    """Create a mock source tree with labeled files.

    Mirrors the real layout: docs_dir is <tmpdir>/source/docs,
    and code_source is at <tmpdir>/tmp/code_source/cosmolattice
    (i.e. ../../tmp/code_source/cosmolattice relative to docs_dir).
    """
    docs_dir = os.path.join(tmpdir, "source", "docs")
    os.makedirs(docs_dir, exist_ok=True)
    src = os.path.join(tmpdir, "tmp", "code_source", "cosmolattice")
    os.makedirs(src, exist_ok=True)

    model_dir = os.path.join(src, "src", "models")
    os.makedirs(model_dir, exist_ok=True)

    with open(os.path.join(model_dir, "lphi4.h"), "w") as f:
        f.write(
            "#pragma once\n"
            "// @label:model_params\n"
            "double lambda = 0.1;\n"
            "double phi0 = 1.0;\n"
            "// @endlabel\n"
            "void setup();\n"
        )

    include_dir = os.path.join(src, "include")
    os.makedirs(include_dir, exist_ok=True)

    with open(os.path.join(include_dir, "cosmo.h"), "w") as f:
        f.write(
            "// @label:constants\n"
            "const double pi = 3.14;\n"
            "// @endlabel\n"
        )

    return docs_dir


class FakePage:
    class FakeFile:
        def __init__(self, src_path):
            self.src_path = src_path
    def __init__(self, src_path="test.md"):
        self.file = self.FakeFile(src_path)


class TestResolveEmgithub:
    def setup_method(self):
        # Reset cached index between tests
        hook._label_index = None

    def test_no_patterns_passthrough(self):
        md = "# Hello world\nSome text."
        result = hook.on_page_markdown(md, FakePage(), {"docs_dir": "/fake"}, None)
        assert result == md

    def test_whole_file_embed(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            docs_dir = _make_source_tree(tmpdir)
            config = {"docs_dir": docs_dir}

            md = "@emgithub(src/models/lphi4.h)"
            result = hook.on_page_markdown(md, FakePage(), config, None)

            assert "emgithub.com/embed-v2.js" in result
            assert "lphi4.h" in result
            # No line range for whole-file
            assert "#L" not in result
            assert "<script" in result

    def test_labeled_embed(self):
        hook._label_index = None
        with tempfile.TemporaryDirectory() as tmpdir:
            docs_dir = _make_source_tree(tmpdir)
            config = {"docs_dir": docs_dir}

            md = "@emgithub(src/models/lphi4.h:model_params)"
            result = hook.on_page_markdown(md, FakePage(), config, None)

            assert "emgithub.com/embed-v2.js" in result
            assert "L3-L4" in result  # Lines inside the label markers

    def test_missing_label_warns(self, caplog):
        hook._label_index = None
        with tempfile.TemporaryDirectory() as tmpdir:
            docs_dir = _make_source_tree(tmpdir)
            config = {"docs_dir": docs_dir}

            md = "@emgithub(src/models/lphi4.h:nonexistent)"
            with caplog.at_level(logging.WARNING):
                result = hook.on_page_markdown(md, FakePage(), config, None)

            # Should leave the original pattern in place
            assert result == md
            assert "nonexistent" in caplog.text

    def test_missing_file_warns(self, caplog):
        hook._label_index = None
        with tempfile.TemporaryDirectory() as tmpdir:
            docs_dir = _make_source_tree(tmpdir)
            config = {"docs_dir": docs_dir}

            md = "@emgithub(no/such/file.h:label)"
            with caplog.at_level(logging.WARNING):
                result = hook.on_page_markdown(md, FakePage(), config, None)

            assert result == md
            assert "no labels" in caplog.text

    def test_multiple_patterns_in_one_page(self):
        hook._label_index = None
        with tempfile.TemporaryDirectory() as tmpdir:
            docs_dir = _make_source_tree(tmpdir)
            config = {"docs_dir": docs_dir}

            md = (
                "Before\n"
                "@emgithub(src/models/lphi4.h:model_params)\n"
                "Middle\n"
                "@emgithub(include/cosmo.h:constants)\n"
                "After"
            )
            result = hook.on_page_markdown(md, FakePage(), config, None)

            assert result.count("<script") == 2
            assert "Before" in result
            assert "Middle" in result
            assert "After" in result

    def test_url_encoding(self):
        hook._label_index = None
        with tempfile.TemporaryDirectory() as tmpdir:
            docs_dir = _make_source_tree(tmpdir)
            config = {"docs_dir": docs_dir}

            md = "@emgithub(src/models/lphi4.h)"
            result = hook.on_page_markdown(md, FakePage(), config, None)

            # The URL should be percent-encoded
            assert "%2F" in result
            assert "https%3A%2F%2Fgithub.com" in result
