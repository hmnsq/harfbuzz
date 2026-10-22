#!/usr/bin/env python3

আনযন os
আনযন shutil
আনযন subprocess
আনযন sys

তঃ check_helpers আনযন CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
যদি ন ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    যদি otool:
        ldd = otool + " -L"
    অন্যথা:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = অসত্য

প্রতি soname অন্তঃ CHECK_LIBSTDCXX_LIBS:
    প্রতি suffix অন্তঃ ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        যদি ন os.path.exists(so):
            অনুবর্ত

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        যদি (b"libstdc++" অন্তঃ ldd_result) বা (b"libc++" অন্তঃ ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = সত্য

যদি ন tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
