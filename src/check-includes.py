#!/usr/bin/env python3

استورد os
استورد re
استورد sys

srcdir = sys.argv[1]
ادھار_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


تعریف removeprefix(s):
    abs_رستہ = os.رستہ.join(ادھار_srcdir, s)
    أرجع os.رستہ.relpath(abs_رستہ, srcdir)


HBHEADERS = [os.رستہ.basename(x) لكل x في os.getenv("HBHEADERS", "").ونڈو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith(".h")
]

HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").ونڈو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith((".cc", ".hh"))
]


stat = 0

چھاپو(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
لكل x في HBHEADERS:
    إذا x == "hb.h" أو x == "hb-common.h":
        استمر
    چھاپو(f"Checking {x}")
    نال open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    پہلا = re.findall(r"#.*include.*", content)[0]
    إذا پہلا ليس في ['#include "hb.h"', '#include "hb-common.h"']:
        چھاپو("failure on %s" % x)
        stat = 1

چھاپو("Checking that source files #include a private header first (or none)")
لكل x في HBSOURCES:
    چھاپو(f"Checking {x}")
    نال open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    includes = re.findall(r"#.*include.*", content)
    إذا includes:
        إذا ليس لمبائی(re.findall(r'".*\.hh"', includes[0])):
            چھاپو("failure on %s" % x)
            stat = 1

چھاپو("Checking that there is no #include <hb-*.h>")
لكل x في HBHEADERS + HBSOURCES:
    چھاپو(f"Checking {x}")
    نال open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا re.findall("#.*include.*<.*hb", content):
        چھاپو("failure on %s" % x)
        stat = 1

sys.exit(stat)
