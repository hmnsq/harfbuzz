#!/usr/bin/env python3

আমদানি os
আমদানি shutil
আমদানি subprocess
আমদানি sys

তঃ check_helpers আমদানি CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
যদি নয় ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    যদি otool:
        ldd = otool + " -L"
    অন্যথা:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = মিথ্যা

জন্য soname মধ্যে CHECK_LIBSTDCXX_LIBS:
    জন্য suffix মধ্যে ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        যদি নয় os.path.exists(so):
            চালু

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_ফলাফল = subprocess.check_output(ldd.split() + [so])
        যদি (b"libstdc++" মধ্যে ldd_ফলাফল) অথবা (b"libc++" মধ্যে ldd_ফলাফল):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = সত্য

যদি নয় tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
