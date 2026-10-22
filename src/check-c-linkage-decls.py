#!/usr/bin/env python3

import os
import sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    trả_về os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) cho x in os.getenv("HBHEADERS", "").split()] or [
    x cho x in os.listdir(".") nếu x.startswith("hb") and x.endswith(".h")
]
HBHEADERS = [x cho x in HBHEADERS nếu x.endswith(".h")]
HBSOURCES = [removeprefix(x) cho x in os.getenv("HBSOURCES", "").split()] or [
    x cho x in os.listdir(".") nếu x.startswith("hb") and x.endswith((".cc", ".hh"))
]
stat = 0

cho x in HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    nếu ("HB_BEGIN_DECLS" not in content) or ("HB_END_DECLS" not in content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

cho x in HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    nếu ("HB_BEGIN_DECLS" in content) or ("HB_END_DECLS" in content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
