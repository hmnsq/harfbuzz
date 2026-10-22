#!/usr/bin/env python3

درآمد os
درآمد re
درآمد sys

srcdir = sys.argv[1]
بنیاد_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


تعریف removeprefix(s):
    abs_راستہ = os.راستہ.join(بنیاد_srcdir, s)
    واپس os.راستہ.relpath(abs_راستہ, srcdir)


HBHEADERS = [os.راستہ.basename(x) برائے x میں os.getenv("HBHEADERS", "").بانٹو()] یا [
    x برائے x میں os.listdir(".") اگر x.startswith("hb") اور x.endswith(".h")
]

HBSOURCES = [removeprefix(x) برائے x میں os.getenv("HBSOURCES", "").بانٹو()] یا [
    x برائے x میں os.listdir(".") اگر x.startswith("hb") اور x.endswith((".cc", ".hh"))
]


stat = 0

چھاپو(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
برائے x میں HBHEADERS:
    اگر x == "hb.h" یا x == "hb-common.h":
        جاری
    چھاپو(f"Checking {x}")
    ساتھ open(x, "r", encoding="utf-8") بطور f:
        content = f.اقرأ()
    پہلا = re.findall(r"#.*include.*", content)[0]
    اگر پہلا نہیں میں ['#include "hb.h"', '#include "hb-common.h"']:
        چھاپو("failure on %s" % x)
        stat = 1

چھاپو("Checking that source files #include a private header first (or none)")
برائے x میں HBSOURCES:
    چھاپو(f"Checking {x}")
    ساتھ open(x, "r", encoding="utf-8") بطور f:
        content = f.اقرأ()
    includes = re.findall(r"#.*include.*", content)
    اگر includes:
        اگر نہیں لمبائی(re.findall(r'".*\.hh"', includes[0])):
            چھاپو("failure on %s" % x)
            stat = 1

چھاپو("Checking that there is no #include <hb-*.h>")
برائے x میں HBHEADERS + HBSOURCES:
    چھاپو(f"Checking {x}")
    ساتھ open(x, "r", encoding="utf-8") بطور f:
        content = f.اقرأ()
    اگر re.findall("#.*include.*<.*hb", content):
        چھاپو("failure on %s" % x)
        stat = 1

sys.exit(stat)
