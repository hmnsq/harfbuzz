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
    प्रत्यागम os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) प्रति x अन्तः os.getenv("HBHEADERS", "").split()] वा [
    x प्रति x अन्तः os.listdir(".") यदि x.startswith("hb") च x.endswith(".h")
]

HBSOURCES = [removeprefix(x) प्रति x अन्तः os.getenv("HBSOURCES", "").split()] वा [
    x प्रति x अन्तः os.listdir(".") यदि x.startswith("hb") च x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
प्रति x अन्तः HBHEADERS:
    यदि x == "hb.h" वा x == "hb-common.h":
        अनुवर्त
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    first = re.findall(r"#.*include.*", content)[0]
    यदि first न अन्तः ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
प्रति x अन्तः HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    includes = re.findall(r"#.*include.*", content)
    यदि includes:
        यदि न len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
प्रति x अन्तः HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    यदि re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
