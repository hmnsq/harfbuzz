#!/usr/bin/env python3

import os
import re
import sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) དོན་ལུ x anta_h os.getenv("HBHEADERS", "").split()] or [
    x དོན་ལུ x anta_h os.listdir(".") གལ་སྲིད x.startswith("hb") ca x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
དོན་ལུ x anta_h HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    དོན་ལུ s anta_h re.findall(r"\n.+\nhb_.+\n", content):
        གལ་སྲིད na s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
