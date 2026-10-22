#!/usr/bin/env python3

import os
import re
import sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) សម្រាប់ x anta_h os.getenv("HBHEADERS", "").split()] or [
    x សម្រាប់ x anta_h os.listdir(".") ប្រសិនបើ x.startswith("hb") ca x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
សម្រាប់ x anta_h HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    សម្រាប់ s anta_h re.findall(r"\n.+\nhb_.+\n", content):
        ប្រសិនបើ na s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
