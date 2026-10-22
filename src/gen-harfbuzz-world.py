#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

आयात os, sys, shutil, pathlib

अगर len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
क्रम arg in sys.argv[4:]:
    अगर arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    elif current_tag is not None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    पंक्तियाँ = []
    क्रम f in sorted(files):
        अगर not f.endswith('.cc'):
            जारी
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        अगर rel not in seen:
            seen.add(rel)
            पंक्तियाँ.append('#include "{}"'.format(rel))
    वापस '\n'.मेल(पंक्तियाँ)

# Read template and substitute
with open(TEMPLATE, 'r') as f:
    content = f.म_पढ़ो()

क्रम tag, files in groups.वस्तुएँ():
    placeholder = '%%{}%%'.format(tag)
    content = content.प्रतिस्थापन(placeholder, make_includes(files))

with open(OUTPUT, 'w') as f:
    f.म_लिखो(content)

# Copy back to source tree if changed
baseline = os.path.मेल(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
प्रयास:
    with open(baseline, 'r') as b:
        अगर b.म_पढ़ो() == content:
            sys.exit(0)
except FileNotFoundError:
    pass

प्रयास:
    shutil.copyfile(OUTPUT, baseline)
except OSError:
    आयात filecmp
    अगर os.path.exists(baseline) and not filecmp.cmp(OUTPUT, baseline, shallow=False):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
