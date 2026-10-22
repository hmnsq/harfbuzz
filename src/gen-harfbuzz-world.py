#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

దిగుమతి os, sys, shutil, pathlib

ఒకవేళ పొడవు(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_ఆధారం_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_ట్యాగ్ = ఏమీలేదు
కొరకు arg లో sys.argv[4:]:
    ఒకవేళ arg.startswith('--'):
        current_ట్యాగ్ = arg[2:]
        groups[current_ట్యాగ్] = []
    లేదావేళ current_ట్యాగ్ అయిన కాదు ఏమీలేదు:
        groups[current_ట్యాగ్].append(arg)

# Build #include lines for each group
నిర్వచనం make_includes(files):
    చూసిన = సమితి()
    పంక్తులు = []
    కొరకు f లో sorted(files):
        ఒకవేళ కాదు f.endswith('.cc'):
            కొనసాగు
        rel = pathlib.మార్గం_2(os.మార్గం.relpath(os.మార్గం.abspath(f), CURRENT_ఆధారం_DIR)).as_posix()
        ఒకవేళ rel కాదు లో చూసిన:
            చూసిన.కూడు(rel)
            పంక్తులు.append('#include "{}"'.format(rel))
    తిరిగి '\n'.join(పంక్తులు)

# Read template and substitute
తోడుగా open(TEMPLATE, 'r') ఇవ f:
    content = f.పఠ()

కొరకు ట్యాగ్_2, files లో groups.వస్తువులు():
    placeholder = '%%{}%%'.format(ట్యాగ్_2)
    content = content.replace(placeholder, make_includes(files))

తోడుగా open(OUTPUT, 'w') ఇవ f:
    f.లిఖ(content)

# Copy back to source tree if changed
baseline = os.మార్గం.join(CURRENT_ఆధారం_DIR, os.మార్గం.basename(OUTPUT))
ప్రయత్నించు:
    తోడుగా open(baseline, 'r') ఇవ b:
        ఒకవేళ b.పఠ() == content:
            sys.exit(0)
మినహాయింపు File_Not_దొరికింది_Error:
    దాటు

ప్రయత్నించు:
    shutil.copyfile(OUTPUT, baseline)
మినహాయింపు OSError:
    దిగుమతి filecmp
    ఒకవేళ os.మార్గం.exists(baseline) మరియు కాదు filecmp.cmp(OUTPUT, baseline, shallow=అబద్ధం):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
