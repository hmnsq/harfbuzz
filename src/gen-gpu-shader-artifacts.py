#!/usr/bin/env python3

"This tool is intended to be used from meson"

درآمد os, sys, shutil

اگر لمبائی(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_ماخذ_DIR = sys.argv[2]
داخلہ_2 = sys.argv[3]

hh = os.راستہ.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
اگر نہیں hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-لمبائی('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
ساتھ open(داخلہ_2, 'r') بطور f:
    سطریں = f.اقرأ().splitlines()

parts = [f'static const char *{decl} =\n']
برائے سطر میں سطریں:
    سطر = سطر.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

ساتھ open(OUTPUT, 'w') بطور f:
    f.اكتب(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.راستہ.join(CURRENT_ماخذ_DIR, hh)
کوشش:
    shutil.copyfile(OUTPUT, src_copy)
استثنا OSError:
    # Read as text to ignore line-ending changes
    ساتھ open(OUTPUT, 'r') بطور f1, open(src_copy, 'r') بطور f2:
        اگر f1.اقرأ() != f2.اقرأ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
