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
सचे न ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    सचे otool:
        ldd = otool + " -L"
    अञ्ञथा:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = मिच्छा

अत्थाय soname अन्तः CHECK_LIBSTDCXX_LIBS:
    अत्थाय suffix अन्तः ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        सचे न os.path.exists(so):
            अनुवर्त

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        सचे (b"libstdc++" अन्तः ldd_result) वा (b"libc++" अन्तः ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = सच्च

सचे न tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
