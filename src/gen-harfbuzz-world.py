#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

استورد os, sys, shutil, pathlib

إذا لمبائی(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_سروت_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_ٹیگ = کجھ_نئیں
لكل arg في sys.argv[4:]:
    إذا arg.startswith('--'):
        current_ٹیگ = arg[2:]
        groups[current_ٹیگ] = []
    وإلاإذا current_ٹیگ اے ليس کجھ_نئیں:
        groups[current_ٹیگ].append(arg)

# Build #include lines for each group
تعریف make_includes(files):
    تکیا = سیٹ()
    سطریں = []
    لكل f في sorted(files):
        إذا ليس f.endswith('.cc'):
            استمر
        rel = pathlib.رستہ_2(os.رستہ.relpath(os.رستہ.abspath(f), CURRENT_سروت_DIR)).as_posix()
        إذا rel ليس في تکیا:
            تکیا.جوڑو(rel)
            سطریں.append('#include "{}"'.format(rel))
    أرجع '\n'.join(سطریں)

# Read template and substitute
نال open(TEMPLATE, 'r') كـ f:
    content = f.اقرأ()

لكل ٹیگ_2, files في groups.اشیاء():
    placeholder = '%%{}%%'.format(ٹیگ_2)
    content = content.replace(placeholder, make_includes(files))

نال open(OUTPUT, 'w') كـ f:
    f.اكتب(content)

# Copy back to source tree if changed
baseline = os.رستہ.join(CURRENT_سروت_DIR, os.رستہ.basename(OUTPUT))
حاول:
    نال open(baseline, 'r') كـ b:
        إذا b.اقرأ() == content:
            sys.exit(0)
استثناء File_Not_لبھیا_Error:
    لنگھو

حاول:
    shutil.copyfile(OUTPUT, baseline)
استثناء OSError:
    استورد filecmp
    إذا os.رستہ.exists(baseline) و ليس filecmp.cmp(OUTPUT, baseline, shallow=خطأ):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
