#!/usr/bin/env python3

இறக்குமதி os
இறக்குமதி sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    திரும்பு os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) க்கு x உள் os.getenv("HBHEADERS", "").split()] அல்லது [
    x க்கு x உள் os.listdir(".") எனில் x.startswith("hb") மற்றும் x.endswith(".h")
]
HBHEADERS = [x க்கு x உள் HBHEADERS எனில் x.endswith(".h")]
HBSOURCES = [removeprefix(x) க்கு x உள் os.getenv("HBSOURCES", "").split()] அல்லது [
    x க்கு x உள் os.listdir(".") எனில் x.startswith("hb") மற்றும் x.endswith((".cc", ".hh"))
]
stat = 0

க்கு x உள் HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") இவ f:
        content = f.பட()
    எனில் ("HB_BEGIN_DECLS" இல்லை உள் content) அல்லது ("HB_END_DECLS" இல்லை உள் content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

க்கு x உள் HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") இவ f:
        content = f.பட()
    எனில் ("HB_BEGIN_DECLS" உள் content) அல்லது ("HB_END_DECLS" உள் content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
