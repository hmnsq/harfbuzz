#!/usr/bin/env python3

"This tool is intended to be used from meson"

واردکن os, os.path, sys, subprocess, shutil

ragel = sys.argv[1]
اگر نه ragel:
	sys.exit ('You have to install ragel if you are going to develop HarfBuzz itself')

اگر len (sys.argv) < 4:
	sys.exit (__doc__)

OUTPUT = sys.argv[2]
CURRENT_SOURCE_DIR = sys.argv[3]
درون_داد_2 = sys.argv[4]

outdir = os.path.dirname (OUTPUT)
shutil.copy (درون_داد_2, outdir)
rl = os.path.basename (درون_داد_2)
hh = rl.replace ('.rl', '.hh')
ret = subprocess.Popen (ragel.split() + ['-e', '-F1', '-o', hh, rl], cwd=outdir).wait ()
اگر ret:
    sys.exit (ret)

# copy it also to src/; if read-only, verify committed copy matches.
generated = os.path.join (outdir, hh)
src_copy = os.path.join (CURRENT_SOURCE_DIR, hh)
تلاش:
    shutil.copyfile (generated, src_copy)
استثنا OSError:
    واردکن filecmp
    اگر نه filecmp.cmp (generated, src_copy, shallow=نادرست):
        sys.exit (f'{src_copy} is out of date; regenerate with a writable source tree')
