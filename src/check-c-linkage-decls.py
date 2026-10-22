#!/usr/bin/env python3

దిగుమతి os
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
HBHEADERS = [x కొరకు x లో HBHEADERS ఒకవేళ x.endswith(".h")]
HBSOURCES = [removeprefix(x) కొరకు x లో os.getenv("HBSOURCES", "").విభజించు()] లేదా [
    x కొరకు x లో os.listdir(".") ఒకవేళ x.startswith("hb") మరియు x.endswith((".cc", ".hh"))
]
stat = 0

కొరకు x లో HBHEADERS:
    ముద్రించు(f"Checking {x}")
    తోడుగా open(x, "r", encoding="utf-8") ఇవ f:
        content = f.పఠ()
    ఒకవేళ ("HB_BEGIN_DECLS" కాదు లో content) లేదా ("HB_END_DECLS" కాదు లో content):
        ముద్రించు(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

కొరకు x లో HBSOURCES:
    ముద్రించు(f"Checking {x}")
    తోడుగా open(x, "r", encoding="utf-8") ఇవ f:
        content = f.పఠ()
    ఒకవేళ ("HB_BEGIN_DECLS" లో content) లేదా ("HB_END_DECLS" లో content):
        ముద్రించు("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
