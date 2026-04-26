#!/usr/bin/env python3

from pathlib import Path
from fnmatch import fnmatch
import argparse
import hashlib
import shutil
import yaml


ROOT = Path(__file__).resolve().parent
ALGOS_DIR = ROOT / "algos"
BUILD_DIR = ROOT / "build"
SNIPPETS_DIR = BUILD_DIR / "snippets"
OUTPUT_TEX = BUILD_DIR / "trd.generated.tex"


def deep_merge(a, b):
    res = a.copy()
    for k, v in b.items():
        if isinstance(v, dict) and isinstance(res.get(k), dict):
            res[k] = deep_merge(res[k], v)
        else:
            res[k] = v
    return res


DEFAULT = {
    "team": "Team",
    "contest_title": "ICPC 2026",
    "problems": "A-N",
    "include": ["**/*.h"],
    "exclude": [],
    "fonts": {},
    "layout": {}
}


def tex_escape(s: str) -> str:
    for k, v in {
        "&": r"\&", "%": r"\%", "$": r"\$", "#": r"\#",
        "_": r"\_", "{": r"\{", "}": r"\}"
    }.items():
        s = s.replace(k, v)
    return s


def pretty(s: str):
    return s.replace("_", " ").replace("-", " ")


def strip(text):
    lines = text.splitlines()
    for i, l in enumerate(lines):
        if l.strip().lower() in {"// include", "// includes"}:
            return "\n".join(lines[i+1:]) + "\n"
    return text + "\n"


def matches(path, patterns):
    return any(fnmatch(path, p) for p in patterns)


def parse_range(s):
    if "-" in s:
        a, b = s.split("-")
        return [chr(c) for c in range(ord(a.strip()), ord(b.strip())+1)]
    return s.split(",")


def discover(cfg):
    if SNIPPETS_DIR.exists():
        shutil.rmtree(SNIPPETS_DIR)
    SNIPPETS_DIR.mkdir(parents=True)

    res = {}

    for f in sorted(ALGOS_DIR.rglob("*.h")):
        rel = f.relative_to(ALGOS_DIR).as_posix()

        if not matches(rel, cfg["include"]): continue
        if matches(rel, cfg["exclude"]): continue

        section = pretty(f.parent.name)
        title = pretty(f.stem)

        code = strip(f.read_text())

        name = hashlib.md5(rel.encode()).hexdigest()[:10] + ".cpp"
        path = SNIPPETS_DIR / name
        path.write_text(code)

        res.setdefault(section, []).append((title, path.as_posix()))

    return res


def table(problems, cfg):
    rows = "\n".join(f"{p} & & & & & \\\\ \\hline" for p in problems)
    contest_title = tex_escape(cfg.get("contest_title", "") or "")
    contest_font = cfg["fonts"]["contest_title"]
    table_font = cfg["fonts"]["problem_table"]

    title_block = ""
    if contest_title:
        title_block = rf"{{{contest_font}\bfseries {contest_title}}}\\[0.5em]"

    # Keep this string simple: avoid tricky f-string brace escaping.
    return "\n".join(
        [
            r"\begin{center}",
            title_block,
            table_font,
            r"\renewcommand{\arraystretch}{1.2}",
            r"\setlength{\tabcolsep}{6pt}",
            # Keep the table spec simple to avoid accidental "extra column" perception.
            r"\begin{tabular}{|c|c|c|c|c|c|}",
            r"\hline",
            r"PROBLEM & READ & SOLVED & WRITTEN & OPENED & COMMENT \\",
            r"\hline",
            rows,
            r"\end{tabular}",
            r"\end{center}",
            r"\vspace{1em}",
            "",
        ]
    )

def timeline_table(cfg: dict) -> str:
    font = cfg["fonts"]["problem_table"]

    times = [
        "0:30", "1:00", "1:30", "2:00",
        "2:30", "3:00", "3:30", "4:00",
        "4:30", "5:00"
    ]

    rows = "\n".join(
        rf"{t} & & & \\ \hline"
        for t in times
    )

    return rf"""
\begin{{center}}
{{{font}
\renewcommand{{\arraystretch}}{{1.2}}
\setlength{{\tabcolsep}}{{6pt}}
\begin{{tabular*}}{{\linewidth}}{{@{{\extracolsep{{\fill}}}}|>{{\centering\arraybackslash}}p{{1.8cm}}|>{{\centering\arraybackslash}}p{{2.5cm}}|>{{\centering\arraybackslash}}p{{2.5cm}}|>{{\centering\arraybackslash}}p{{2.5cm}}|}}
\hline
Time & Andrey & Taisia & Timofey \\
\hline
{rows}
\end{{tabular*}}
}}
\end{{center}}

\vspace{{1em}}
"""


