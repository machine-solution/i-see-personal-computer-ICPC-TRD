.PHONY: gen-trd gen-tex gen-pdf clean list

# Output files in this repo root (trd/)
TRD_PDF := TRD.pdf

# Generator paths (relative to trd/)
VENV_DIR := .venv
PY       := $(VENV_DIR)/bin/python
PIP      := $(VENV_DIR)/bin/pip

CONFIG   := trd.yaml
TEX_OUT  := build/trd.generated.tex
PDF_OUT  := build/trd.generated.pdf

gen-trd: gen-pdf

gen-tex: $(PY)
	@$(PY) generate_trd.py --config $(CONFIG)
	@echo "Generated: $(TEX_OUT)"

gen-pdf: gen-tex
	@command -v pdflatex >/dev/null 2>&1 || { \
		echo "pdflatex not found. Install TeX to build PDF (TeX Live / MacTeX / MiKTeX)."; \
		exit 1; \
	}
	@pdflatex -shell-escape -interaction=nonstopmode -halt-on-error -output-directory=build $(TEX_OUT) || { \
		echo ""; \
		echo "pdflatex failed. See build/trd.generated.log for details."; \
		exit 1; \
	}
	@pdflatex -shell-escape -interaction=nonstopmode -halt-on-error -output-directory=build $(TEX_OUT) || { \
		echo ""; \
		echo "pdflatex failed on 2nd pass. See build/trd.generated.log for details."; \
		exit 1; \
	}
	@cp -f $(PDF_OUT) $(TRD_PDF)
	@echo "Generated: $(TRD_PDF)"

list: $(PY)
	@$(PY) generate_trd.py --config $(CONFIG) --list

$(PY): requirements.txt
	@python3 -m venv $(VENV_DIR)
	@$(PIP) install -r requirements.txt >/dev/null

clean:
	@rm -rf build .venv $(TRD_PDF)
	@echo "Cleaned TRD artifacts"
