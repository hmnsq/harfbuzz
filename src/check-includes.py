#!/usr/bin/env python3

আমদানি os
আমদানি re
আমদানি sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ফেরত os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) জন্য x মধ্যে os.getenv("HBHEADERS", "").split()] অথবা [
    x জন্য x মধ্যে os.listdir(".") যদি x.startswith("hb") এবং x.endswith(".h")
]

HBSOURCES = [removeprefix(x) জন্য x মধ্যে os.getenv("HBSOURCES", "").split()] অথবা [
    x জন্য x মধ্যে os.listdir(".") যদি x.startswith("hb") এবং x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
জন্য x মধ্যে HBHEADERS:
    যদি x == "hb.h" অথবা x == "hb-common.h":
        চালু
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইব f:
        content = f.পঠ()
    first = re.findall(r"#.*include.*", content)[0]
    যদি first নয় মধ্যে ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
জন্য x মধ্যে HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইব f:
        content = f.পঠ()
    includes = re.findall(r"#.*include.*", content)
    যদি includes:
        যদি নয় len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
জন্য x মধ্যে HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইব f:
        content = f.পঠ()
    যদি re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
