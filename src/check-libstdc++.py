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
ܐܢ ܠܐ ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    ܐܢ otool:
        ldd = otool + " -L"
    ܐܚܪܢܐ:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = ܕܓܠܐ

لكل soname في CHECK_LIBSTDCXX_LIBS:
    لكل suffix في ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        ܐܢ ܠܐ os.path.exists(so):
            استمر

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        ܐܢ (b"libstdc++" في ldd_result) ܐܘ (b"libc++" في ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = ܫܪܪܐ

ܐܢ ܠܐ tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
