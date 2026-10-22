#!/usr/bin/env python3

"This tool is intended to be used from meson"

আনযন os, sys, shutil

যদি len(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]
প্ৰবেশ_2 = sys.argv[3]

hh = os.path.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
যদি নহয় hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-len('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
with open(প্ৰবেশ_2, 'r') ইৱ f:
    সাৰিগুলি = f.পঠ().splitlines()

parts = [f'static const char *{decl} =\n']
বাবে সাৰি অন্তঃ সাৰিগুলি:
    সাৰি = সাৰি.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

with open(OUTPUT, 'w') ইৱ f:
    f.লিখ(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.path.join(CURRENT_SOURCE_DIR, hh)
যত্ন:
    shutil.copyfile(OUTPUT, src_copy)
অপৱাদ OSError:
    # Read as text to ignore line-ending changes
    with open(OUTPUT, 'r') ইৱ f1, open(src_copy, 'r') ইৱ f2:
        যদি f1.পঠ() != f2.পঠ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
