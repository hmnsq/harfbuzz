#!/usr/bin/env python3

"This tool is intended to be used from meson"

import os, sys, shutil

if len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
पासोक_2 = sys.argv[3]

hh = os.path.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
if not hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-len('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
with open(पासोक_2, 'r') as f:
    मगा_लिन्या = f.read().splitlines()

parts = [f'static const char *{decl} =\n']
for लिन्या in मगा_लिन्या:
    लिन्या = लिन्या.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

with open(OUTPUT, 'w') as f:
    f.write(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.path.join(CURRENT_SOURCE_DIR, hh)
try:
    shutil.copyfile(OUTPUT, src_copy)
except OSError:
    # Read as text to ignore line-ending changes
    with open(OUTPUT, 'r') as f1, open(src_copy, 'r') as f2:
        if f1.read() != f2.read():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
