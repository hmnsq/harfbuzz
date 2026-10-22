#!/usr/bin/env python3

आनयन os
आनयन re
आनयन sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    फर्काऊ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) लागि x अन्तः os.getenv("HBHEADERS", "").split()] वा [
    x लागि x अन्तः os.listdir(".") यदि x.startswith("hb") र x.endswith(".h")
]

HBSOURCES = [removeprefix(x) लागि x अन्तः os.getenv("HBSOURCES", "").split()] वा [
    x लागि x अन्तः os.listdir(".") यदि x.startswith("hb") र x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
लागि x अन्तः HBHEADERS:
    यदि x == "hb.h" वा x == "hb-common.h":
        जारी
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    first = re.findall(r"#.*include.*", content)[0]
    यदि first होइन अन्तः ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
लागि x अन्तः HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    includes = re.findall(r"#.*include.*", content)
    यदि includes:
        यदि होइन len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
लागि x अन्तः HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    यदि re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
