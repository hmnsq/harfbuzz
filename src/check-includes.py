#!/usr/bin/env python3

ଆମଦାନୀ os
ଆମଦାନୀ re
ଆମଦାନୀ sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ଫେରନ୍ତୁ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) ପାଇଁ x ଭିତରେ os.getenv("HBHEADERS", "").split()] କିମ୍ବା [
    x ପାଇଁ x ଭିତରେ os.listdir(".") ଯଦି x.startswith("hb") ଏବଂ x.endswith(".h")
]

HBSOURCES = [removeprefix(x) ପାଇଁ x ଭିତରେ os.getenv("HBSOURCES", "").split()] କିମ୍ବା [
    x ପାଇଁ x ଭିତରେ os.listdir(".") ଯଦି x.startswith("hb") ଏବଂ x.endswith((".cc", ".hh"))
]


stat = 0

print(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
ପାଇଁ x ଭିତରେ HBHEADERS:
    ଯଦି x == "hb.h" କିମ୍ବା x == "hb-common.h":
        ଜାରି
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ଇଵ f:
        content = f.ପଠ()
    first = re.findall(r"#.*include.*", content)[0]
    ଯଦି first ନୁହେଁ ଭିତରେ ['#include "hb.h"', '#include "hb-common.h"']:
        print("failure on %s" % x)
        stat = 1

print("Checking that source files #include a private header first (or none)")
ପାଇଁ x ଭିତରେ HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ଇଵ f:
        content = f.ପଠ()
    includes = re.findall(r"#.*include.*", content)
    ଯଦି includes:
        ଯଦି ନୁହେଁ len(re.findall(r'".*\.hh"', includes[0])):
            print("failure on %s" % x)
            stat = 1

print("Checking that there is no #include <hb-*.h>")
ପାଇଁ x ଭିତରେ HBHEADERS + HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ଇଵ f:
        content = f.ପଠ()
    ଯଦି re.findall("#.*include.*<.*hb", content):
        print("failure on %s" % x)
        stat = 1

sys.exit(stat)
