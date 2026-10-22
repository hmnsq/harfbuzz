#!/usr/bin/env python3

இறக்குமதி os
இறக்குமதி shutil
இறக்குமதி subprocess
இறக்குமதி sys

தஃ check_helpers இறக்குமதி CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
எனில் இல்லை ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    எனில் otool:
        ldd = otool + " -L"
    இல்லையெனில்:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = பொய்

க்கு soname உள் CHECK_LIBSTDCXX_LIBS:
    க்கு suffix உள் ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        எனில் இல்லை os.path.exists(so):
            தொடர்

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        எனில் (b"libstdc++" உள் ldd_result) அல்லது (b"libc++" உள் ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = உண்மை

எனில் இல்லை tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
