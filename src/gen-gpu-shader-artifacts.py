#!/usr/bin/env python3

"This tool is intended to be used from meson"

దిగుమతి os, sys, shutil

ఒకవేళ పొడవు(sys.argv) < 4:
    sys.exit(__doc__)

OUTPUT = sys.argv[1]
CURRENT_ఆధారం_DIR = sys.argv[2]
ఆదానం_2 = sys.argv[3]

hh = os.మార్గం.basename(OUTPUT)
# Decl name derives from the .hh filename: hyphens -> underscores,
# strip the .hh suffix.  Example: hb-gpu-fragment-glsl.hh -> hb_gpu_fragment_glsl.
ఒకవేళ కాదు hh.endswith('.hh'):
    sys.exit(f'Output must end with .hh: {hh}')
decl = hh[:-పొడవు('.hh')].replace('-', '_')

# Stringize the GLSL source into a C string literal
తోడుగా open(ఆదానం_2, 'r') ఇవ f:
    పంక్తులు = f.పఠ().splitlines()

parts = [f'static const char *{decl} =\n']
కొరకు పంక్తి లో పంక్తులు:
    పంక్తి = పంక్తి.replace('\\', '\\\\').replace('"', '\\"')
    parts.append(f'"{line}\\n"\n')
parts.append(';\n')

content = ''.join(parts)

తోడుగా open(OUTPUT, 'w') ఇవ f:
    f.లిఖ(content)

# Copy back to source tree; if read-only, verify committed copy matches.
src_copy = os.మార్గం.join(CURRENT_ఆధారం_DIR, hh)
ప్రయత్నించు:
    shutil.copyfile(OUTPUT, src_copy)
మినహాయింపు OSError:
    # Read as text to ignore line-ending changes
    తోడుగా open(OUTPUT, 'r') ఇవ f1, open(src_copy, 'r') ఇవ f2:
        ఒకవేళ f1.పఠ() != f2.పఠ():
            sys.exit(f'{src_copy} is out of date; regenerate with a writable source tree')
