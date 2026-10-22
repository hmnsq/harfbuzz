#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

استورد os, sys, shutil, pathlib

إذا اوږدوالی(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_سرچینه_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_نښان = هیڅ
لكل arg في sys.argv[4:]:
    إذا arg.startswith('--'):
        current_نښان = arg[2:]
        groups[current_نښان] = []
    وإلاإذا current_نښان دی ليس هیڅ:
        groups[current_نښان].append(arg)

# Build #include lines for each group
تعریف make_includes(files):
    کتل_شوی = سېټ()
    کرښې = []
    لكل f في sorted(files):
        إذا ليس f.endswith('.cc'):
            استمر
        rel = pathlib.لار_2(os.لار.relpath(os.لار.abspath(f), CURRENT_سرچینه_DIR)).as_posix()
        إذا rel ليس في کتل_شوی:
            کتل_شوی.جمع_کړه(rel)
            کرښې.append('#include "{}"'.format(rel))
    أرجع '\n'.join(کرښې)

# Read template and substitute
سره open(TEMPLATE, 'r') كـ f:
    content = f.اقرأ()

لكل نښان_2, files في groups.توکي():
    placeholder = '%%{}%%'.format(نښان_2)
    content = content.replace(placeholder, make_includes(files))

سره open(OUTPUT, 'w') كـ f:
    f.اكتب(content)

# Copy back to source tree if changed
baseline = os.لار.join(CURRENT_سرچینه_DIR, os.لار.basename(OUTPUT))
حاول:
    سره open(baseline, 'r') كـ b:
        إذا b.اقرأ() == content:
            sys.exit(0)
استثناء File_Not_وموندل_Error:
    تیرشه

حاول:
    shutil.copyfile(OUTPUT, baseline)
استثناء OSError:
    استورد filecmp
    إذا os.لار.exists(baseline) و ليس filecmp.cmp(OUTPUT, baseline, shallow=خطأ):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
