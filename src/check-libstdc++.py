#!/usr/bin/env python3

ಆನಯನ os
ಆನಯನ shutil
ಆನಯನ subprocess
ಆನಯನ sys

ತಃ check_helpers ಆನಯನ CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
ಯದಿ ನ ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    ಯದಿ otool:
        ldd = otool + " -L"
    ಅನ್ಯಥಾ:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = ಅಸತ್ಯ

ಪ್ರತಿ soname ಅನ್ತಃ CHECK_LIBSTDCXX_LIBS:
    ಪ್ರತಿ suffix ಅನ್ತಃ ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        ಯದಿ ನ os.path.exists(so):
            ಅನುವರ್ತ

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        ಯದಿ (b"libstdc++" ಅನ್ತಃ ldd_result) ವಾ (b"libc++" ಅನ್ತಃ ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = ಸತ್ಯ

ಯದಿ ನ tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
