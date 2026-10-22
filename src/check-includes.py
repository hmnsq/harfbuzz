#!/usr/bin/env python3

आयात os
आयात re
आयात sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    परत os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) साठी x मध्ये os.getenv("HBHEADERS", "").split()] किंवा [
    x साठी x मध्ये os.listdir(".") जर x.startswith("hb") आणि x.endswith(".h")
]

HBSOURCES = [removeprefix(x) साठी x मध्ये os.getenv("HBSOURCES", "").split()] किंवा [
    x साठी x मध्ये os.listdir(".") जर x.startswith("hb") आणि x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
साठी x मध्ये HBHEADERS:
    जर x == "hb.h" किंवा x == "hb-common.h":
        सुरूठेवा
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    first = re.findall(r"#.*include.*", content)[0]
    जर first नाही मध्ये ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
साठी x मध्ये HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    includes = re.findall(r"#.*include.*", content)
    जर includes:
        जर नाही len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
साठी x मध्ये HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    जर re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
