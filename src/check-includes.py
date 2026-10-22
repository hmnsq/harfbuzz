#!/usr/bin/env python3

استورد os
استورد re
استورد sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    החזר os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) עבור x في os.getenv("HBHEADERS", "").split()] או [
    x עבור x في os.listdir(".") אם x.startswith("hb") וגם x.endswith(".h")
]

HBSOURCES = [removeprefix(x) עבור x في os.getenv("HBSOURCES", "").split()] או [
    x עבור x في os.listdir(".") אם x.startswith("hb") וגם x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
עבור x في HBHEADERS:
    אם x == "hb.h" או x == "hb-common.h":
        استمر
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    first = re.findall(r"#.*include.*", content)[0]
    אם first לא في ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
עבור x في HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    includes = re.findall(r"#.*include.*", content)
    אם includes:
        אם לא len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
עבור x في HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    אם re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
