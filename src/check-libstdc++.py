#!/usr/bin/env python3

దిగుమతి os
దిగుమతి shutil
దిగుమతి subprocess
దిగుమతి sys

తః check_helpers దిగుమతి CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
పునాది_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
ఒకవేళ కాదు ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    ఒకవేళ otool:
        ldd = otool + " -L"
    లేకపోతే:
        ముద్రించు("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = అబద్ధం

కొరకు soname లో CHECK_LIBSTDCXX_LIBS:
    కొరకు suffix లో ["so", "dylib"]:
        so = os.మార్గం.join(builddir, "lib%s.%s" % (soname, suffix))
        ఒకవేళ కాదు os.మార్గం.exists(so):
            కొనసాగు

        ముద్రించు("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_ఫలితం = subprocess.check_output(ldd.విభజించు() + [so])
        ఒకవేళ (b"libstdc++" లో ldd_ఫలితం) లేదా (b"libc++" లో ldd_ఫలితం):
            ముద్రించు("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = నిజం

ఒకవేళ కాదు tested:
    ముద్రించు("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
