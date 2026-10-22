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
    ꦮꦭꦶ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) ꦏꦁꦒꦺꦴ x anta_h os.getenv("HBHEADERS", "").split()] or [
    x ꦏꦁꦒꦺꦴ x anta_h os.listdir(".") ꦪꦼꦤ꧀ x.startswith("hb") ca x.endswith(".h")
]

HBSOURCES = [removeprefix(x) ꦏꦁꦒꦺꦴ x anta_h os.getenv("HBSOURCES", "").split()] or [
    x ꦏꦁꦒꦺꦴ x anta_h os.listdir(".") ꦪꦼꦤ꧀ x.startswith("hb") ca x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
ꦏꦁꦒꦺꦴ x anta_h HBHEADERS:
    ꦪꦼꦤ꧀ x == "hb.h" or x == "hb-common.h":
        continue
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    first = re.findall(r"#.*include.*", content)[0]
    ꦪꦼꦤ꧀ first na anta_h ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
ꦏꦁꦒꦺꦴ x anta_h HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    includes = re.findall(r"#.*include.*", content)
    ꦪꦼꦤ꧀ includes:
        ꦪꦼꦤ꧀ na len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
ꦏꦁꦒꦺꦴ x anta_h HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    ꦪꦼꦤ꧀ re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
