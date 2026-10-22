#!/usr/bin/env python3

ഇറക്കുമതി os
ഇറക്കുമതി shutil
ഇറക്കുമതി subprocess
ഇറക്കുമതി sys

തഃ check_helpers ഇറക്കുമതി CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
എങ്കിൽ അല്ല ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    എങ്കിൽ otool:
        ldd = otool + " -L"
    അല്ലെങ്കിൽ:
        print("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = അസത്യം

വേണ്ടി soname ഇൽ CHECK_LIBSTDCXX_LIBS:
    വേണ്ടി suffix ഇൽ ["so", "dylib"]:
        so = os.path.join(builddir, "lib%s.%s" % (soname, suffix))
        എങ്കിൽ അല്ല os.path.exists(so):
            തുടരുക

        print("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_result = subprocess.check_output(ldd.split() + [so])
        എങ്കിൽ (b"libstdc++" ഇൽ ldd_result) അഥവാ (b"libc++" ഇൽ ldd_result):
            print("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = സത്യം

എങ്കിൽ അല്ല tested:
    print("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
