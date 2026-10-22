#!/usr/bin/env python3

استورد os
استورد shutil
استورد subprocess
استورد sys

من check_helpers استورد CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
ادھار_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
إذا ليس ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    إذا otool:
        ldd = otool + " -L"
    وإلا:
        چھاپو("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = خطأ

لكل soname في CHECK_LIBSTDCXX_LIBS:
    لكل suffix في ["so", "dylib"]:
        so = os.رستہ.join(builddir, "lib%s.%s" % (soname, suffix))
        إذا ليس os.رستہ.exists(so):
            استمر

        چھاپو("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_نتیجہ = subprocess.check_output(ldd.ونڈو() + [so])
        إذا (b"libstdc++" في ldd_نتیجہ) أو (b"libc++" في ldd_نتیجہ):
            چھاپو("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = سچ

إذا ليس tested:
    چھاپو("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
