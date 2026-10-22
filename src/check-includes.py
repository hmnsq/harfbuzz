#!/usr/bin/env python3

இறக்குமதி os
இறக்குமதி re
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

HBSOURCES = [removeprefix(x) க்கு x உள் os.getenv("HBSOURCES", "").split()] அல்லது [
    x க்கு x உள் os.listdir(".") எனில் x.startswith("hb") மற்றும் x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
க்கு x உள் HBHEADERS:
    எனில் x == "hb.h" அல்லது x == "hb-common.h":
        தொடர்
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") இவ f:
        content = f.பட()
    first = re.findall(r"#.*include.*", content)[0]
    எனில் first இல்லை உள் ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
க்கு x உள் HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") இவ f:
        content = f.பட()
    includes = re.findall(r"#.*include.*", content)
    எனில் includes:
        எனில் இல்லை len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
க்கு x உள் HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") இவ f:
        content = f.பட()
    எனில் re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
