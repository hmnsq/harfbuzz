#!/usr/bin/env python3

ଆମଦାନୀ os
ଆମଦାନୀ shutil
ଆମଦାନୀ subprocess
ଆମଦାନୀ sys

ତଃ check_helpers ଆମଦାନୀ CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
ଯଦି ନୁହେଁ ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    ଯଦି otool:
        ldd = otool + " -L"
    ଅନ୍ୟଥା:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = ମିଥ୍ୟା

ପାଇଁ soname ଭିତରେ CHECK_LIBSTDCXX_LIBS:
    ପାଇଁ suffix ଭିତରେ ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        ଯଦି ନୁହେଁ os.path.exists(so):
            ଜାରି

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        ଯଦି (b"libstdc++" ଭିତରେ ldd_result) କିମ୍ବା (b"libc++" ଭିତରେ ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = ସତ୍ୟ

ଯଦି ନୁହେଁ tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
