#!/usr/bin/env python3

আমদানি os
আমদানি re
আমদানি sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ফেরত os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) জন্য x মধ্যে os.getenv("HBHEADERS", "").split()] অথবা [
    x জন্য x মধ্যে os.listdir(".") যদি x.startswith("hb") এবং x.endswith(".h")
]
HBSOURCES = [removeprefix(x) জন্য x মধ্যে os.getenv("HBSOURCES", "").split()] অথবা [
    x জন্য x মধ্যে os.listdir(".") যদি x.startswith("hb") এবং x.endswith((".cc", ".hh"))
]


stat = 0

জন্য x মধ্যে HBHEADERS + HBSOURCES:
    যদি নয় x.endswith("h") অথবা x == "hb-gobject-structs.h":
        চালু
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
