#!/usr/bin/env python3

"This tool is intended to be used from meson"

આયાત os, sys, shutil

જો len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
આગત_2 = sys.argv[3]

hh = os.path.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
જો નહીં hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-len('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
with open(આગત_2, 'r') ઇવ f:
    લીટીઓ = f.પઠ().splitlines()

parts = [f'static const char *{decl} =\n']
માટે લીટી માં લીટીઓ:
    લીટી = લીટી.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

with open(OUTPUT, 'w') ઇવ f:
    f.લિખ(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.path.join(CURRENT_SOURCE_DIR, hh)
પ્રયાસ:
    shutil.copyfile(OUTPUT, src_copy)
અપવાદ OSError:
    # Read as text to ignore line-ending changes
    with open(OUTPUT, 'r') ઇવ f1, open(src_copy, 'r') ઇવ f2:
        જો f1.પઠ() != f2.પઠ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
