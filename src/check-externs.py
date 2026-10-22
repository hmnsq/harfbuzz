#!/usr/bin/env python3

import os
import re
import sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) 为 x in os.getenv("HBHEADERS", "").split()] or [
    x 为 x in os.listdir(".") 如果 x.startswith("hb") and x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
为 x in HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    为 s in re.findall(r"\n.+\nhb_.+\n", content):
        如果 not s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
