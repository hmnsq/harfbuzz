#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

استورد os, sys, shutil, pathlib

𐤀𐤌 len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
لكل arg في sys.argv[4:]:
    𐤀𐤌 arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    وإلاإذا current_tag is 𐤋𐤀 None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    lines = []
    لكل f في sorted(files):
        𐤀𐤌 𐤋𐤀 f.endswith('.cc'):
            استمر
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        𐤀𐤌 rel 𐤋𐤀 في seen:
            seen.add(rel)
            lines.append('#include "{}"'.format(rel))
    أرجع '\n'.join(lines)

# Read template and substitute
with open(TEMPLATE, 'r') كـ f:
    content = f.اقرأ()

لكل tag, files في groups.items():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') كـ f:
    f.اكتب(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
حاول:
    with open(baseline, 'r') كـ b:
        𐤀𐤌 b.اقرأ() == content:
            sys.exit(0)
استثناء FileNotFoundError:
    pass

حاول:
    shutil.copyfile(OUTPUT, baseline)
استثناء OSError:
    استورد filecmp
    𐤀𐤌 os.path.exists(baseline) 𐤅 𐤋𐤀 filecmp.cmp(OUTPUT, baseline, shallow=خطأ):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
