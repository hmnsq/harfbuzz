#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

استورد os, sys, shutil, pathlib

إذا طول(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_منبع_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_برچسب = هیچ
لكل arg في sys.argv[4:]:
    إذا arg.startswith('--'):
        current_برچسب = arg[2:]
        groups[current_برچسب] = []
    وإلاإذا current_برچسب است ليس هیچ:
        groups[current_برچسب].append(arg)

# Build #include lines for each group
تعریف make_includes(files):
    دیده = مجموعه()
    سطرها = []
    لكل f في sorted(files):
        إذا ليس f.endswith('.cc'):
            استمر
        rel = pathlib.مسیر_2(os.مسیر.relpath(os.مسیر.abspath(f), CURRENT_منبع_DIR)).as_posix()
        إذا rel ليس في دیده:
            دیده.جمع_کن(rel)
            سطرها.append('#include "{}"'.format(rel))
    أرجع '\n'.join(سطرها)

# Read template and substitute
با open(TEMPLATE, 'r') كـ f:
    content = f.اقرأ()

لكل برچسب_2, files في groups.اقلام():
    placeholder = '%%{}%%'.format(برچسب_2)
    content = content.replace(placeholder, make_includes(files))

با open(OUTPUT, 'w') كـ f:
    f.اكتب(content)

# Copy back to source tree if changed
baseline = os.مسیر.join(CURRENT_منبع_DIR, os.مسیر.basename(OUTPUT))
حاول:
    با open(baseline, 'r') كـ b:
        إذا b.اقرأ() == content:
            sys.exit(0)
استثناء File_Not_یافت_Error:
    بگذر

حاول:
    shutil.copyfile(OUTPUT, baseline)
استثناء OSError:
    استورد filecmp
    إذا os.مسیر.exists(baseline) و ليس filecmp.cmp(OUTPUT, baseline, shallow=خطأ):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
