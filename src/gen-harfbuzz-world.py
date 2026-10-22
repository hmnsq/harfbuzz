#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

ಆನಯನ os, sys, shutil, pathlib

ಯದಿ len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
ಪ್ರತಿ arg ಅನ್ತಃ sys.argv[4:]:
    ಯದಿ arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    ನೋಚೇದ್ಯದಿ current_tag is ನ None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    ಸಾಲುಗಳು = []
    ಪ್ರತಿ f ಅನ್ತಃ sorted(files):
        ಯದಿ ನ f.endswith('.cc'):
            ಅನುವರ್ತ
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        ಯದಿ rel ನ ಅನ್ತಃ seen:
            seen.add(rel)
            ಸಾಲುಗಳು.append('#include "{}"'.format(rel))
    ಪ್ರತ್ಯಾಗಮ '\n'.join(ಸಾಲುಗಳು)

# Read template and substitute
with open(TEMPLATE, 'r') ಇವ f:
    content = f.ಪಠ()

ಪ್ರತಿ tag, files ಅನ್ತಃ groups.ವಸ್ತುಗಳು():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') ಇವ f:
    f.ಲಿಖ(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
ಯತ್ನ:
    with open(baseline, 'r') ಇವ b:
        ಯದಿ b.ಪಠ() == content:
            sys.exit(0)
ಅಪವಾದ FileNotFoundError:
    pass

ಯತ್ನ:
    shutil.copyfile(OUTPUT, baseline)
ಅಪವಾದ OSError:
    ಆನಯನ filecmp
    ಯದಿ os.path.exists(baseline) ಚ ನ filecmp.cmp(OUTPUT, baseline, shallow=ಅಸತ್ಯ):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
