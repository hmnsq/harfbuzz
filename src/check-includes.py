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
    ܦܢܐ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) لكل x في os.getenv("HBHEADERS", "").split()] ܐܘ [
    x لكل x في os.listdir(".") ܐܢ x.startswith("hb") ܘ x.endswith(".h")
]

HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").split()] ܐܘ [
    x لكل x في os.listdir(".") ܐܢ x.startswith("hb") ܘ x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
لكل x في HBHEADERS:
    ܐܢ x == "hb.h" ܐܘ x == "hb-common.h":
        استمر
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    first = re.findall(r"#.*include.*", content)[0]
    ܐܢ first ܠܐ في ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
لكل x في HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    includes = re.findall(r"#.*include.*", content)
    ܐܢ includes:
        ܐܢ ܠܐ len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
لكل x في HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    ܐܢ re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
