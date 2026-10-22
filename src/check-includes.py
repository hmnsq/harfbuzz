#!/usr/bin/env python3

واردکن os
واردکن re
واردکن sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    بازگشت os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) برای x در os.getenv("HBHEADERS", "").split()] یا [
    x برای x در os.listdir(".") اگر x.startswith("hb") و x.endswith(".h")
]

HBSOURCES = [removeprefix(x) برای x در os.getenv("HBSOURCES", "").split()] یا [
    x برای x در os.listdir(".") اگر x.startswith("hb") و x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
برای x در HBHEADERS:
    اگر x == "hb.h" یا x == "hb-common.h":
        ادامه
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") بهعنوان f:
        content = f.اقرأ()
    first = re.findall(r"#.*include.*", content)[0]
    اگر first نه در ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
برای x در HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") بهعنوان f:
        content = f.اقرأ()
    includes = re.findall(r"#.*include.*", content)
    اگر includes:
        اگر نه len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
برای x در HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") بهعنوان f:
        content = f.اقرأ()
    اگر re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
