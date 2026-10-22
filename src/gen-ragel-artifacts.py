#!/usr/bin/env python3

"This tool is intended to be used from meson"

ਆਯਾਤ os, os.ਰਸਤਾ, sys, subprocess, shutil

ragel = sys.argv[1]
ਜੇ ਨਹੀਂ ragel:
	sys.exit ('You have to install ragel if you are going to develop HarfBuzz itself')

ਜੇ ਲੰਬਾਈ (sys.argv) < 4:
	sys.exit (__doc__)

OUTPUT = sys.argv[2]
CURRENT_ਸਰੋਤ_DIR = sys.argv[3]
ਨਿਵੇਸ਼_2 = sys.argv[4]

outdir = os.ਰਸਤਾ.dirname (OUTPUT)
shutil.copy (ਨਿਵੇਸ਼_2, outdir)
rl = os.ਰਸਤਾ.basename (ਨਿਵੇਸ਼_2)
hh = rl.replace ('.rl', '.hh')
ret = subprocess.Popen (ragel.ਵੰਡੋ() + ['-e', '-F1', '-o', hh, rl], cwd=outdir).wait ()
ਜੇ ret:
    sys.exit (ret)

# copy it also to src/; if read-only, verify committed copy matches.
generated = os.ਰਸਤਾ.join (outdir, hh)
src_copy = os.ਰਸਤਾ.join (CURRENT_ਸਰੋਤ_DIR, hh)
ਕੋਸ਼ਿਸ਼:
    shutil.copyfile (generated, src_copy)
ਅਪਵਾਦ OSError:
    ਆਯਾਤ filecmp
    ਜੇ ਨਹੀਂ filecmp.cmp (generated, src_copy, shallow=ਝੂਠ):
        sys.exit (f'{src_copy} is out of date; regenerate with a writable source tree')
