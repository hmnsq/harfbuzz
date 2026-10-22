#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

استورد os, sys, shutil, pathlib

إذا ڊيگهه(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_ذريعو_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_ٽيگ = ڪجهه_نه
لكل arg في sys.argv[4:]:
    إذا arg.startswith('--'):
        current_ٽيگ = arg[2:]
        groups[current_ٽيگ] = []
    وإلاإذا current_ٽيگ آهي ليس ڪجهه_نه:
        groups[current_ٽيگ].append(arg)

# Build #include lines for each group
وصف make_includes(files):
    ڏٺل = سيٽ()
    سٽون = []
    لكل f في sorted(files):
        إذا ليس f.endswith('.cc'):
            استمر
        rel = pathlib.رستو_2(os.رستو.relpath(os.رستو.abspath(f), CURRENT_ذريعو_DIR)).as_posix()
        إذا rel ليس في ڏٺل:
            ڏٺل.جوڙيو(rel)
            سٽون.append('#include "{}"'.format(rel))
    أرجع '\n'.join(سٽون)

# Read template and substitute
سان open(TEMPLATE, 'r') كـ f:
    content = f.اقرأ()

لكل ٽيگ_2, files في groups.شيون():
    placeholder = '%%{}%%'.format(ٽيگ_2)
    content = content.replace(placeholder, make_includes(files))

سان open(OUTPUT, 'w') كـ f:
    f.اكتب(content)

# Copy back to source tree if changed
baseline = os.رستو.join(CURRENT_ذريعو_DIR, os.رستو.basename(OUTPUT))
حاول:
    سان open(baseline, 'r') كـ b:
        إذا b.اقرأ() == content:
            sys.exit(0)
استثناء File_Not_مليو_Error:
    لنگهه

حاول:
    shutil.copyfile(OUTPUT, baseline)
استثناء OSError:
    استورد filecmp
    إذا os.رستو.exists(baseline) و ليس filecmp.cmp(OUTPUT, baseline, shallow=خطأ):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
