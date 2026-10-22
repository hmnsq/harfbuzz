#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

درآمد os, sys, shutil, pathlib

اگر لمبائی(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_ماخذ_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_لیبل = کچھ_نہیں
برائے arg میں sys.argv[4:]:
    اگر arg.startswith('--'):
        current_لیبل = arg[2:]
        groups[current_لیبل] = []
    وگرنہ_اگر current_لیبل ہے نہیں کچھ_نہیں:
        groups[current_لیبل].append(arg)

# Build #include lines for each group
تعریف make_includes(files):
    نظر_آیا = سیٹ()
    سطریں = []
    برائے f میں sorted(files):
        اگر نہیں f.endswith('.cc'):
            جاری
        rel = pathlib.راستہ_2(os.راستہ.relpath(os.راستہ.abspath(f), CURRENT_ماخذ_DIR)).as_posix()
        اگر rel نہیں میں نظر_آیا:
            نظر_آیا.جوڑو(rel)
            سطریں.append('#include "{}"'.format(rel))
    واپس '\n'.join(سطریں)

# Read template and substitute
ساتھ open(TEMPLATE, 'r') بطور f:
    content = f.اقرأ()

برائے لیبل_2, files میں groups.اشیاء():
    placeholder = '%%{}%%'.format(لیبل_2)
    content = content.replace(placeholder, make_includes(files))

ساتھ open(OUTPUT, 'w') بطور f:
    f.اكتب(content)

# Copy back to source tree if changed
baseline = os.راستہ.join(CURRENT_ماخذ_DIR, os.راستہ.basename(OUTPUT))
کوشش:
    ساتھ open(baseline, 'r') بطور b:
        اگر b.اقرأ() == content:
            sys.exit(0)
استثنا File_Not_ملا_Error:
    گزرو

کوشش:
    shutil.copyfile(OUTPUT, baseline)
استثنا OSError:
    درآمد filecmp
    اگر os.راستہ.exists(baseline) اور نہیں filecmp.cmp(OUTPUT, baseline, shallow=جھوٹ):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
