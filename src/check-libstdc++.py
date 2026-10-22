#!/usr/bin/env python3

استورد os
استورد shutil
استورد subprocess
استورد sys

من check_helpers استورد CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
بنياد_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
إذا ليس ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    إذا otool:
        ldd = otool + " -L"
    وإلا:
        ڇاپيو("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = خطأ

لكل soname في CHECK_LIBSTDCXX_LIBS:
    لكل suffix في ["so", "dylib"]:
        so = os.رستو.join(builddir, "lib%s.%s" % (soname, suffix))
        إذا ليس os.رستو.exists(so):
            استمر

        ڇاپيو("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_نتيجو = subprocess.check_output(ldd.ورهايو() + [so])
        إذا (b"libstdc++" في ldd_نتيجو) أو (b"libc++" في ldd_نتيجو):
            ڇاپيو("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = سچ

إذا ليس tested:
    ڇاپيو("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
