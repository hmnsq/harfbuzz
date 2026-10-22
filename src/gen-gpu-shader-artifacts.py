#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil

إذا ڊيگهه(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_ذريعو_DIR = sys.argv[2]
داخلا_3 = sys.argv[3]

hh = os.رستو.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
إذا ليس hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-ڊيگهه('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
سان open(داخلا_3, 'r') كـ f:
    سٽون = f.اقرأ().splitlines()

parts = [f'static const char *{decl} =\n']
لكل سٽ في سٽون:
    سٽ = سٽ.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

سان open(OUTPUT, 'w') كـ f:
    f.اكتب(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.رستو.join(CURRENT_ذريعو_DIR, hh)
حاول:
    shutil.copyfile(OUTPUT, src_copy)
استثناء OSError:
    # Read as text to ignore line-ending changes
    سان open(OUTPUT, 'r') كـ f1, open(src_copy, 'r') كـ f2:
        إذا f1.اقرأ() != f2.اقرأ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
