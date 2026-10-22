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

కొరకు x లో HBHEADERS + HBSOURCES:
    ఒకవేళ కాదు x.endswith("h") లేదా x == "hb-gobject-structs.h":
        కొనసాగు
    ముద్రించు(f"Checking {x}")
    ట్యాగ్_2 = (
        x.పై()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.మార్గం.sep, "_")
        .replace("/", "_")
    )
    తోడుగా open(x, "r", encoding="utf-8") ఇవ f:
        content = f.పఠ()
    ఒకవేళ పొడవు(re.findall(ట్యాగ్_2 + r"\b", content)) != 3:
        ముద్రించు(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, ట్యాగ్_2)
        )
        stat = 1

sys.exit(stat)
