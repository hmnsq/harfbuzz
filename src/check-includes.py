#!/usr/bin/env python3

ಆನಯನ os
ಆನಯನ re
ಆನಯನ sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ಪ್ರತ್ಯಾಗಮ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) ಪ್ರತಿ x ಅನ್ತಃ os.getenv("HBHEADERS", "").split()] ವಾ [
    x ಪ್ರತಿ x ಅನ್ತಃ os.listdir(".") ಯದಿ x.startswith("hb") ಚ x.endswith(".h")
]

HBSOURCES = [removeprefix(x) ಪ್ರತಿ x ಅನ್ತಃ os.getenv("HBSOURCES", "").split()] ವಾ [
    x ಪ್ರತಿ x ಅನ್ತಃ os.listdir(".") ಯದಿ x.startswith("hb") ಚ x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
ಪ್ರತಿ x ಅನ್ತಃ HBHEADERS:
    ಯದಿ x == "hb.h" ವಾ x == "hb-common.h":
        ಅನುವರ್ತ
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ಇವ f:
        content = f.ಪಠ()
    first = re.findall(r"#.*include.*", content)[0]
    ಯದಿ first ನ ಅನ್ತಃ ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
ಪ್ರತಿ x ಅನ್ತಃ HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ಇವ f:
        content = f.ಪಠ()
    includes = re.findall(r"#.*include.*", content)
    ಯದಿ includes:
        ಯದಿ ನ len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
ಪ್ರತಿ x ಅನ್ತಃ HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ಇವ f:
        content = f.ಪಠ()
    ಯದಿ re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
