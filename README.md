# TRD generator

This repository contains a small generator that builds a **team reference document (TRD)** from header files (`.h`) placed under `algos/`.

## How it works

- **Input**: C++ header files under `algos/**.h`
- **Selection**: controlled by `trd.yaml`:
  - `include`: glob patterns matched against paths *relative to* `algos/`
  - `exclude`: glob patterns to skip
- **Output (intermediate)**: LaTeX file `build/trd.generated.tex`
- **Output (final)**: PDF compiled by `pdflatex`

When generating, headers are grouped into sections by their parent directory. Each file becomes a subsection with source path shown above the listing.

## Dependencies

### Python

- `python3` (recommended: 3.10+)
- Python package(s):
  - `PyYAML` (see `requirements.txt`)

### LaTeX

LaTeX is **only needed if you want to build a PDF**. If you only need the generated `.tex`, you can skip installing TeX completely and use `make gen-tex`.

The document uses `babel` with `english,russian`, so you need Cyrillic language support too.

#### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y texlive-latex-base texlive-latex-recommended texlive-latex-extra texlive-lang-cyrillic
```

#### macOS

- Install **MacTeX** (includes `pdflatex`), or a smaller TeX distribution that provides `pdflatex` + required packages.
- Typical option (Homebrew cask):

```bash
brew install --cask mactex
```

#### Windows

- Install a TeX distribution:
  - **MiKTeX** (usually easiest on Windows), or
  - **TeX Live**
- Ensure `pdflatex` is available in `PATH` (restart terminal after install).

## Install Python deps

If you want to run the script manually:

```bash
python3 -m venv .venv
.venv/bin/pip install -r requirements.txt
```

## Generate

### Generate only LaTeX (`.tex`)

```bash
make gen-tex
```

This produces `build/trd.generated.tex`.

### Generate PDF

From the repository root:

```bash
make gen-pdf
```

This will:

- create `.venv` and install Python deps (once)
- run `generate_trd.py --config trd.yaml`
- run `pdflatex` to compile the PDF
- copy the final PDF to `TRD.pdf`

`make gen-trd` is an alias for `make gen-pdf`.

## Configure what goes into TRD

Edit `trd.yaml`. Example keys:

- `title`: title on the first page
- `team`: header (left) on each page
- `problems`: `"A-N"` or `"A,B,C"`
- `include` / `exclude`: file selection rules

Important: patterns are matched against paths relative to `algos/`.

## Formatting (clang-format) and VS Code

This repo includes a `.clang-format` file (Google-based style, 4 spaces, 120 columns).

### Install clang-format

- **Linux (Ubuntu/Debian)**:

```bash
sudo apt-get update
sudo apt-get install -y clang-format
```

- **macOS**:

```bash
brew install clang-format
```

- **Windows**:
  - Install LLVM (includes `clang-format`), then make sure `clang-format` is in `PATH`.

### Run clang-format manually

Format a single file:

```bash
clang-format -i algos/path/to/File.h
```

### VS Code auto-format on save

1. Install the extension **"Clang-Format"** (or use the built-in C/C++ extension if you prefer).
2. Add a workspace settings file at `.vscode/settings.json`:

```json
{
  "editor.formatOnSave": true,
  "[cpp]": { "editor.defaultFormatter": "xaver.clang-format" },
  "[c]": { "editor.defaultFormatter": "xaver.clang-format" },
  "clang-format.style": "file"
}
```

If VS Code can’t find `clang-format`, set its path explicitly (example for Linux/macOS):

```json
{
  "clang-format.executable": "/usr/bin/clang-format"
}
```

## Layout your algorithms

Put headers under `algos/`, for example:

```text
algos/
  Graphs/
    Dijkstra.h
  Data Structures/
    Fenwick.h
```

Then reference them in `include` patterns.
