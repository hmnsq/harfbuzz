#!/usr/bin/env python3

આયાત os
આયાત re
આયાત sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    પાછા os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) માટે x માં os.getenv("HBHEADERS", "").split()] અથવા [
    x માટે x માં os.listdir(".") જો x.startswith("hb") અને x.endswith(".h")
]

HBSOURCES = [removeprefix(x) માટે x માં os.getenv("HBSOURCES", "").split()] અથવા [
    x માટે x માં os.listdir(".") જો x.startswith("hb") અને x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
માટે x માં HBHEADERS:
    જો x == "hb.h" અથવા x == "hb-common.h":
        ચાલુ
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ઇવ f:
        content = f.પઠ()
    first = re.findall(r"#.*include.*", content)[0]
    જો first નહીં માં ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
માટે x માં HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ઇવ f:
        content = f.પઠ()
    includes = re.findall(r"#.*include.*", content)
    જો includes:
        જો નહીં len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
માટે x માં HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ઇવ f:
        content = f.પઠ()
    જો re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
