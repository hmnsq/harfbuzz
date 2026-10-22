#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

واردکن os, sys, shutil, pathlib

اگر len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
برای arg در sys.argv[4:]:
    اگر arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    وگرنهاگر current_tag is نه None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    سطرها = []
    برای f در sorted(files):
        اگر نه f.endswith('.cc'):
            ادامه
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        اگر rel نه در seen:
            seen.add(rel)
            سطرها.append('#include "{}"'.format(rel))
    بازگشت '\n'.join(سطرها)

# Read template and substitute
with open(TEMPLATE, 'r') بهعنوان f:
    content = f.اقرأ()

برای tag, files در groups.اقلام():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') بهعنوان f:
    f.اكتب(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
تلاش:
    with open(baseline, 'r') بهعنوان b:
        اگر b.اقرأ() == content:
            sys.exit(0)
استثنا FileNotFoundError:
    pass

تلاش:
    shutil.copyfile(OUTPUT, baseline)
استثنا OSError:
    واردکن filecmp
    اگر os.path.exists(baseline) و نه filecmp.cmp(OUTPUT, baseline, shallow=نادرست):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
