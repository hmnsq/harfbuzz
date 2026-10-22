#!/usr/bin/env python3

استورد os
استورد shutil
استورد subprocess
استورد sys

من check_helpers استورد CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
𐡄𐡍 𐡋𐡀 ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    𐡄𐡍 otool:
        ldd = otool + " -L"
    وإلا:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = خطأ

لكل soname في CHECK_LIBSTDCXX_LIBS:
    لكل suffix في ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        𐡄𐡍 𐡋𐡀 os.path.exists(so):
            استمر

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        𐡄𐡍 (b"libstdc++" في ldd_result) 𐡀𐡅 (b"libc++" في ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = 𐡒𐡔𐡈

𐡄𐡍 𐡋𐡀 tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
