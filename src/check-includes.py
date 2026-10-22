#!/usr/bin/env python3

దిగుమతి os
దిగుమతి re
దిగుమతి sys

srcdir = sys.argv[1]
పునాది_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


నిర్వచనం removeprefix(s):
    abs_మార్గం = os.మార్గం.join(పునాది_srcdir, s)
    తిరిగి os.మార్గం.relpath(abs_మార్గం, srcdir)


HBHEADERS = [os.మార్గం.basename(x) కొరకు x లో os.getenv("HBHEADERS", "").విభజించు()] లేదా [
    x కొరకు x లో os.listdir(".") ఒకవేళ x.startswith("hb") మరియు x.endswith(".h")
]

HBSOURCES = [removeprefix(x) కొరకు x లో os.getenv("HBSOURCES", "").విభజించు()] లేదా [
    x కొరకు x లో os.listdir(".") ఒకవేళ x.startswith("hb") మరియు x.endswith((".cc", ".hh"))
]


stat = 0

ముద్రించు(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
కొరకు x లో HBHEADERS:
    ఒకవేళ x == "hb.h" లేదా x == "hb-common.h":
        కొనసాగు
    ముద్రించు(f"Checking {x}")
    తోడుగా open(x, "r", encoding="utf-8") ఇవ f:
        content = f.పఠ()
    మొదటి = re.findall(r"#.*include.*", content)[0]
    ఒకవేళ మొదటి కాదు లో ['#include "hb.h"', '#include "hb-common.h"']:
        ముద్రించు("failure on %s" % x)
        stat = 1

ముద్రించు("Checking that source files #include a private header first (or none)")
కొరకు x లో HBSOURCES:
    ముద్రించు(f"Checking {x}")
    తోడుగా open(x, "r", encoding="utf-8") ఇవ f:
        content = f.పఠ()
    includes = re.findall(r"#.*include.*", content)
    ఒకవేళ includes:
        ఒకవేళ కాదు పొడవు(re.findall(r'".*\.hh"', includes[0])):
            ముద్రించు("failure on %s" % x)
            stat = 1

ముద్రించు("Checking that there is no #include <hb-*.h>")
కొరకు x లో HBHEADERS + HBSOURCES:
    ముద్రించు(f"Checking {x}")
    తోడుగా open(x, "r", encoding="utf-8") ఇవ f:
        content = f.పఠ()
    ఒకవేళ re.findall("#.*include.*<.*hb", content):
        ముద్రించు("failure on %s" % x)
        stat = 1

sys.exit(stat)
