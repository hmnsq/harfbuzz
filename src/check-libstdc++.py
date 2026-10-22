#!/usr/bin/env python3

ਆਯਾਤ os
ਆਯਾਤ shutil
ਆਯਾਤ subprocess
ਆਯਾਤ sys

ਤਃ check_helpers ਆਯਾਤ CHECK_LIBSTDCXX_LIBS

srcdir = sys.argv[1]
ਅਧਾਰ_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

ldd = os.getenv("LDD", shutil.which("ldd"))
ਜੇ ਨਹੀਂ ldd:
    otool = os.getenv("OTOOL", shutil.which("otool"))
    ਜੇ otool:
        ldd = otool + " -L"
    ਨਹੀਂਤਾਂ:
        ਛਾਪੋ("check-libstdc++.py: 'ldd' not found; skipping test")
        sys.exit(77)

stat = 0
tested = ਝੂਠ

ਲਈ soname ਵਿੱਚ CHECK_LIBSTDCXX_LIBS:
    ਲਈ suffix ਵਿੱਚ ["so", "dylib"]:
        so = os.ਰਸਤਾ.join(builddir, "lib%s.%s" % (soname, suffix))
        ਜੇ ਨਹੀਂ os.ਰਸਤਾ.exists(so):
            ਜਾਰੀ

        ਛਾਪੋ("Checking that we are not linking to libstdc++ or libc++ in %s" % so)
        ldd_ਨਤੀਜਾ = subprocess.check_output(ldd.ਵੰਡੋ() + [so])
        ਜੇ (b"libstdc++" ਵਿੱਚ ldd_ਨਤੀਜਾ) ਜਾਂ (b"libc++" ਵਿੱਚ ldd_ਨਤੀਜਾ):
            ਛਾਪੋ("Ouch, %s is linked to libstdc++ or libc++" % so)
            stat = 1

        tested = ਸੱਚ

ਜੇ ਨਹੀਂ tested:
    ਛਾਪੋ("check-libstdc++.py: libharfbuzz shared library not found; skipping test")
    sys.exit(77)

sys.exit(stat)
