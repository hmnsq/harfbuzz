#!/usr/bin/env python3

आनयन os
आनयन re
आनयन sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    फर्काऊ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) लागि x अन्तः os.getenv("HBHEADERS", "").split()] वा [
    x लागि x अन्तः os.listdir(".") यदि x.startswith("hb") र x.endswith(".h")
]
HBSOURCES = [removeprefix(x) लागि x अन्तः os.getenv("HBSOURCES", "").split()] वा [
    x लागि x अन्तः os.listdir(".") यदि x.startswith("hb") र x.endswith((".cc", ".hh"))
]


stat = 0

लागि x अन्तः HBHEADERS + HBSOURCES:
    यदि होइन x.endswith("h") वा x == "hb-gobject-structs.h":
        जारी
    print(f"Checking {x}")
    tag = (
        x.upper()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.path.sep, "_")
        .replace("/", "_")
    )
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    यदि len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
