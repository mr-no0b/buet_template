#!/usr/bin/env python3

import hashlib
import os
import shutil
import subprocess
import sys
import argparse
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
CODE_DIR = SCRIPT_DIR / "code"
NOTEBOOK_TEX = SCRIPT_DIR / "notebook.tex"
CONTENTS_TEX = SCRIPT_DIR / "contents.tex"
SANITIZED_CODE_DIR = SCRIPT_DIR / ".latex_code"


class GenerationError(RuntimeError):
    pass


GENERATED_DIRS = [
    SCRIPT_DIR / "_minted",
    SCRIPT_DIR / ".latex_code",
]

GENERATED_FILES = [
    SCRIPT_DIR / "contents.tex",
    SCRIPT_DIR / "notebook.aux",
    SCRIPT_DIR / "notebook.bbl",
    SCRIPT_DIR / "notebook.blg",
    SCRIPT_DIR / "notebook.fls",
    SCRIPT_DIR / "notebook.log",
    SCRIPT_DIR / "notebook.out",
    SCRIPT_DIR / "notebook.toc",
    SCRIPT_DIR / "notebook.fdb_latexmk",
    SCRIPT_DIR / "notebook.fls",
    SCRIPT_DIR / "notebook.synctex.gz",
]


def clean(also_pdf: bool = False) -> None:
    for d in GENERATED_DIRS:
        if d.exists():
            shutil.rmtree(d)

    for f in GENERATED_FILES:
        try:
            f.unlink()
        except FileNotFoundError:
            pass

    if also_pdf:
        try:
            (SCRIPT_DIR / "notebook.pdf").unlink()
        except FileNotFoundError:
            pass


def _require_file(path: Path, label: str) -> None:
    if not path.exists():
        raise GenerationError(f"Missing {label}: {path}")


def _require_tool(exe_name: str, hint: str) -> None:
    if shutil.which(exe_name) is None:
        raise GenerationError(f"Missing required tool '{exe_name}'. {hint}")


def _sha1_short(path: Path, length: int = 10) -> str:
    h = hashlib.sha1()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()[:length]


def _count_lines(path: Path) -> int:
    # Count newlines in binary to avoid decode errors on odd files.
    data = path.read_bytes()
    if not data:
        return 0
    return data.count(b"\n") + (0 if data.endswith(b"\n") else 1)


def _sanitize_component(name: str) -> str:
    # Make filesystem+TeX friendly names: avoid spaces and special chars.
    # Keep it readable and stable.
    out = []
    for ch in name:
        if ch.isalnum() or ch in "._-":
            out.append(ch)
        elif ch == "&":
            out.append("and")
        else:
            out.append("_")
    sanitized = "".join(out)
    # Avoid empty components.
    return sanitized or "_"


def _sanitized_relpath(original_relpath: Path) -> Path:
    parts = [_sanitize_component(p) for p in original_relpath.parts]
    return Path(*parts)


def _prepare_sanitized_tree() -> None:
    # Rebuild each run to avoid stale links.
    if SANITIZED_CODE_DIR.exists():
        shutil.rmtree(SANITIZED_CODE_DIR)
    SANITIZED_CODE_DIR.mkdir(parents=True, exist_ok=True)


def _link_or_copy(src: Path, dst: Path) -> None:
    dst.parent.mkdir(parents=True, exist_ok=True)
    try:
        os.symlink(src, dst)
    except OSError:
        shutil.copy2(src, dst)


