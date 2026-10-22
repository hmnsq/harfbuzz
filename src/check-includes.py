#!/usr/bin/env python3

আনযন os
আনযন re
আনযন sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ঘূৰাই os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) বাবে x অন্তঃ os.getenv("HBHEADERS", "").split()] বা [
    x বাবে x অন্তঃ os.listdir(".") যদি x.startswith("hb") আৰু x.endswith(".h")
]

HBSOURCES = [removeprefix(x) বাবে x অন্তঃ os.getenv("HBSOURCES", "").split()] বা [
    x বাবে x অন্তঃ os.listdir(".") যদি x.startswith("hb") আৰু x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
বাবে x অন্তঃ HBHEADERS:
    যদি x == "hb.h" বা x == "hb-common.h":
        অনুৱর্ত
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইৱ f:
        content = f.পঠ()
    first = re.findall(r"#.*include.*", content)[0]
    যদি first নহয় অন্তঃ ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
বাবে x অন্তঃ HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইৱ f:
        content = f.পঠ()
    includes = re.findall(r"#.*include.*", content)
    যদি includes:
        যদি নহয় len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
বাবে x অন্তঃ HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইৱ f:
        content = f.পঠ()
    যদি re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
