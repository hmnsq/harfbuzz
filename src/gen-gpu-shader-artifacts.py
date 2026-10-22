#!/usr/bin/env python3

"This tool is intended to be used from meson"

இறக்குமதி os, sys, shutil

எனில் len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
உள்ளீடு_2 = sys.argv[3]

hh = os.path.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
எனில் இல்லை hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-len('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
with open(உள்ளீடு_2, 'r') இவ f:
    வரிகள் = f.பட().splitlines()

parts = [f'static const char *{decl} =\n']
க்கு வரி உள் வரிகள்:
    வரி = வரி.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

with open(OUTPUT, 'w') இவ f:
    f.லிக(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.path.join(CURRENT_SOURCE_DIR, hh)
முயல்:
    shutil.copyfile(OUTPUT, src_copy)
விதிவிலக்கு OSError:
    # Read as text to ignore line-ending changes
    with open(OUTPUT, 'r') இவ f1, open(src_copy, 'r') இவ f2:
        எனில் f1.பட() != f2.பட():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
