#!/usr/bin/env python3

import os
import re
import sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    повернути os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) для x in os.getenv("HBHEADERS", "").split()] or [
    x для x in os.listdir(".") якщо x.startswith("hb") and x.endswith(".h")
]

HBSOURCES = [removeprefix(x) для x in os.getenv("HBSOURCES", "").split()] or [
    x для x in os.listdir(".") якщо x.startswith("hb") and x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
для x in HBHEADERS:
    якщо x == "hb.h" or x == "hb-common.h":
        continue
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    first = re.findall(r"#.*include.*", content)[0]
    якщо first not in ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
для x in HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    includes = re.findall(r"#.*include.*", content)
    якщо includes:
        якщо not len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
для x in HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    якщо re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