def get_sections():
    if not CODE_DIR.exists():
        raise GenerationError(f"Missing code directory: {CODE_DIR}")

    _prepare_sanitized_tree()

    sections = []
    for (root, _dirs, files) in os.walk(str(CODE_DIR)):
        root_path = Path(root)
        subsections = []
        section_name = root_path.name

        for file_name in sorted(files):
            subsection_name = os.path.splitext(file_name)[0]
            if subsection_name.startswith("."):
                continue

            rel_path = (root_path / file_name).relative_to(SCRIPT_DIR)
            abs_path = SCRIPT_DIR / rel_path
            try:
                number_of_lines = _count_lines(abs_path)
                hash_value = _sha1_short(abs_path)
            except OSError as e:
                raise GenerationError(f"Failed reading {rel_path}: {e}")

            # Create a TeX-friendly mirror path for minted inputs.
            sanitized_rel = Path(".latex_code") / _sanitized_relpath(rel_path)
            sanitized_abs = SCRIPT_DIR / sanitized_rel
            if sanitized_abs.exists():
                # Collision: append a short hash before extension.
                stem = sanitized_abs.stem
                suffix = sanitized_abs.suffix
                sanitized_rel = sanitized_rel.with_name(f"{stem}_{hash_value}{suffix}")
                sanitized_abs = SCRIPT_DIR / sanitized_rel
            _link_or_copy(abs_path, sanitized_abs)

            subsections.append(
                (
                    sanitized_rel.as_posix(),
                    subsection_name,
                    number_of_lines,
                    hash_value,
                )
            )

        # Skip the top-level "code" directory section header (old behavior via sections[1:]).
        if root_path == CODE_DIR:
            continue
        sections.append((section_name, subsections))

    return sections

def get_style(filename):
    ext = filename.lower().split('.')[-1]
    if ext in ['c', 'cc', 'cpp']:
        return 'c++'
    elif ext in ['java']:
        return 'java'
    elif ext in ['py']:
        return 'python'
    else:
        return 'text'

def texify(s):
    # Escape LaTeX special chars for section/subsection titles.
    # Keep this conservative: titles are plain text, not math.
    if s is None:
        return ""
    s = str(s)
    # Order matters: escape backslash first.
    s = s.replace("\\", r"\textbackslash{}")
    s = s.replace("&", r"\&")
    s = s.replace("%", r"\%")
    s = s.replace("$", r"\$")
    s = s.replace("#", r"\#")
    s = s.replace("_", r"\_")
    s = s.replace("{", r"\{")
    s = s.replace("}", r"\}")
    s = s.replace("~", r"\textasciitilde{}")
    s = s.replace("^", r"\textasciicircum{}")
    return s


def texify_filename(path: str) -> str:
    # Paths we generate under .latex_code are already sanitized; keep minimal validation.
    if any(ch in path for ch in "\n\r\\{}"):
        raise GenerationError(f"Unsupported character in path for minted: {path!r}")
    return path

def sanitize_code_for_latex(content):
    """Replace problematic Unicode characters with ASCII equivalents."""
    replacements = {
        # Curly quotes
        '\u201c': '"',  # "
        '\u201d': '"',  # "
        '\u2018': "'",  # '
        '\u2019': "'",  # '
        # Math symbols
        '\u2286': '⊆',  # Keep as-is or use \subseteq in comments
        '\u2287': '⊇',  # Keep as-is or use \supseteq
        '\u2211': 'Σ',  # ∑ -> Sigma
        # Add more as needed
    }
    
    for unicode_char, ascii_char in replacements.items():
        content = content.replace(unicode_char, ascii_char)
    
    return content

def process_directory(base_path, section_name, temp_dir):
    """Process a directory and generate LaTeX content."""
    output = []
    
    if section_name:
        safe_section = escape_latex(section_name)
        output.append(f"\\section{{{safe_section}}}\n")
    
    entries = sorted(os.listdir(base_path))
    
    for entry in entries:
        full_path = os.path.join(base_path, entry)
        
        if os.path.isdir(full_path):
            subsection_output = process_directory(full_path, None, temp_dir)
            if subsection_output:
                safe_entry = escape_latex(entry)
                output.append(f"\\subsection{{{safe_entry}}}\n")
                output.extend(subsection_output)
        
        elif os.path.isfile(full_path):
            try:
                with open(full_path, 'r', encoding='utf-8') as f:
                    content = f.read()
                
                # Sanitize Unicode characters
                content = sanitize_code_for_latex(content)
                
                file_hash = hashlib.sha1(content.encode('utf-8')).hexdigest()[:10]
                
                safe_filename = entry.replace('&', 'and').replace(' ', '_').replace('#', 'num')
                temp_file_path = os.path.join(temp_dir, safe_filename)
                
                os.makedirs(os.path.dirname(temp_file_path), exist_ok=True)
                
                with open(temp_file_path, 'w', encoding='utf-8') as temp_file:
                    temp_file.write(content)
                
                file_ext = os.path.splitext(entry)[1][1:]
                if not file_ext:
                    file_ext = 'text'
                
                safe_entry = escape_latex(entry)
                lines_info = f"[{content.count(chr(10)) + 1} lines] - {file_hash}"
                
                output.append(f"\\subsection{{{safe_entry} \\scriptsize {lines_info}}}\n")
                output.append(f"\\inputminted{{{file_ext}}}{{{temp_file_path}}}\n")
            
            except Exception as e:
                print(f"Warning: Could not process {full_path}: {e}")
    
    return output

