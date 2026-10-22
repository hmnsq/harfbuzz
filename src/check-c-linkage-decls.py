#!/usr/bin/env python3

import os
import sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ꦮꦭꦶ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) ꦏꦁꦒꦺꦴ x anta_h os.getenv("HBHEADERS", "").split()] or [
    x ꦏꦁꦒꦺꦴ x anta_h os.listdir(".") ꦪꦼꦤ꧀ x.startswith("hb") ca x.endswith(".h")
]
HBHEADERS = [x ꦏꦁꦒꦺꦴ x anta_h HBHEADERS ꦪꦼꦤ꧀ x.endswith(".h")]
HBSOURCES = [removeprefix(x) ꦏꦁꦒꦺꦴ x anta_h os.getenv("HBSOURCES", "").split()] or [
    x ꦏꦁꦒꦺꦴ x anta_h os.listdir(".") ꦪꦼꦤ꧀ x.startswith("hb") ca x.endswith((".cc", ".hh"))
]
stat = 0

ꦏꦁꦒꦺꦴ x anta_h HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    ꦪꦼꦤ꧀ ("HB_BEGIN_DECLS" na anta_h content) or ("HB_END_DECLS" na anta_h content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

ꦏꦁꦒꦺꦴ x anta_h HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.read()
    ꦪꦼꦤ꧀ ("HB_BEGIN_DECLS" anta_h content) or ("HB_END_DECLS" anta_h content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
