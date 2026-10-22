#!/usr/bin/env python3

استورد os
استورد re
استورد sys

srcdir = sys.argv[1]
بنسټ_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


تعریف removeprefix(s):
    abs_لار = os.لار.join(بنسټ_srcdir, s)
    أرجع os.لار.relpath(abs_لار, srcdir)


HBHEADERS = [os.لار.basename(x) لكل x في os.getenv("HBHEADERS", "").وېشل()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith(".h")
]

HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").وېشل()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith((".cc", ".hh"))
]


stat = 0

چاپ(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
لكل x في HBHEADERS:
    إذا x == "hb.h" أو x == "hb-common.h":
        استمر
    چاپ(f"Checking {x}")
    سره open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    لومړی = re.findall(r"#.*include.*", content)[0]
    إذا لومړی ليس في ['#include "hb.h"', '#include "hb-common.h"']:
        چاپ("failure on %s" % x)
        stat = 1

چاپ("Checking that source files #include a private header first (or none)")
لكل x في HBSOURCES:
    چاپ(f"Checking {x}")
    سره open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    includes = re.findall(r"#.*include.*", content)
    إذا includes:
        إذا ليس اوږدوالی(re.findall(r'".*\.hh"', includes[0])):
            چاپ("failure on %s" % x)
            stat = 1

چاپ("Checking that there is no #include <hb-*.h>")
لكل x في HBHEADERS + HBSOURCES:
    چاپ(f"Checking {x}")
    سره open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا re.findall("#.*include.*<.*hb", content):
        چاپ("failure on %s" % x)
        stat = 1

sys.exit(stat)
