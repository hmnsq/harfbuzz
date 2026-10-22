#!/usr/bin/env python3

"This tool is intended to be used from meson"

आनयन os, sys, shutil

यदि len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
निवेश_2 = sys.argv[3]

hh = os.path.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
यदि न hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-len('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
with open(निवेश_2, 'r') इव f:
    पंक्तियाँ = f.पठ().splitlines()

parts = [f'static const char *{decl} =\n']
प्रति पंक्ति अन्तः पंक्तियाँ:
    पंक्ति = पंक्ति.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

with open(OUTPUT, 'w') इव f:
    f.लिख(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.path.join(CURRENT_SOURCE_DIR, hh)
यत्न:
    shutil.copyfile(OUTPUT, src_copy)
अपवाद OSError:
    # Read as text to ignore line-ending changes
    with open(OUTPUT, 'r') इव f1, open(src_copy, 'r') इव f2:
        यदि f1.पठ() != f2.पठ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
