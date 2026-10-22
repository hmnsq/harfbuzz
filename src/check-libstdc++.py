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
གལ་སྲིད na ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    གལ་སྲིད otool:
        ldd = otool + " -L"
    དེ་མིན:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = False

དོན་ལུ soname anta_h CHECK_LIBSTDCXX_LIBS:
    དོན་ལུ suffix anta_h ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        གལ་སྲིད na os.path.exists(so):
            continue

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        གལ་སྲིད (b"libstdc++" anta_h ldd_result) or (b"libc++" anta_h ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = True

གལ་སྲིད na tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
