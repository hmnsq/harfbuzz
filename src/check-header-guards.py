#!/usr/bin/env python3

இறக்குமதி os
இறக்குமதி re
இறக்குமதி sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    திரும்பு os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) க்கு x உள் os.getenv("HBHEADERS", "").split()] அல்லது [
    x க்கு x உள் os.listdir(".") எனில் x.startswith("hb") மற்றும் x.endswith(".h")
]
HBSOURCES = [removeprefix(x) க்கு x உள் os.getenv("HBSOURCES", "").split()] அல்லது [
    x க்கு x உள் os.listdir(".") எனில் x.startswith("hb") மற்றும் x.endswith((".cc", ".hh"))
]


stat = 0

க்கு x உள் HBHEADERS + HBSOURCES:
    எனில் இல்லை x.endswith("h") அல்லது x == "hb-gobject-structs.h":
        தொடர்
    print(f"Checking {x}")
    tag = (
        x.upper()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.path.sep, "_")
        .replace("/", "_")
    )
    with open(x, "r", encoding="utf-8") இவ f:
        content = f.பட()
    எனில் len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
