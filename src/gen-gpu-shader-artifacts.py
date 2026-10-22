#!/usr/bin/env python3

"This tool is intended to be used from meson"

আনযন os, sys, shutil

যদি len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
প্রবেশ_2 = sys.argv[3]

hh = os.path.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
যদি ন hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-len('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
with open(প্রবেশ_2, 'r') ইব f:
    সারিগুলি = f.পঠ().splitlines()

parts = [f'static const char *{decl} =\n']
প্রতি সারি অন্তঃ সারিগুলি:
    সারি = সারি.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

with open(OUTPUT, 'w') ইব f:
    f.লিখ(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.path.join(CURRENT_SOURCE_DIR, hh)
যত্ন:
    shutil.copyfile(OUTPUT, src_copy)
অপবাদ OSError:
    # Read as text to ignore line-ending changes
    with open(OUTPUT, 'r') ইব f1, open(src_copy, 'r') ইব f2:
        যদি f1.পঠ() != f2.পঠ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
