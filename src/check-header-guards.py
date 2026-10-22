#!/usr/bin/env python3

import os
import re
import sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    반환 os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) 위해 x in os.getenv("HBHEADERS", "").split()] or [
    x 위해 x in os.listdir(".") 만약 x.startswith("hb") and x.endswith(".h")
]
HBSOURCES = [removeprefix(x) 위해 x in os.getenv("HBSOURCES", "").split()] or [
    x 위해 x in os.listdir(".") 만약 x.startswith("hb") and x.endswith((".cc", ".hh"))
]


stat = 0

위해 x in HBHEADERS + HBSOURCES:
    만약 not x.endswith("h") or x == "hb-gobject-structs.h":
        continue
    print(f"Checking {x}")
    tag = (
        x.upper()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.path.sep, "_")
        .replace("/", "_")
    )
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    만약 len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
