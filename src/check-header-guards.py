#!/usr/bin/env python3

আনযন os
আনযন re
আনযন sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    প্রত্যাগম os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) প্রতি x অন্তঃ os.getenv("HBHEADERS", "").split()] বা [
    x প্রতি x অন্তঃ os.listdir(".") যদি x.startswith("hb") চ x.endswith(".h")
]
HBSOURCES = [removeprefix(x) প্রতি x অন্তঃ os.getenv("HBSOURCES", "").split()] বা [
    x প্রতি x অন্তঃ os.listdir(".") যদি x.startswith("hb") চ x.endswith((".cc", ".hh"))
]


stat = 0

প্রতি x অন্তঃ HBHEADERS + HBSOURCES:
    যদি ন x.endswith("h") বা x == "hb-gobject-structs.h":
        অনুবর্ত
    print(f"Checking {x}")
    tag = (
        x.upper()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.path.sep, "_")
        .replace("/", "_")
    )
    with open(x, "r", encoding="utf-8") ইব f:
        content = f.পঠ()
    যদি len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
