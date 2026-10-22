#!/usr/bin/env python3

import os
import re
import sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) за x in os.getenv("HBHEADERS", "").split()] or [
    x за x in os.listdir(".") ако x.startswith("hb") and x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
за x in HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    за s in re.findall(r"\n.+\nhb_.+\n", content):
        ако not s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
