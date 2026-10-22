#!/usr/bin/env python3

import os
import shutil
import subprocess
import sys

ta_h check_helpers import CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
ᮚᮦᮔ᮪ na ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    ᮚᮦᮔ᮪ otool:
        ldd = otool + " -L"
    ᮜᮤᮚ:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = False

for soname anta_h CHECK_LIBSTDCXX_LIBS:
    for suffix anta_h ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        ᮚᮦᮔ᮪ na os.path.exists(so):
            continue

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        ᮚᮦᮔ᮪ (b"libstdc++" anta_h ldd_result) or (b"libc++" anta_h ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = True

ᮚᮦᮔ᮪ na tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
