#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

ഇറക്കുമതി os, sys, shutil, pathlib

എങ്കിൽ len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
വേണ്ടി arg ഇൽ sys.argv[4:]:
    എങ്കിൽ arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    അല്ലെങ്കിലെങ്കിൽ current_tag is അല്ല None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    വരികൾ = []
    വേണ്ടി f ഇൽ sorted(files):
        എങ്കിൽ അല്ല f.endswith('.cc'):
            തുടരുക
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        എങ്കിൽ rel അല്ല ഇൽ seen:
            seen.add(rel)
            വരികൾ.append('#include "{}"'.format(rel))
    തിരികെ '\n'.join(വരികൾ)

# Read template and substitute
with open(TEMPLATE, 'r') ഇവ f:
    content = f.പഠ()

വേണ്ടി tag, files ഇൽ groups.ഇനങ്ങൾ():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') ഇവ f:
    f.ലിഖ(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
ശ്രമിക്കുക:
    with open(baseline, 'r') ഇവ b:
        എങ്കിൽ b.പഠ() == content:
            sys.exit(0)
അപവാദം FileNotFoundError:
    pass

ശ്രമിക്കുക:
    shutil.copyfile(OUTPUT, baseline)
അപവാദം OSError:
    ഇറക്കുമതി filecmp
    എങ്കിൽ os.path.exists(baseline) ഒപ്പം അല്ല filecmp.cmp(OUTPUT, baseline, shallow=അസത്യം):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