def get_tex(sections):
    tex = ''
    for (section_name, subsections) in sections:
        tex += '\\section{%s}\n' % texify(section_name)
        for (relative_path, subsection_name, number_of_lines, hash_value) in subsections:
            tex += '\\subsection{\\small %s  \\scriptsize [%s lines] - %s}\n' % (texify(subsection_name), number_of_lines, hash_value)
            tex += '\\inputminted{%s}{%s}\n' % (get_style(relative_path), texify_filename(relative_path))
        tex += '\n'
    return tex

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate ICPC notebook PDF from code/ using notebook.tex")
    parser.add_argument(
        "--clean",
        action="store_true",
        help="Remove generated files (_minted/, .latex_code/, LaTeX aux files, contents.tex). Keeps notebook.pdf.",
    )
    parser.add_argument(
        "--clean-all",
        action="store_true",
        help="Same as --clean, but also removes notebook.pdf.",
    )
    args = parser.parse_args()

    try:
        os.chdir(str(SCRIPT_DIR))

        if args.clean or args.clean_all:
            clean(also_pdf=bool(args.clean_all))
            print("Cleaned generated artifacts.")
            sys.exit(0)

        _require_file(NOTEBOOK_TEX, "LaTeX template")
        # Dependencies for minted
        _require_tool("latexmk", "Install via your TeX distribution (e.g., texlive + latexmk).")
        _require_tool("pdflatex", "Install via your TeX distribution (e.g., texlive-latex-base/texlive).")
        _require_tool("pygmentize", "Install Pygments (e.g., 'pip install Pygments' or distro package).")

        sections = get_sections()
        tex = get_tex(sections)
        CONTENTS_TEX.write_text(tex, encoding="utf-8")

        latexmk_options = [
            "latexmk",
            "-lualatex",
            "-shell-escape",
            "-cd",
            "-latexoption=-interaction=nonstopmode",
            "-latexoption=-halt-on-error",
            str(NOTEBOOK_TEX.name),
        ]
        print("Running latexmk... (this can take a bit on first run)")
        proc = subprocess.run(latexmk_options)
        if proc.returncode != 0:
            log_path = SCRIPT_DIR / "notebook.log"
            if log_path.exists():
                try:
                    tail = log_path.read_text(encoding="utf-8", errors="replace").splitlines()[-80:]
                    sys.stderr.write("\n--- notebook.log (last 80 lines) ---\n")
                    sys.stderr.write("\n".join(tail) + "\n")
                except OSError:
                    pass
            raise GenerationError(
                "LaTeX build failed (latexmk returned non-zero). "
                "Common fixes: install missing TeX packages; ensure -shell-escape is allowed; "
                "and that minted/Pygments is installed."
            )

        out_pdf = SCRIPT_DIR / "notebook.pdf"
        if out_pdf.exists():
            print(f"Generated: {out_pdf}")
        else:
            print("Build completed, but notebook.pdf was not found. Check latexmk output above.")
    except KeyboardInterrupt:
        print("Cancelled by user (Ctrl-C).", file=sys.stderr)
        sys.exit(130)
    except GenerationError as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(2)

