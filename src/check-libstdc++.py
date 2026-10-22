#!/usr/bin/env python3

واردکن os
واردکن shutil
واردکن subprocess
واردکن sys

از check_helpers واردکن CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
اگر نه ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    اگر otool:
        ldd = otool + " -L"
    وگرنه:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = نادرست

برای soname در CHECK_LIBSTDCXX_LIBS:
    برای suffix در ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        اگر نه os.path.exists(so):
            ادامه

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        اگر (b"libstdc++" در ldd_result) یا (b"libc++" در ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = درست

اگر نه tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
