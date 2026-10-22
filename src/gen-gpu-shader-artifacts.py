#!/usr/bin/env python3

"This tool is intended to be used from meson"

ଆମଦାନୀ os, sys, shutil

ଯଦି len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
ନିବେଶ_2 = sys.argv[3]

hh = os.path.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
ଯଦି ନୁହେଁ hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-len('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
with open(ନିବେଶ_2, 'r') ଇଵ f:
    ଧାଡ଼ିଗୁଡ଼ିକ = f.ପଠ().splitlines()

parts = [f'static const char *{decl} =\n']
ପାଇଁ ଧାଡ଼ି ଭିତରେ ଧାଡ଼ିଗୁଡ଼ିକ:
    ଧାଡ଼ି = ଧାଡ଼ି.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

with open(OUTPUT, 'w') ଇଵ f:
    f.ଲିଖ(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.path.join(CURRENT_SOURCE_DIR, hh)
ଚେଷ୍ଟା:
    shutil.copyfile(OUTPUT, src_copy)
ବ୍ୟତିକ୍ରମ OSError:
    # Read as text to ignore line-ending changes
    with open(OUTPUT, 'r') ଇଵ f1, open(src_copy, 'r') ଇଵ f2:
        ଯଦି f1.ପଠ() != f2.ପଠ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
