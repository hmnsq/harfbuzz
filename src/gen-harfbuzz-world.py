#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

استورد os, sys, shutil, pathlib

إذا len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
لكل arg في sys.argv[4:]:
    إذا arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    وإلاإذا current_tag is ليس None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    هێڵەکان = []
    لكل f في sorted(files):
        إذا ليس f.endswith('.cc'):
            استمر
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        إذا rel ليس في seen:
            seen.add(rel)
            هێڵەکان.append('#include "{}"'.format(rel))
    أرجع '\n'.join(هێڵەکان)

# Read template and substitute
with open(TEMPLATE, 'r') كـ f:
    content = f.اقرأ()

لكل tag, files في groups.کاڵاکان():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') كـ f:
    f.اكتب(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
حاول:
    with open(baseline, 'r') كـ b:
        إذا b.اقرأ() == content:
            sys.exit(0)
استثناء FileNotFoundError:
    pass

حاول:
    shutil.copyfile(OUTPUT, baseline)
استثناء OSError:
    استورد filecmp
    إذا os.path.exists(baseline) و ليس filecmp.cmp(OUTPUT, baseline, shallow=خطأ):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
