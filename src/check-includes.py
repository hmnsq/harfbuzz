#!/usr/bin/env python3

आयात os
आयात re
आयात sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.मेल(base_srcdir, s)
    वापस os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) क्रम x in os.getenv("HBHEADERS", "").split()] or [
    x क्रम x in os.listdir(".") अगर x.startswith("hb") and x.endswith(".h")
]

HBSOURCES = [removeprefix(x) क्रम x in os.getenv("HBSOURCES", "").split()] or [
    x क्रम x in os.listdir(".") अगर x.startswith("hb") and x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
क्रम x in HBHEADERS:
    अगर x == "hb.h" or x == "hb-common.h":
        जारी
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.म_पढ़ो()
    first = re.findall(r"#.*include.*", content)[0]
    अगर first not in ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
क्रम x in HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.म_पढ़ो()
    includes = re.findall(r"#.*include.*", content)
    अगर includes:
        अगर not len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
क्रम x in HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.म_पढ़ो()
    अगर re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
