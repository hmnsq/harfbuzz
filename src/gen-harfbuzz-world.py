#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

আনযন os, sys, shutil, pathlib

যদি len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_tag = None
প্রতি arg অন্তঃ sys.argv[4:]:
    যদি arg.startswith('--'):
        current_tag = arg[2:]
        groups[current_tag] = []
    নোচেদ্যদি current_tag is ন None:
        groups[current_tag].append(arg)

# Build #include lines for each group
def make_includes(files):
    seen = set()
    সারিগুলি = []
    প্রতি f অন্তঃ sorted(files):
        যদি ন f.endswith('.cc'):
            অনুবর্ত
        rel = pathlib.Path(os.path.relpath(os.path.abspath(f), CURRENT_SOURCE_DIR)).as_posix()
        যদি rel ন অন্তঃ seen:
            seen.add(rel)
            সারিগুলি.append('#include "{}"'.format(rel))
    প্রত্যাগম '\n'.join(সারিগুলি)

# Read template and substitute
with open(TEMPLATE, 'r') ইব f:
    content = f.পঠ()

প্রতি tag, files অন্তঃ groups.বস্তুসমূহ():
    placeholder = '%%{}%%'.format(tag)
    content = content.replace(placeholder, make_includes(files))

with open(OUTPUT, 'w') ইব f:
    f.লিখ(content)

# Copy back to source tree if changed
baseline = os.path.join(CURRENT_SOURCE_DIR, os.path.basename(OUTPUT))
যত্ন:
    with open(baseline, 'r') ইব b:
        যদি b.পঠ() == content:
            sys.exit(0)
অপবাদ FileNotFoundError:
    pass

যত্ন:
    shutil.copyfile(OUTPUT, baseline)
অপবাদ OSError:
    আনযন filecmp
    যদি os.path.exists(baseline) চ ন filecmp.cmp(OUTPUT, baseline, shallow=অসত্য):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
