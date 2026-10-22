#!/usr/bin/env python3

"This tool is intended to be used from meson"

ਆਯਾਤ os, sys, shutil

ਜੇ ਲੰਬਾਈ(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_ਸਰੋਤ_DIR = sys.argv[2]
ਨਿਵੇਸ਼_2 = sys.argv[3]

hh = os.ਰਸਤਾ.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
ਜੇ ਨਹੀਂ hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-ਲੰਬਾਈ('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
ਨਾਲ open(ਨਿਵੇਸ਼_2, 'r') ਇਵ f:
    ਸਤਰਾਂ = f.ਪਠ().splitlines()

parts = [f'static const char *{decl} =\n']
ਲਈ ਸਤਰ ਵਿੱਚ ਸਤਰਾਂ:
    ਸਤਰ = ਸਤਰ.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

ਨਾਲ open(OUTPUT, 'w') ਇਵ f:
    f.ਲਿਖ(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.ਰਸਤਾ.join(CURRENT_ਸਰੋਤ_DIR, hh)
ਕੋਸ਼ਿਸ਼:
    shutil.copyfile(OUTPUT, src_copy)
ਅਪਵਾਦ OSError:
    # Read as text to ignore line-ending changes
    ਨਾਲ open(OUTPUT, 'r') ਇਵ f1, open(src_copy, 'r') ਇਵ f2:
        ਜੇ f1.ਪਠ() != f2.ਪਠ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