def generate(cfg, sections):
    f = cfg["fonts"]
    l = cfg["layout"]
    probs = parse_range(cfg["problems"])
    team = tex_escape(cfg["team"])

    out = []

    out.append(r"\documentclass[10pt,a4paper,landscape]{article}")
    out.append(r"\usepackage[utf8]{inputenc}")
    out.append(r"\usepackage[T2A]{fontenc}")
    out.append(r"\usepackage[russian,english]{babel}")
    out.append(r"\usepackage{geometry}")
    out.append(r"\usepackage{multicol}")
    out.append(r"\usepackage{array}")
    out.append(r"\usepackage{fancyhdr}")
    out.append(r"\usepackage{titlesec}")
    out.append(r"\usepackage{tocloft}")
    out.append(r"\usepackage{lastpage}")
    out.append(r"\usepackage{xcolor}")
    out.append(r"\usepackage{listings}")

    out.append(
        rf"\geometry{{left={l['left_margin']},right={l['right_margin']},top={l['top_margin']},bottom={l['bottom_margin']},headheight={l['head_height']},headsep={l['head_sep']},includehead}}"
    )

    out.append(r"\setlength{\parindent}{0pt}")
    out.append(r"\setlength{\parskip}{0pt}")
    out.append(rf"\setlength{{\columnsep}}{{{l['column_sep']}}}")

    out.append(r"\pagestyle{fancy}")
    out.append(r"\fancyhf{}")
    out.append(rf"\fancyhead[L]{{{f['header']} {team}}}")
    out.append(rf"\fancyhead[R]{{{f['header']} \thepage\ of \pageref{{LastPage}}}}")
    out.append(r"\renewcommand{\headrulewidth}{0.4pt}")
    out.append(r"\fancypagestyle{plain}{")
    out.append(r"  \fancyhf{}")
    out.append(rf"  \fancyhead[L]{{{f['header']} {team}}}")
    out.append(rf"  \fancyhead[R]{{{f['header']} \thepage\ of \pageref{{LastPage}}}}")
    out.append(r"  \renewcommand{\headrulewidth}{0.4pt}")
    out.append(r"}")

    out.append(r"\renewcommand{\contentsname}{Содержание}")
    out.append(rf"\renewcommand{{\cfttoctitlefont}}{{{f['toc_title']}\bfseries}}")

    out.append(rf"\titleformat{{\section}}{{{f['section']}\bfseries}}{{\thesection}}{{0.5em}}{{}}")
    out.append(rf"\titleformat{{\subsection}}{{{f['subsection']}\bfseries}}{{\thesubsection}}{{0.5em}}{{}}")

    # стиль кода как в TRD, но с цветной подсветкой
    out.append(r"\definecolor{codeblue}{rgb}{0.05,0.05,0.70}")
    out.append(r"\definecolor{codepurple}{rgb}{0.55,0.00,0.55}")
    out.append(r"\definecolor{codegreen}{rgb}{0.00,0.45,0.00}")
    out.append(r"\definecolor{codegray}{rgb}{0.45,0.45,0.45}")
    out.append(r"\definecolor{codeteal}{rgb}{0.00,0.45,0.55}")

    out.append(r"\lstdefinestyle{trd}{")
    out.append(rf"  basicstyle=\ttfamily{f['code']},")
    out.append(r"  language=C++,")
    out.append(r"  numbers=none,")
    out.append(r"  keywordstyle=\color{codeblue}\bfseries,")
    out.append(r"  keywordstyle=[2]\color{codepurple},")
    out.append(r"  keywordstyle=[3]\color{codeteal},")
    out.append(r"  commentstyle=\color{codegray}\itshape,")
    out.append(r"  stringstyle=\color{codegreen},")
    out.append(r"  morekeywords=[2]{ll,ull,ld,string,vector,pair,set,map,unordered_map,unordered_set,queue,deque,priority_queue,stack,array,tuple},")
    out.append(r"  morekeywords=[3]{cin,cout,endl,sort,lower_bound,upper_bound,begin,end,rbegin,rend,push_back,pop_back,emplace_back,make_pair,swap,min,max,abs,gcd},")
    out.append(r"  breaklines=true,")
    out.append(r"  breakatwhitespace=false,")
    out.append(r"  columns=fullflexible,")
    out.append(r"  keepspaces=true,")
    out.append(r"  showstringspaces=false,")
    out.append(r"  tabsize=2,")
    out.append(r"  xleftmargin=0pt,")
    out.append(r"  aboveskip=0.15em,")
    out.append(r"  belowskip=0.25em")
    out.append(r"}")

    out.append(r"\begin{document}")
    # Ensure header is present on first page too.
    out.append(r"\pagestyle{fancy}")
    out.append(r"\thispagestyle{fancy}")
    out.append(r"\begin{multicols}{2}")

    out.append(table(probs, cfg))
    out.append(timeline_table(cfg))
    out.append(r"\tableofcontents")
    out.append(r"\vspace{0.5em}")

    for sec, items in sections.items():
        out.append(rf"\section{{{tex_escape(sec)}}}")
        for title, path in items:
            out.append(rf"\subsection{{{tex_escape(title)}}}")
            out.append(rf"\lstinputlisting[style=trd]{{{path}}}")

    out.append(r"\end{multicols}")
    out.append(r"\end{document}")

    return "\n".join(out)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", default="trd.yml")
    args = parser.parse_args()

    cfg = deep_merge(DEFAULT, yaml.safe_load(open(args.config)))
    sections = discover(cfg)

    BUILD_DIR.mkdir(exist_ok=True)
    OUTPUT_TEX.write_text(generate(cfg, sections))

    print("OK → build/trd.generated.tex")
    print("compile twice:")
    print("pdflatex -output-directory=build build/trd.generated.tex")


if __name__ == "__main__":
    main()
