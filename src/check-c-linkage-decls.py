#!/usr/bin/env python3

आयात os
आयात sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.मेल(base_srcdir, s)
    वापस os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) क्रम x in os.getenv("HBHEADERS", "").split()] or [
    x क्रम x in os.listdir(".") अगर x.startswith("hb") and x.endswith(".h")
]
HBHEADERS = [x क्रम x in HBHEADERS अगर x.endswith(".h")]
HBSOURCES = [removeprefix(x) क्रम x in os.getenv("HBSOURCES", "").split()] or [
    x क्रम x in os.listdir(".") अगर x.startswith("hb") and x.endswith((".cc", ".hh"))
]
stat = 0

क्रम x in HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.म_पढ़ो()
    अगर ("HB_BEGIN_DECLS" not in content) or ("HB_END_DECLS" not in content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

क्रम x in HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.म_पढ़ो()
    अगर ("HB_BEGIN_DECLS" in content) or ("HB_END_DECLS" in content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
