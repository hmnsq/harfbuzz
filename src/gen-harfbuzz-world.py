#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

આયાત os, sys, shutil, pathlib

જો len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
માટે arg માં sys.argv[4:]:
    જો arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    અથવાજો current_tag is નહીં None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    લીટીઓ = []
    માટે f માં sorted(files):
        જો નહીં f.endswith('.cc'):
            ચાલુ
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        જો rel નહીં માં seen:
            seen.add(rel)
            લીટીઓ.append('#include "{}"'.format(rel))
    પાછા '\n'.join(લીટીઓ)

# Read template and substitute
with open(TEMPLATE, 'r') ઇવ f:
    content = f.પઠ()

માટે tag, files માં groups.વસ્તુઓ():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') ઇવ f:
    f.લિખ(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
પ્રયાસ:
    with open(baseline, 'r') ઇવ b:
        જો b.પઠ() == content:
            sys.exit(0)
અપવાદ FileNotFoundError:
    pass

પ્રયાસ:
    shutil.copyfile(OUTPUT, baseline)
અપવાદ OSError:
    આયાત filecmp
    જો os.path.exists(baseline) અને નહીં filecmp.cmp(OUTPUT, baseline, shallow=ખોટું):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
