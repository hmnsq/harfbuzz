#!/usr/bin/env python3

import os
import shutil
import subprocess
import sys

from check_helpers import CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
якщо not ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    якщо otool:
        ldd = otool + " -L"
    інакше:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = False

для soname in CHECK_LIBSTDCXX_LIBS:
    для suffix in ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        якщо not os.path.exists(so):
            continue

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        якщо (b"libstdc++" in ldd_result) or (b"libc++" in ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = True

якщо not tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
