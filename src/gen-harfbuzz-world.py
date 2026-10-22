#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

இறக்குமதி os, sys, shutil, pathlib

எனில் len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
க்கு arg உள் sys.argv[4:]:
    எனில் arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    அல்லதெனில் current_tag is இல்லை None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    வரிகள் = []
    க்கு f உள் sorted(files):
        எனில் இல்லை f.endswith('.cc'):
            தொடர்
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        எனில் rel இல்லை உள் seen:
            seen.add(rel)
            வரிகள்.append('#include "{}"'.format(rel))
    திரும்பு '\n'.join(வரிகள்)

# Read template and substitute
with open(TEMPLATE, 'r') இவ f:
    content = f.பட()

க்கு tag, files உள் groups.பொருட்கள்():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') இவ f:
    f.லிக(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
முயல்:
    with open(baseline, 'r') இவ b:
        எனில் b.பட() == content:
            sys.exit(0)
விதிவிலக்கு FileNotFoundError:
    pass

முயல்:
    shutil.copyfile(OUTPUT, baseline)
விதிவிலக்கு OSError:
    இறக்குமதி filecmp
    எனில் os.path.exists(baseline) மற்றும் இல்லை filecmp.cmp(OUTPUT, baseline, shallow=பொய்):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
