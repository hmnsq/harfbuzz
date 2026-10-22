#!/usr/bin/env python3

"This tool is intended to be used from meson"

ഇറക്കുമതി os, sys, shutil

എങ്കിൽ len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
നിവേശം_2 = sys.argv[3]

hh = os.path.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
എങ്കിൽ അല്ല hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-len('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
with open(നിവേശം_2, 'r') ഇവ f:
    വരികൾ = f.പഠ().splitlines()

parts = [f'static const char *{decl} =\n']
വേണ്ടി വരി ഇൽ വരികൾ:
    വരി = വരി.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

with open(OUTPUT, 'w') ഇവ f:
    f.ലിഖ(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.path.join(CURRENT_SOURCE_DIR, hh)
ശ്രമിക്കുക:
    shutil.copyfile(OUTPUT, src_copy)
അപവാദം OSError:
    # Read as text to ignore line-ending changes
    with open(OUTPUT, 'r') ഇവ f1, open(src_copy, 'r') ഇവ f2:
        എങ്കിൽ f1.പഠ() != f2.പഠ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
