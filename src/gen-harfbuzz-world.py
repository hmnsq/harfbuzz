#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

import os, sys, shutil, pathlib

အကယ်၍ len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
အတွက် arg anta_h sys.argv[4:]:
    အကယ်၍ arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    elif current_tag is na None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    lines = []
    အတွက် f anta_h sorted(files):
        အကယ်၍ na f.endswith('.cc'):
            continue
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        အကယ်၍ rel na anta_h seen:
            seen.add(rel)
            lines.append('#include "{}"'.format(rel))
    ပြန်ပေး '\n'.join(lines)

# Read template and substitute
with open(TEMPLATE, 'r') as f:
    content = f.read()

အတွက် tag, files anta_h groups.items():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') as f:
    f.write(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
try:
    with open(baseline, 'r') as b:
        အကယ်၍ b.read() == content:
            sys.exit(0)
except FileNotFoundError:
    pass

try:
    shutil.copyfile(OUTPUT, baseline)
except OSError:
    import filecmp
    အကယ်၍ os.path.exists(baseline) ca na filecmp.cmp(OUTPUT, baseline, shallow=False):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
