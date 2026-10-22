#!/usr/bin/env python3

درآمد os
درآمد shutil
درآمد subprocess
درآمد sys

سے check_helpers درآمد CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
بنیاد_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
اگر نہیں ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    اگر otool:
        ldd = otool + " -L"
    ورنہ:
        چھاپو("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = جھوٹ

برائے soname میں CHECK_LIBSTDCXX_LIBS:
    برائے suffix میں ["so", "dylib"]:
        so = os.راستہ.join(builddir, "lib%s.%s" % (soname, suffix))
        اگر نہیں os.راستہ.exists(so):
            جاری

        چھاپو("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_نتیجہ = subprocess.check_output(ldd.بانٹو() + [so])
        اگر (b"libstdc++" میں ldd_نتیجہ) یا (b"libc++" میں ldd_نتیجہ):
            چھاپو("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = سچ

اگر نہیں tested:
    چھاپو("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
