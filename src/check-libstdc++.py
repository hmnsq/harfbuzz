#!/usr/bin/env python3

आयात os
आयात shutil
आयात subprocess
आयात sys

from check_helpers आयात CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
अगर not ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    अगर otool:
        ldd = otool + " -L"
    अन्यथा:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = False

क्रम soname in CHECK_LIBSTDCXX_LIBS:
    क्रम suffix in ["so", "dylib"]:
        so = os.path.मेल(builddir, "lib%s.%s" % (soname, suffix))
        अगर not os.path.exists(so):
            जारी

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_परिणाम = subprocess.check_output(ldd.split() + [so])
        अगर (b"libstdc++" in ldd_परिणाम) or (b"libc++" in ldd_परिणाम):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = True

अगर not tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
