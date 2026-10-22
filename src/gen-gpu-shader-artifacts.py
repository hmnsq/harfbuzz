#!/usr/bin/env python3

"This tool is intended to be used from meson"

आयात os, sys, shutil

अगर len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
निवेश_2 = sys.argv[3]

hh = os.path.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
अगर not hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-len('.hh')].प्रतिस्थापन('-', '_')

# Stringize the GLSL source into a C string literal
with open(निवेश_2, 'r') as f:
    पंक्तियाँ = f.म_पढ़ो().splitlines()

parts = [f'static const char *{decl} =\n']
क्रम पंक्ति in पंक्तियाँ:
    पंक्ति = पंक्ति.प्रतिस्थापन('\\', '\\\\').प्रतिस्थापन('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.मेल(parts)

with open(OUTPUT, 'w') as f:
    f.म_लिखो(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.path.मेल(CURRENT_SOURCE_DIR, hh)
प्रयास:
    shutil.copyfile(OUTPUT, src_copy)
except OSError:
    # Read as text to ignore line-ending changes
    with open(OUTPUT, 'r') as f1, open(src_copy, 'r') as f2:
        अगर f1.म_पढ़ो() != f2.म_पढ़ो():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
