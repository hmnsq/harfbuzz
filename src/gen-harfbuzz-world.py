#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

ଆମଦାନୀ os, sys, shutil, pathlib

ଯଦି len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
ପାଇଁ arg ଭିତରେ sys.argv[4:]:
    ଯଦି arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    ନଚେତଯଦି current_tag is ନୁହେଁ None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    ଧାଡ଼ିଗୁଡ଼ିକ = []
    ପାଇଁ f ଭିତରେ sorted(files):
        ଯଦି ନୁହେଁ f.endswith('.cc'):
            ଜାରି
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        ଯଦି rel ନୁହେଁ ଭିତରେ seen:
            seen.add(rel)
            ଧାଡ଼ିଗୁଡ଼ିକ.append('#include "{}"'.format(rel))
    ଫେରନ୍ତୁ '\n'.join(ଧାଡ଼ିଗୁଡ଼ିକ)

# Read template and substitute
with open(TEMPLATE, 'r') ଇଵ f:
    content = f.ପଠ()

ପାଇଁ tag, files ଭିତରେ groups.ବସ୍ତୁଗୁଡ଼ିକ():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') ଇଵ f:
    f.ଲିଖ(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
ଚେଷ୍ଟା:
    with open(baseline, 'r') ଇଵ b:
        ଯଦି b.ପଠ() == content:
            sys.exit(0)
ବ୍ୟତିକ୍ରମ FileNotFoundError:
    pass

ଚେଷ୍ଟା:
    shutil.copyfile(OUTPUT, baseline)
ବ୍ୟତିକ୍ରମ OSError:
    ଆମଦାନୀ filecmp
    ଯଦି os.path.exists(baseline) ଏବଂ ନୁହେଁ filecmp.cmp(OUTPUT, baseline, shallow=ମିଥ୍ୟା):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
