#!/usr/bin/env python3

దిగుమతి os
దిగుమతి re
దిగుమతి sys

srcdir = sys.argv[1]
పునాది_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.మార్గం.basename(x) కొరకు x లో os.getenv("HBHEADERS", "").విభజించు()] లేదా [
    x కొరకు x లో os.listdir(".") ఒకవేళ x.startswith("hb") మరియు x.endswith(".h")
]

stat = 0

ముద్రించు("Checking that all public symbols are exported with HB_EXTERN")
కొరకు x లో HBHEADERS:
    ముద్రించు(f"Checking {x}")
    తోడుగా open(x, "r", encoding="utf-8") ఇవ f:
        content = f.పఠ()
    కొరకు s లో re.findall(r"\n.+\nhb_.+\n", content):
        ఒకవేళ కాదు s.startswith("\nHB_EXTERN "):
            ముద్రించు("failure on:", s)
            stat = 1

sys.exit(stat)
