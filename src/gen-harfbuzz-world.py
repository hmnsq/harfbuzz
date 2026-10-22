#!/usr/bin/env python3

"""Generate harfbuzz-world.cc from a template and tagged source lists.

Usage:
  gen-harfbuzz-world.py OUTPUT SRCDIR TEMPLATE --TAG file1.cc file2.cc --TAG2 ...

Each --TAG starts a new group.  The template file has %%TAG%%
placeholders that get replaced with #include lines for that group.
"""

ਆਯਾਤ os, sys, shutil, pathlib

ਜੇ ਲੰਬਾਈ(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_ਸਰੋਤ_DIR = sys.argv[2]
TEMPLATE = sys.argv[3]

# Parse --TAG file... groups
groups = {}
current_ਟੈਗ = ਕੁਝ_ਨਹੀਂ
ਲਈ arg ਵਿੱਚ sys.argv[4:]:
    ਜੇ arg.startswith('--'):
        current_ਟੈਗ = arg[2:]
        groups[current_ਟੈਗ] = []
    ਜਾਂਜੇ current_ਟੈਗ ਹੈ ਨਹੀਂ ਕੁਝ_ਨਹੀਂ:
        groups[current_ਟੈਗ].append(arg)

# Build #include lines for each group
ਪਰਿਭਾਸ਼ਾ make_includes(files):
    ਵੇਖਿਆ = ਸੈੱਟ()
    ਸਤਰਾਂ = []
    ਲਈ f ਵਿੱਚ sorted(files):
        ਜੇ ਨਹੀਂ f.endswith('.cc'):
            ਜਾਰੀ
        rel = pathlib.ਰਸਤਾ_2(os.ਰਸਤਾ.relpath(os.ਰਸਤਾ.abspath(f), CURRENT_ਸਰੋਤ_DIR)).as_posix()
        ਜੇ rel ਨਹੀਂ ਵਿੱਚ ਵੇਖਿਆ:
            ਵੇਖਿਆ.ਜੋੜੋ(rel)
            ਸਤਰਾਂ.append('#include "{}"'.format(rel))
    ਵਾਪਸ '\n'.join(ਸਤਰਾਂ)

# Read template and substitute
ਨਾਲ open(TEMPLATE, 'r') ਇਵ f:
    content = f.ਪਠ()

ਲਈ ਟੈਗ_2, files ਵਿੱਚ groups.ਚੀਜ਼ਾਂ():
    placeholder = '%%{}%%'.format(ਟੈਗ_2)
    content = content.replace(placeholder, make_includes(files))

ਨਾਲ open(OUTPUT, 'w') ਇਵ f:
    f.ਲਿਖ(content)

# Copy back to source tree if changed
baseline = os.ਰਸਤਾ.join(CURRENT_ਸਰੋਤ_DIR, os.ਰਸਤਾ.basename(OUTPUT))
ਕੋਸ਼ਿਸ਼:
    ਨਾਲ open(baseline, 'r') ਇਵ b:
        ਜੇ b.ਪਠ() == content:
            sys.exit(0)
ਅਪਵਾਦ File_Not_ਮਿਲਿਆ_Error:
    ਲੰਘੋ

ਕੋਸ਼ਿਸ਼:
    shutil.copyfile(OUTPUT, baseline)
ਅਪਵਾਦ OSError:
    ਆਯਾਤ filecmp
    ਜੇ os.ਰਸਤਾ.exists(baseline) ਅਤੇ ਨਹੀਂ filecmp.cmp(OUTPUT, baseline, shallow=ਝੂਠ):
        sys.exit('{} is out of date; regenerate with a writable source tree'.format(baseline))
