#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

आनयन os, sys, shutil, pathlib

सचे len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
अत्थाय arg अन्तः sys.argv[4:]:
    सचे arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    नोचेद्यदि current_tag is न None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    पङ्क्तयः = []
    अत्थाय f अन्तः sorted(files):
        सचे न f.endswith('.cc'):
            अनुवर्त
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        सचे rel न अन्तः seen:
            seen.add(rel)
            पङ्क्तयः.append('#include "{}"'.format(rel))
    पटिनिवत्त '\n'.join(पङ्क्तयः)

# Read template and substitute
with open(TEMPLATE, 'r') इव f:
    content = f.पठ()

अत्थाय tag, files अन्तः groups.वस्तूनि():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') इव f:
    f.लिख(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
यत्न:
    with open(baseline, 'r') इव b:
        सचे b.पठ() == content:
            sys.exit(0)
अपवाद FileNotFoundError:
    pass

यत्न:
    shutil.copyfile(OUTPUT, baseline)
अपवाद OSError:
    आनयन filecmp
    सचे os.path.exists(baseline) च न filecmp.cmp(OUTPUT, baseline, shallow=मिच्छा):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
