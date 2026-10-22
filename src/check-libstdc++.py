#!/usr/bin/env python3

आनयन os
आनयन shutil
आनयन subprocess
आनयन sys

तः check_helpers आनयन CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
यदि होइन ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    यदि otool:
        ldd = otool + " -L"
    अन्यथा:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = असत्य

लागि soname अन्तः CHECK_LIBSTDCXX_LIBS:
    लागि suffix अन्तः ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        यदि होइन os.path.exists(so):
            जारी

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        यदि (b"libstdc++" अन्तः ldd_result) वा (b"libc++" अन्तः ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = सत्य

यदि होइन tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
