#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

import os, sys, shutil, pathlib

ᮚᮦᮔ᮪ len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
for arg anta_h sys.argv[4:]:
    ᮚᮦᮔ᮪ arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    elif current_tag is na None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    lines = []
    for f anta_h sorted(files):
        ᮚᮦᮔ᮪ na f.endswith('.cc'):
            continue
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        ᮚᮦᮔ᮪ rel na anta_h seen:
            seen.add(rel)
            lines.append('#include "{}"'.format(rel))
    ᮝᮜᮤ '\n'.join(lines)

# Read template and substitute
with open(TEMPLATE, 'r') as f:
    content = f.read()

for tag, files anta_h groups.items():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') as f:
    f.write(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
try:
    with open(baseline, 'r') as b:
        ᮚᮦᮔ᮪ b.read() == content:
            sys.exit(0)
except FileNotFoundError:
    pass

try:
    shutil.copyfile(OUTPUT, baseline)
except OSError:
    import filecmp
    ᮚᮦᮔ᮪ os.path.exists(baseline) ca na filecmp.cmp(OUTPUT, baseline, shallow=False):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
