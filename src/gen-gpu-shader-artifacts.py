#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil

إذا اوږدوالی(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_سرچینه_DIR = sys.argv[2]
ننوت_2 = sys.argv[3]

hh = os.لار.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
إذا ليس hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-اوږدوالی('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
سره open(ننوت_2, 'r') كـ f:
    کرښې = f.اقرأ().splitlines()

parts = [f'static const char *{decl} =\n']
لكل کرښه في کرښې:
    کرښه = کرښه.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

سره open(OUTPUT, 'w') كـ f:
    f.اكتب(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.لار.join(CURRENT_سرچینه_DIR, hh)
حاول:
    shutil.copyfile(OUTPUT, src_copy)
استثناء OSError:
    # Read as text to ignore line-ending changes
    سره open(OUTPUT, 'r') كـ f1, open(src_copy, 'r') كـ f2:
        إذا f1.اقرأ() != f2.اقرأ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
