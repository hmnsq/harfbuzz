#!/usr/bin/env python3

"This tool is intended to be used from meson"

واردکن os, sys, shutil

اگر len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
درون_داد_2 = sys.argv[3]

hh = os.path.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
اگر نه hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-len('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
with open(درون_داد_2, 'r') بهعنوان f:
    سطرها = f.اقرأ().splitlines()

parts = [f'static const char *{decl} =\n']
برای سطر در سطرها:
    سطر = سطر.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

with open(OUTPUT, 'w') بهعنوان f:
    f.اكتب(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.path.join(CURRENT_SOURCE_DIR, hh)
تلاش:
    shutil.copyfile(OUTPUT, src_copy)
استثنا OSError:
    # Read as text to ignore line-ending changes
    with open(OUTPUT, 'r') بهعنوان f1, open(src_copy, 'r') بهعنوان f2:
        اگر f1.اقرأ() != f2.اقرأ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
