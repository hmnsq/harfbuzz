#!/usr/bin/env python3

"This tool is intended to be used from meson"

ಆನಯನ os, sys, shutil

ಯದಿ len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
ಒಳಹರಿವು_2 = sys.argv[3]

hh = os.path.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
ಯದಿ ನ hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-len('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
with open(ಒಳಹರಿವು_2, 'r') ಇವ f:
    ಸಾಲುಗಳು = f.ಪಠ().splitlines()

parts = [f'static const char *{decl} =\n']
ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ ಸಾಲುಗಳು:
    ಸಾಲು = ಸಾಲು.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

with open(OUTPUT, 'w') ಇವ f:
    f.ಲಿಖ(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.path.join(CURRENT_SOURCE_DIR, hh)
ಯತ್ನ:
    shutil.copyfile(OUTPUT, src_copy)
ಅಪವಾದ OSError:
    # Read as text to ignore line-ending changes
    with open(OUTPUT, 'r') ಇವ f1, open(src_copy, 'r') ಇವ f2:
        ಯದಿ f1.ಪಠ() != f2.ಪಠ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
