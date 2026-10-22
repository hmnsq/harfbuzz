#!/usr/bin/env python3

आयात os
आयात shutil
आयात subprocess
आयात sys

तः check_helpers आयात CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
जर नाही ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    जर otool:
        ldd = otool + " -L"
    अन्यथा:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = असत्य

साठी soname मध्ये CHECK_LIBSTDCXX_LIBS:
    साठी suffix मध्ये ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        जर नाही os.path.exists(so):
            सुरूठेवा

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        जर (b"libstdc++" मध्ये ldd_result) किंवा (b"libc++" मध्ये ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = सत्य

जर नाही tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
