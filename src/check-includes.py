#!/usr/bin/env python3

استورد os
استورد re
استورد sys

srcdir = sys.argv[1]
بنياد_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


وصف removeprefix(s):
    abs_رستو = os.رستو.join(بنياد_srcdir, s)
    أرجع os.رستو.relpath(abs_رستو, srcdir)


HBHEADERS = [os.رستو.basename(x) لكل x في os.getenv("HBHEADERS", "").ورهايو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith(".h")
]

HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").ورهايو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith((".cc", ".hh"))
]


stat = 0

ڇاپيو(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
لكل x في HBHEADERS:
    إذا x == "hb.h" أو x == "hb-common.h":
        استمر
    ڇاپيو(f"Checking {x}")
    سان open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    پهريون = re.findall(r"#.*include.*", content)[0]
    إذا پهريون ليس في ['#include "hb.h"', '#include "hb-common.h"']:
        ڇاپيو("failure on %s" % x)
        stat = 1

ڇاپيو("Checking that source files #include a private header first (or none)")
لكل x في HBSOURCES:
    ڇاپيو(f"Checking {x}")
    سان open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    includes = re.findall(r"#.*include.*", content)
    إذا includes:
        إذا ليس ڊيگهه(re.findall(r'".*\.hh"', includes[0])):
            ڇاپيو("failure on %s" % x)
            stat = 1

ڇاپيو("Checking that there is no #include <hb-*.h>")
لكل x في HBHEADERS + HBSOURCES:
    ڇاپيو(f"Checking {x}")
    سان open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا re.findall("#.*include.*<.*hb", content):
        ڇاپيو("failure on %s" % x)
        stat = 1

sys.exit(stat)
