#!/usr/bin/env python3

આયાત os
આયાત shutil
આયાત subprocess
આયાત sys

તઃ check_helpers આયાત CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
જો નહીં ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    જો otool:
        ldd = otool + " -L"
    નહીંતર:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = ખોટું

માટે soname માં CHECK_LIBSTDCXX_LIBS:
    માટે suffix માં ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        જો નહીં os.path.exists(so):
            ચાલુ

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        જો (b"libstdc++" માં ldd_result) અથવા (b"libc++" માં ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = સાચું

જો નહીં tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
