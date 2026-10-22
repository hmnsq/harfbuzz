#!/usr/bin/env python3

ഇറക്കുമതി os
ഇറക്കുമതി re
ഇറക്കുമതി sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    തിരികെ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) വേണ്ടി x ഇൽ os.getenv("HBHEADERS", "").split()] അഥവാ [
    x വേണ്ടി x ഇൽ os.listdir(".") എങ്കിൽ x.startswith("hb") ഒപ്പം x.endswith(".h")
]

HBSOURCES = [removeprefix(x) വേണ്ടി x ഇൽ os.getenv("HBSOURCES", "").split()] അഥവാ [
    x വേണ്ടി x ഇൽ os.listdir(".") എങ്കിൽ x.startswith("hb") ഒപ്പം x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
വേണ്ടി x ഇൽ HBHEADERS:
    എങ്കിൽ x == "hb.h" അഥവാ x == "hb-common.h":
        തുടരുക
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ഇവ f:
        content = f.പഠ()
    first = re.findall(r"#.*include.*", content)[0]
    എങ്കിൽ first അല്ല ഇൽ ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
വേണ്ടി x ഇൽ HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ഇവ f:
        content = f.പഠ()
    includes = re.findall(r"#.*include.*", content)
    എങ്കിൽ includes:
        എങ്കിൽ അല്ല len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
വേണ്ടി x ഇൽ HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ഇവ f:
        content = f.പഠ()
    എങ്കിൽ re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
