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
    ආපසු os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) සඳහා x anta_h os.getenv("HBHEADERS", "").split()] or [
    x සඳහා x anta_h os.listdir(".") නම් x.startswith("hb") ca x.endswith(".h")
]

HBSOURCES = [removeprefix(x) සඳහා x anta_h os.getenv("HBSOURCES", "").split()] or [
    x සඳහා x anta_h os.listdir(".") නම් x.startswith("hb") ca x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
සඳහා x anta_h HBHEADERS:
    නම් x == "hb.h" or x == "hb-common.h":
        continue
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    first = re.findall(r"#.*include.*", content)[0]
    නම් first na anta_h ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
සඳහා x anta_h HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    includes = re.findall(r"#.*include.*", content)
    නම් includes:
        නම් na len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
සඳහා x anta_h HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    නම් re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
