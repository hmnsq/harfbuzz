#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

आयात os, sys, shutil, pathlib

जर len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
साठी arg मध्ये sys.argv[4:]:
    जर arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    किंवाजर current_tag is नाही None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    ओळी = []
    साठी f मध्ये sorted(files):
        जर नाही f.endswith('.cc'):
            सुरूठेवा
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        जर rel नाही मध्ये seen:
            seen.add(rel)
            ओळी.append('#include "{}"'.format(rel))
    परत '\n'.join(ओळी)

# Read template and substitute
with open(TEMPLATE, 'r') इव f:
    content = f.पठ()

साठी tag, files मध्ये groups.वस्तुसंच():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') इव f:
    f.लिख(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
प्रयत्न:
    with open(baseline, 'r') इव b:
        जर b.पठ() == content:
            sys.exit(0)
अपवाद FileNotFoundError:
    pass

प्रयत्न:
    shutil.copyfile(OUTPUT, baseline)
अपवाद OSError:
    आयात filecmp
    जर os.path.exists(baseline) आणि नाही filecmp.cmp(OUTPUT, baseline, shallow=असत्य):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
