# Makefile for the CosmoLattice documentation.
#
# The parameter appendix tables in
#   source/docs/Manual/Appendix_Parameters.md
# are GENERATED from the single source of truth
#   source/data/parameters.yaml
# After editing parameters.yaml, run `make gen-params` and commit the
# regenerated appendix. Run `make check-params` locally before committing to
# confirm the database is in sync — both the appendix (generated from the YAML)
# and the C++ get<> call sites (no parameter drifted). This is the same script
# build.sh runs, and that a future CI step can call.

# Prefer the project virtualenv created by setup_python.sh / build.sh — it has
# PyYAML and the rest of the docs dependencies. Bare `python3` (as resolved by
# make's /bin/sh) is often a system/Homebrew interpreter without PyYAML, which
# breaks the parameter scripts. Fall back to it only when the venv is absent.
# Override explicitly with `make <target> PYTHON=/path/to/python`.
VENV_PYTHON := tmp/.venv/bin/python
PYTHON ?= $(shell [ -x "$(VENV_PYTHON)" ] && echo "$(VENV_PYTHON)" || echo python3)
GEN    := scripts/gen_param_appendix.py

.PHONY: help gen-params gen-model-caps check-params check-params-code check-model-caps check-models test-params docs

help:
	@echo "CosmoLattice documentation — make targets:"
	@echo "  make gen-params         Regenerate the parameter appendix tables from parameters.yaml"
	@echo "  make gen-model-caps     Regenerate the metadata.models capability block in parameters.yaml"
	@echo "  make check-params       Verify the database is in sync: appendix + code<->YAML (no write)"
	@echo "  make check-params-code  Verify parameters.yaml matches the C++ get<> call sites only"
	@echo "  make check-model-caps   Verify metadata.models matches the model headers (no write)"
	@echo "  make check-models       Verify models.yaml lists exactly the models in the code (no write)"
	@echo "  make test-params        Run the parameter generator + drift-checker unit tests"
	@echo "  make docs               Build the full documentation site (runs build.sh)"

# Regenerate the appendix tables in place. Idempotent: only the marker regions
# in Appendix_Parameters.md change; surrounding prose is left untouched.
gen-params:
	$(PYTHON) $(GEN)

# Regenerate the metadata.models capability block (mirrored from models/*.h
# ModelPars) in both parameters.yaml copies. Idempotent: only the marked region
# changes.
gen-model-caps:
	$(PYTHON) scripts/gen_model_capabilities.py

# Local sync check (both directions): runs the unit tests + a no-write appendix
# `--check` + the code<->YAML drift checker. Exits non-zero on any drift without
# modifying files. Self-contained so a future CI step can call the same script.
check-params:
	PYTHON=$(PYTHON) bash scripts/check_params_sync.sh

# Just the code<->YAML drift check (parameters.yaml vs the get<> call sites).
check-params-code:
	$(PYTHON) scripts/check_params_code.py

# Just the model-capability drift check (metadata.models vs models/*.h ModelPars).
check-model-caps:
	$(PYTHON) scripts/gen_model_capabilities.py --check

# Just the model-registry check (models.yaml vs models/*.h vs metadata.models).
check-models:
	$(PYTHON) scripts/check_models_registry.py

test-params:
	$(PYTHON) scripts/test_gen_param_appendix.py
	$(PYTHON) scripts/test_check_params_code.py
	$(PYTHON) scripts/test_gen_model_capabilities.py

# Full documentation build (Doxygen + MkDocs). Output goes to website/site/.
docs:
	bash build.sh
